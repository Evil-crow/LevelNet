/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_EPOLL_H
#define LEVELNET_NET_EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <memory>

namespace levelnet {

class Channel;
class EventLoop;
class Epoll {
 public:
  explicit Epoll(EventLoop *loop) noexcept ;
  ~Epoll();

  int Poll(std::vector<Channel *> &active_channels, int timeout);
  bool Add(Channel *channel);
  bool Update(Channel *channel);
  bool Delete(Channel *channel);

 private:
  int FillActiveChannels(int event_num, std::vector<Channel *> &active_channels);

  int ep_fd_;
  EventLoop *loop_;
  std::vector<epoll_event> active_events_;
  const constexpr unsigned long KMaxEventSize = 1024;
};

}

#endif //LEVELNET_NET_EPOLL_H
