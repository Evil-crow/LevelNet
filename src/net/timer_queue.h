/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_TIMER_QUEUE_H
#define LEVELNET_NET_TIMER_QUEUE_H

#include <set>
#include <map>
#include <atomic>
#include <memory>
#include <vector>

#include "net/timer.h"
#include "net/timer_id.hpp"

namespace levelnet {

class Timer;
class TimerID;
class Channel;
class EventLoop;

class TimerQueue {
 public:
  explicit TimerQueue(EventLoop *loop);
  ~TimerQueue();

  TimerID AddTimer(Timer::TimeCallback callback, Time when , double time_space);
  void CancelTimer(TimerID timer_id);

 private:
  using TimerPtr = std::shared_ptr<Timer>;

  std::vector<std::pair<Time, TimerPtr>> GetMatured(Time now);
  void Reset(std::vector<std::pair<Time, TimerPtr>> &matured, Time now);
  bool Insert(TimerPtr timer);

  EventLoop *loop_;
  int timerfd_;
  std::unique_ptr<Channel> timer_channel_;

  std::atomic<bool> calling_matured_timers_;
  std::multimap<Time, TimerPtr> timers_;
  std::map<int64_t, TimerPtr> active_timers_;
};

}

#endif //LEVELNET_NET_TIMER_QUEUE_H
