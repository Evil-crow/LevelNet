/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/timer_queue.h"

#include <unistd.h>
#include <sys/timerfd.h>
#include <cassert>
#include <algorithm>

#include "net/channel.h"
#include "net/event_loop.h"
#include "utility/logger/logger.h"

namespace levelnet {

namespace detail {

int CreateTimerFd() {
  int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  if (fd < 0) {
    LOG(ERROR) << "detail::CreateTimerFd() => Create Timerfd Error";
    std::abort();
  }
  return fd;
}

timespec HowmuchTimeFromNow(Time when) {
  int64_t microseconds = when.MicrosecondsSinceUnix() - Time::Now().MicrosecondsSinceUnix();
  if (microseconds < 100)
    microseconds = 100;

  timespec tv{};
  tv.tv_sec = static_cast<time_t>(microseconds / Time::KMicrosecondsPerSecond);
  tv.tv_nsec = static_cast<long>((microseconds % Time::KMicrosecondsPerSecond) * 1000);

  return tv;
}

void ReadTimerFd(int timerfd, Time now) {
  uint64_t t;
  auto n = ::read(timerfd, &t, sizeof(t));
  assert(n == sizeof(t));
}

void ResetTimerFd(int timerfd, Time when) {
  itimerspec new_value{}, old_value{};
  ::memset(&new_value, 0, sizeof(itimerspec));
  ::memset(&old_value, 0, sizeof(itimerspec));
  new_value.it_value = HowmuchTimeFromNow(when);
  int ret = ::timerfd_settime(timerfd, 0, &new_value, &old_value);
  if (ret) {
    LOG(ERROR) << "detail::ResetTimerFd => Set Timer Error";
    std::abort();
  }
}

}

TimerQueue::TimerQueue(EventLoop *loop)
    : loop_(loop),
      timerfd_(detail::CreateTimerFd()),
      timer_channel_(std::make_unique<Channel>(loop, timerfd_)),
      calling_matured_timers_(false) {

  timer_channel_->SetReadCallback([this]() {
    Time now(Time::Now());
    detail::ReadTimerFd(timerfd_, now);

    auto matured = GetMatured(now);
    calling_matured_timers_.store(true);
    for (const auto &var : matured)
      var.second->Tick();
    calling_matured_timers_.store(false);

    Reset(matured, now);
  });

  timer_channel_->SetFlags(READ, true);
  loop_->AddChannel(timer_channel_.get());
}

TimerQueue::~TimerQueue() {
  timer_channel_->ClearFlags();
  loop_->DeleteChannel(timer_channel_.get());
  ::close(timerfd_);
}

TimerID TimerQueue::AddTimer(Timer::TimeCallback callback, Time when, double time_space) {
  auto timer = std::make_shared<Timer>(std::move(callback), when, time_space);
  loop_->TaskInLoop([timer, this]() {
    auto timer_when = timer->When();
    bool change = Insert(timer);
    if (change) {
      detail::ResetTimerFd(timerfd_, timer_when);
    }
  });

  return TimerID{timer.get(), timer->Seq()};
}

void TimerQueue::CancelTimer(TimerID timer_id) {
  loop_->TaskInLoop([this, timer_id]() {
    assert(timers_.size() == active_timers_.size());
    auto iter = active_timers_.find(timer_id.seq_);
    if (iter == active_timers_.end()) {
      LOG(ERROR) << "TimerQueue::CancelTimer => Timer Lost";
      std::abort();
    }
    active_timers_.erase(iter);
    auto range = timers_.equal_range(timer_id.t_->When());
    for (auto i = range.first; i != range.second; ++i)
      if (i->second == iter->second)
        timers_.erase(i);
    assert(timers_.size() == active_timers_.size());
  });
}

std::vector<std::pair<Time, TimerQueue::TimerPtr>> TimerQueue::GetMatured(Time now) {
  std::vector<std::pair<Time, TimerPtr>> entry;
  assert(timers_.size() == active_timers_.size());
  auto iter = timers_.lower_bound(now);

  std::copy(timers_.begin(), iter, std::back_inserter(entry));
  for (const auto &var : entry) {
    size_t n = active_timers_.erase(var.second->Seq());
    assert(n == 1);
  }
  timers_.erase(timers_.begin(), iter);
  assert(timers_.size() == active_timers_.size());
  return entry;
}

void TimerQueue::Reset(std::vector<std::pair<Time, TimerPtr>> &matured, Time now) {
  Time next_matured;
  for (auto &var : matured)
    if (var.second->Repeat()) {
      var.second->Restart(now);
      Insert(std::move(var.second));
    }

  if (!timers_.empty())
    next_matured = timers_.cbegin()->second->When();
  if (next_matured.Vaild())
    detail::ResetTimerFd(timerfd_, next_matured);
}

bool TimerQueue::Insert(TimerQueue::TimerPtr timer) {
  assert(timers_.size() == active_timers_.size());
  bool changed = false;
  Time time = timer->When();
  auto iter = timers_.cbegin();
  if (iter == timers_.cend() || time < iter->first)
    changed = true;

  TimerPtr temp(timer);
  timers_.insert(std::make_pair<Time, TimerPtr>(timer->When(), std::move(timer)));
  active_timers_.insert(std::make_pair<int64_t, TimerPtr>(temp->Seq(), std::move(temp)));
  assert(timers_.size() == active_timers_.size());

  return changed;
}

}