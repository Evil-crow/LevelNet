/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_EVENT_LOOP_H
#define LEVELNET_NET_EVENT_LOOP_H

#include <mutex>
#include <memory>
#include <vector>
#include <functional>

namespace levelnet {

class Epoll;
class Channel;
class EventLoop {
 public:
  using Functor = std::function<void()>;

  EventLoop();
  ~EventLoop();

  void Loop();
  void Quit();
  bool AddChannel(Channel *channel);
  bool UpdateChannel(Channel *channel);
  bool DeleteChannel(Channel *channel);
  void TaskInLoop(Functor cb);

 private:
  void DoPendingFunctor();
  void WakeUp();

  bool looping_;
  bool quit_;
  bool event_handling_;
  bool calling_pending_functor_;

  int event_fd_;
  std::unique_ptr<Epoll> epoller_;
  std::unique_ptr<Channel> event_channel_;
  std::vector<Channel *> active_channels_;
  std::vector<Functor> pending_functors_;
  std::mutex mutex_;
};

}

#endif //LEVELNET_NET_EVENT_LOOP_H
