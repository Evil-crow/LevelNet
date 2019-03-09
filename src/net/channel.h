/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_CHANNEL_H
#define LEVELNET_NET_CHANNEL_H

#include <sys/epoll.h>
#include <functional>

namespace levelnet {

class EventLoop;

enum ChannelEvent {
  R = EPOLLIN,
  W = EPOLLOUT,
  ET = EPOLLET,
  ERR = EPOLLERR,
  HUP = EPOLLRDHUP,
};

class Channel {
 public:
  using EventCallback = std::function<void()>;

  Channel(EventLoop *loop,int fd);
  ~Channel() = default;

  void HandleEvent();
  void SetFlags(ChannelEvent event, bool flag);
  void ClearFlags();
  void SetEvents(unsigned int events);
  void SetReadCallback(EventCallback &&cb);
  void SetWriteCallback(EventCallback &&cb);
  void SetCloseCallback(EventCallback &&cb);
  void SetErrorCallback(EventCallback &&cb);

  int Fd() const;
  unsigned int Flags() const;

 private:
  EventLoop *loop_;
  int fd_;

  unsigned int flags_;
  unsigned int revents_;
  bool event_handling_;

  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback err_callback_;
  EventCallback close_callback_;
};

}

#endif //LEVELNET_NET_CHANNEL_H
