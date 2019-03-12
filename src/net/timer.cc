/**
 * Created by Crow on 3/11/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/timer.h"

#include "utility/time.h"

namespace levelnet {

std::atomic<int64_t> Timer::atomic_seq_counter_ {0};

Timer::Timer(const TimeCallback &callback, Time when, double time_space)
    : callback_(callback),
      when_(when),
      time_space_(static_cast<const time_t>(time_space)),
      is_repeat_(time_space > 0.0),
      seq_(Timer::AtomicSeqCounter()) { ; }

void Timer::Tick() { callback_(); }

bool Timer::Restart(Time now) {
  if (is_repeat_) {
    when_ = AddTime(now, time_space_);
  } else {
    when_ = Time::Invaild();
  }
}

Time Timer::When() const { return when_; }

bool Timer::Repeat() const { return is_repeat_; }

int64_t Timer::Seq() const { return seq_; }

int64_t Timer::AtomicSeqCounter() {
  atomic_seq_counter_++;
  return atomic_seq_counter_.load();
}

}