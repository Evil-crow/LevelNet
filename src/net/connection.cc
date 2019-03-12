/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/connection.h"

#include <unistd.h>

#include "net/channel.h"
#include "net/socket.h"
#include "net/socketops.h"
#include "net/event_loop.h"
#include "utility/logger/logger.h"

namespace levelnet {

Connection::Connection(EventLoop *loop, int fd, const IPAddress &address)
    : loop_(loop),
      address_(address),
      socket_(std::make_unique<Socket>(fd)),
      channel_(std::make_unique<Channel>(loop, fd))
{
  parser_ = std::make_unique<Parser>();
}

bool Connection::ConnectionEstablished() {
  channel_->SetFlags(ET | WRITE | READ | HUP | ERR, true);
  channel_->SetReadCallback([this]() {
    read_buffer_.ReadFd(socket_->Fd());
    auto len = message_callback_(this, read_buffer_, parser_);
    read_buffer_.Reprepare(len);
  });
  channel_->SetWriteCallback([this]() {
    if (write_callback_) {
      write_callback_();
    }
    if (write_queue_.DoTask()) {
      channel_->SetFlags(WRITE, false);
      loop_->UpdateChannel(channel_.get());
    }
  });
  channel_->SetCloseCallback([this]() {
    loop_->TaskInLoop([this]() { close_callback_(socket_->Fd()); });
  });
  channel_->SetErrorCallback([this]() {
    loop_->TaskInLoop([this]() { ErrorCallback(); });
  });

  return loop_->UpdateChannel(channel_.get());
}

bool Connection::ConnectionDestoryed() {
  channel_->ClearFlags();
  return loop_->DeleteChannel(channel_.get());
}

bool Connection::ShutdownConnection() {
  loop_->TaskInLoop([this]() { detail::ShutdownWrite(socket_->Fd()); });
  return true;
}

bool Connection::ForceCloseConnection() {
  loop_->TaskInLoop([this]() { close_callback_(socket_->Fd()); });
  return true;
}

void Connection::SendData(const unsigned char *data, size_t total) {
  size_t completed_(0), remained_(total);
  while (true) {
    auto var = ::write(socket_->Fd(), data, total);
    if (var > 0) {
      if (var == total) {
        return ;
      } else {
        completed_ += var;
        remained_ -= var;
      }
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        channel_->SetFlags(WRITE, true);
        loop_->UpdateChannel(channel_.get());
        write_queue_.TaskInQueue(socket_->Fd(), data, completed_, remained_);
      } else {
        LOG(ERROR) << "Connection::SendData() => Send Data Error";
        std::abort();
      }
    }
  }
}

void Connection::SetConnectionCallback(const EventCallback &cb) {
  connection_callback_ = cb;
}

void Connection::SetWriteCallback(const EventCallback &cb) {
  write_callback_ = cb;
}

void Connection::SetMessageCallback(const MessageCallback &cb) {
  message_callback_ = cb;
}

void Connection::SetCloseCallback(const CloseCallback &cb) {
  close_callback_ = cb;
}

void Connection::ErrorCallback() {
  LOG(ERROR) << "Connection::ErrorCallback() => Error Occured";
  loop_->TaskInLoop([this]() { close_callback_(socket_->Fd()); });
}

}
