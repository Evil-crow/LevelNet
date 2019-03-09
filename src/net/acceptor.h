/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_ACCEPTOR_H
#define LEVELNET_NET_ACCEPTOR_H

#include <memory>
#include <functional>

#include "net/ip_address.h"

namespace levelnet {

class Socket;
class Channel;
class IPAddress;
class EventLoop;
class Acceptor {
 public:
  using ConnectionCallback = std::function<void(int, const IPAddress &)>;
  Acceptor(EventLoop *loop, const IPAddress &address);
  ~Acceptor() = default;

  bool Listening();
  void SetConnectionCallback(ConnectionCallback cb);

 private:
  bool IsListening();

  EventLoop *loop_;
  IPAddress address_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  bool is_listening_;
  ConnectionCallback callback_;
};

}

#endif //LEVELNET_NET_ACCEPTOR_H
