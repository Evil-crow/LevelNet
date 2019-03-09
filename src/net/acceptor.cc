/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/acceptor.h"

#include "net/socket.h"
#include "net/channel.h"
#include "net/event_loop.h"
#include "utility/logger/logger.h"

namespace levelnet {

Acceptor::Acceptor(EventLoop *loop, const IPAddress &address)
    : loop_(loop),
      address_(address),
      socket_(std::make_unique<Socket>()),
      channel_(std::make_unique<Channel>(loop, socket_->Fd())),
      is_listening_(false) {
  socket_->SetReuseport(true);
  socket_->SetKeepalive(true);
  socket_->SetTcpNoNegle(true);
}

bool Acceptor::Listening() {
  socket_->Bind(address_);
  socket_->Listen();

  channel_->SetFlags(R | ET, true);
  channel_->SetReadCallback([]() {
    IPAddress peer_address;
    while (true) {
      int connfd = socket_->Accept(peer_address);
      if (connfd > 0) {
        // WARNING: NOT copy ctor, RAII Handle will close fd, you have three methods:
        // 1. move arguments, NOT USE MOVE , It'll close Handle!
        // 2. use file descriptor, NOT directly RAII Handle,(RAII Handle will close the fd)
        // 3. use smart pointer. std::unique_ptr / std::shared_ptr.
        if (callback_)
          callback_(connfd, peer_address);
      } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          LOG(INFO) << "Acceptor::HandleEvent";
          break;
        } else {
          LOG(ERROR) << "Acceptor::HandleEvent() => Accept Error";
          std::abort();
        }
      }
    }
  });
  loop_->AddChannel(channel_.get());
}

void Acceptor::SetConnectionCallback(ConnectionCallback cb) {
  callback_ = std::move(cb);
}

bool Acceptor::IsListening() {
  return is_listening_;
}

}
