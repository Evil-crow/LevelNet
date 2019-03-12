/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/channel.h"

#include "net/event_loop.h"

namespace levelnet {

Channel::Channel(EventLoop *loop, int fd)
  : loop_(loop), fd_(fd) { ; }

void Channel::SetReadCallback(levelnet::Channel::EventCallback &&cb)  { read_callback_ = cb; }
void Channel::SetWriteCallback(levelnet::Channel::EventCallback &&cb) { write_callback_ = cb; }
void Channel::SetCloseCallback(levelnet::Channel::EventCallback &&cb) { close_callback_ = cb; }
void Channel::SetErrorCallback(levelnet::Channel::EventCallback &&cb) { err_callback_ = cb; }

void Channel::SetFlags(unsigned long  event, bool flag) {
  flags_ = flag ? (flags_ | event) : (flags_ & ~event);
}

void Channel::ClearFlags() {
  flags_ = 0;
}

void Channel::SetEvents(unsigned int events) {
  revents_ = events;
}

void Channel::HandleEvent() {
  event_handling_ = true;
  if (revents_ & ChannelEvent::ERR) {
    if (err_callback_)
      err_callback_();
  } else if (revents_ & ChannelEvent::HUP) {
    if (close_callback_)
      close_callback_();
  } else if (revents_ & ChannelEvent::READ) {
    if (read_callback_)
      read_callback_();
  } else if (revents_ & ChannelEvent::WRITE) {
    if (write_callback_)
      write_callback_();
  }
  event_handling_ = false;
}

int Channel::Fd() const {
  return fd_;
}

unsigned int Channel::Flags() const {
  return flags_;
}

}
