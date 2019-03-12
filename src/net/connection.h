/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_CONNECTION_H
#define LEVELNET_NET_CONNECTION_H

#include <memory>
#include <functional>

#include "net/ip_address.h"
#include "utility/buffer.h"
#include "utility/write_queue.h"

namespace levelnet {

class Socket;
class Parser;
class Channel;
class IPAddress;
class EventLoop;
class Connection {
 public:
  using EventCallback = std::function<void()>;
  using CloseCallback = std::function<void(int)>;
  using MessageCallback = std::function<long (Connection *, Buffer &, std::unique_ptr<Parser> &)>;

  Connection(EventLoop *loop, int fd, const IPAddress &address);

  bool ConnectionEstablished();
  bool ConnectionDestoryed();
  bool ShutdownConnection();
  bool ForceCloseConnection();
  void SendData(const unsigned char *data, size_t total);

  void SetConnectionCallback(const EventCallback &cb);
  void SetWriteCallback(const EventCallback &cb);
  void SetMessageCallback(const MessageCallback &cb);
  void SetCloseCallback(const CloseCallback &cb);

 private:
  void ErrorCallback();

  EventLoop *loop_;
  IPAddress address_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Parser> parser_;

  EventCallback connection_callback_;                  // register by user provide function -> OnConnection fucntion
  MessageCallback message_callback_;                   // register by user provide function -> OnMessage Function
  EventCallback write_callback_;                       // register by Connector -> to check connection status, may be add timer
  CloseCallback close_callback_;                       // register as TCPServer::RemoveConnection
  Buffer read_buffer_;
  WriteQueue write_queue_;
};

}

#endif //LEVELNET_NET_CONNECTION_H
