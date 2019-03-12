/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/event_loop.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <vector>

#include "net/epoll.h"
#include "net/channel.h"
#include "net/timer_queue.h"
#include "utility/logger/logger.h"

namespace levelnet {

namespace detail {

int CreateEventFd() {
  int fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (fd < 0) {
    LOG(ERROR) << "detail::CreateEventFd() => Eventfd Error";
    std::abort();
  }
  return fd;
}

}

constexpr int KTimeout = 100;

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      event_handling_(false),
      calling_pending_functor_(false),
      event_fd_(detail::CreateEventFd()),
      epoller_(std::make_unique<Epoll>(this)),
      event_channel_(std::make_unique<Channel>(this, event_fd_)),
      timer_queue_(std::make_unique<TimerQueue>(this))
{
  active_channels_.reserve(1024);

  event_channel_->SetFlags(READ, true);
  epoller_->Add(event_channel_.get());
  event_channel_->SetReadCallback([this]() {
    eventfd_t data;
    if (::eventfd_read(event_fd_, &data) < 0) {
      LOG(ERROR) << "EventLoop::EventLoop()::SetReadCallback() => Event_read Error";
      std::abort();
    } else {
      LOG(INFO) << "Handle Wake Up";
    }
  });
}

EventLoop::~EventLoop() {
  this->Quit();
  ::close(event_fd_);
  active_channels_.clear();
  pending_functors_.clear();
}

void EventLoop::Loop() {
  looping_ = true;
  quit_ = false;

  while (!quit_) {
    event_handling_ = true;
    active_channels_.clear();
    auto nums = epoller_->Poll(active_channels_, KTimeout);
    for (const auto &var : active_channels_)
      var->HandleEvent();
    DoPendingFunctor();
    event_handling_ = false;
  }
  looping_ = false;
}

void EventLoop::Quit() {
  quit_ = true;
  if (event_handling_)
    WakeUp();
}

bool EventLoop::AddChannel(Channel *channel) {
  return epoller_->Add(channel);
}

bool EventLoop::UpdateChannel(Channel *channel) {
  return epoller_->Update(channel);
}

bool EventLoop::DeleteChannel(Channel *channel) {
  return epoller_->Delete(channel);
}

void EventLoop::WakeUp() {
  eventfd_t data = 6;
  ::eventfd_write(event_fd_, data);
  LOG(INFO) << "Loop Wake Up";
}

void EventLoop::DoPendingFunctor() {
  calling_pending_functor_ = true;
  std::vector<Functor> temp;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    temp.swap(pending_functors_);
  }
  for (const auto &functor : temp)
    functor();
  calling_pending_functor_ = false;
}

void EventLoop::TaskInLoop(EventLoop::Functor cb) {
  {
    std::lock_guard<std::mutex> lk(mutex_);
    pending_functors_.push_back(std::move(cb));
  }

  if (calling_pending_functor_)
    WakeUp();
}

TimerID EventLoop::RunAt(const Timer::TimeCallback &callback, Time time) {
  return timer_queue_->AddTimer(std::move(callback), time, 0);
}

TimerID EventLoop::RunAfter(const Timer::TimeCallback &callback, double after) {
  Time time_after(AddTime(Time::Now(), after));
//  return timer_queue_->AddTimer(callback, time_after, 0);
  return RunAt(callback, time_after);
}

TimerID EventLoop::RunEvery(const Timer::TimeCallback &callback, double space) {
  Time time_every(AddTime(Time::Now(), space));
  return timer_queue_->AddTimer(callback, time_every, space);
}

void EventLoop::Cancel(TimerID timer_id) {
  timer_queue_->CancelTimer(timer_id);
}

}
