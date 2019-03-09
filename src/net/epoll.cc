/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/epoll.h"

#include <unistd.h>
#include <sys/epoll.h>
#include <cassert>

#include "net/channel.h"
#include "utility/logger/logger.h"

namespace levelnet {

const int KEpollTimeout = 100;

namespace detail {
int CreateEpollFd() {
  int fd = ::epoll_create1(EPOLL_CLOEXEC);
  if (fd < 0) {
    LOG(ERROR) << "detail::CreateEPollFd() => fd < 0";
    std::abort();
  }

  return fd;
}
}

Epoll::Epoll(levelnet::EventLoop *loop) noexcept
  : loop_(loop),
    ep_fd_(detail::CreateEpollFd())
{
  active_events_.reserve(KMaxEventSize);
}

Epoll::~Epoll() {
  ::close(ep_fd_);
  loop_ = nullptr;
}

int Epoll::Poll(std::vector<Channel *> &active_channels, int timeout) {
  active_events_.clear();
  active_events_.resize(KMaxEventSize);
  int nums = ::epoll_wait(ep_fd_, active_events_.data(), (int) KMaxEventSize, timeout);
  if (nums > 0) {
    auto fill_num = FillActiveChannels(nums, active_channels);
    assert(fill_num == nums);
    return nums;
  } else if (nums == 0) {
    return nums;
  } else {
    LOG(ERROR) << "Epoll::Poll() => Epoll_wait Error";
    std::abort();
  }
}

int Epoll::FillActiveChannels(int event_num, std::vector<Channel *> &active_channels) {
  for (int i = 0; i < event_num; ++i) {
    auto channel = reinterpret_cast<Channel *>(active_events_[i].data.ptr);
    channel->SetEvents(active_events_[i].events);
    active_channels.push_back(channel);
  }
}

bool Epoll::Add(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if(::epoll_ctl(ep_fd_, EPOLL_CTL_ADD, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Add() => Add Event Error";
    std::abort();
  }
  return true;
}

bool Epoll::Update(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if (::epoll_ctl(ep_fd_, EPOLL_CTL_MOD, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Update() => Update Event Error";
    std::abort();
  }
  return true;
}

bool Epoll::Delete(Channel *channel) {
  epoll_event ep_event{};
  ep_event.events = channel->Flags();
  ep_event.data.ptr = channel;

  if (::epoll_ctl(ep_fd_, EPOLL_CTL_DEL, channel->Fd(), &ep_event)) {
    LOG(ERROR) << "Epoll::Delete() => Delete Event Error";
    std::abort();
  }
  return true;
}

}
