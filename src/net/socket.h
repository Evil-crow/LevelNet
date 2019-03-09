/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_SOCKET_H
#define LEVELNET_NET_SOCKET_H

namespace levelnet {

class IPAddress;
class Socket {
 public:
  Socket();
  explicit Socket(int fd);
  Socket(const Socket &rhs);
  Socket &operator=(const Socket &rhs) = default;
  ~Socket();

  bool Bind(const IPAddress &address);
  bool Listen();
  int Accept(IPAddress &address);
  bool ShutdownWrite();

  bool SetReuseport(bool flag);
  bool SetTcpNoNegle(bool flag);
  bool SetKeepalive(bool flag);

  int Fd();

 private:
  int sock_fd_;
};

}

#endif //LEVELNET_NET_SOCKET_H
