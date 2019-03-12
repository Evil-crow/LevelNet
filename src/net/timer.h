/**
 * Created by Crow on 3/11/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_TIMER_H
#define LEVELNET_NET_TIMER_H

#include <atomic>
#include <functional>
#include "utility/time.h"

namespace levelnet {

class Timer {
 public:
  using TimeCallback = std::function<void()>;
  Timer(const TimeCallback &callback, Time when, double time_space);

  void Tick();
  bool Restart(Time now);

  Time When() const;
  bool Repeat() const;
  int64_t Seq() const;

  static int64_t AtomicSeqCounter();

 private:
  const TimeCallback callback_;
  const time_t time_space_;
  const bool is_repeat_;
  const int64_t seq_;
  Time when_;

  static std::atomic<int64_t> atomic_seq_counter_;
};

}

#endif //LEVELNET_NET_TIMER_H
