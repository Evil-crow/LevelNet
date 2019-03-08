/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/epoll.h"

#include <unistd.h>
#include <sys/epoll.h>

#include "net/channel.h"
#include "utility/logger/logger.h"

namespace levelnet {

const int KEpollTimeout = 100;

namespace {
int CreateEpollFd() {
  int fd = ::epoll_create1(EPOLL_CLOEXEC);
  if (fd < 0) {
    LOG(ERROR) << "CreateEPollFd => fd < 0";
    std::abort();
  }

  return fd;
}
}

Epoll::Epoll(levelnet::EventLoop *loop) noexcept
  : loop_(loop),
    ep_fd_(CreateEpollFd())
{

  }

Epoll::~Epoll() {
  ::close(ep_fd_);
  loop_ = nullptr;
}

int Epoll::Poll(std::vector<Channel *> &active_channel, int timeout) {
  int nums = ::epoll_wait(ep_fd_, active_events_.data(), KMaxEventSize, timeout);
}

bool Epoll::Add(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if(::epoll_ctl(ep_fd_, EPOLL_CTL_ADD, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Add => Add Event Error";
    std::abort();
  }
  return true;
}

bool Epoll::Update(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if (::epoll_ctl(ep_fd_, EPOLL_CTL_MOD, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Update => Update Event Error";
    std::abort();
  }
  return true;
}

bool Epoll::Delete(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if (::epoll_ctl(ep_fd_, EPOLL_CTL_DEL, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Delete => Delete Event Error";
    std::abort();
  }
  return true;
}

}
