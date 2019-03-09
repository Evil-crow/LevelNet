/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/socket.h"

#include <unistd.h>
#include <netinet/tcp.h>

#include "net/socketops.h"
#include "net/ip_address.h"
#include "utility/logger/logger.h"

namespace levelnet {

Socket::Socket()
    : sock_fd_(detail::CreateSocket()) { ; }

Socket::Socket(int fd)
    : sock_fd_(fd) { ; }

Socket::Socket(const Socket &rhs)
    : sock_fd_(rhs.sock_fd_) { ; }

Socket::~Socket() {
  if (::close(sock_fd_)) {
    LOG(ERROR) << "Socket::~Socket() => Close Fd Error";
    std::abort();
  }
}

bool Socket::Bind(const IPAddress &address) {
  return detail::BindOrDie(sock_fd_, address.SockaddrPtr());
}

bool Socket::Listen() {
  return detail::ListenOrDie(sock_fd_);
}

int Socket::Accept(IPAddress &address) {
  return detail::AcceptConn(sock_fd_, address);
}

bool Socket::ShutdownWrite() {
  return detail::ShutdownWrite(sock_fd_);
}

bool Socket::SetReuseport(bool flag) {
  int val = flag ? 1 : 0;
  if (::setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(int))) {
    LOG(ERROR) << "Socket::SetReuseport() => Setsockopt Error";
    std::abort();
  }
  return true;
}

bool Socket::SetKeepalive(bool flag) {
  int val = flag ? 1 : 0;
  if (::setsockopt(sock_fd_, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(int))) {
    LOG(ERROR) << "Socket::SetKeepalive() => Setsockopt Error";
    std::abort();
  }
  return true;
}

bool Socket::SetTcpNoNegle(bool flag) {
  int val = flag ? 1 : 0;
  if (::setsockopt(sock_fd_, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val))) {
    LOG(ERROR) << "Socket::SetTcpNegle() => Setsockopt Error";
    std::abort();
  }
  return true;
}

int Socket::Fd() {
  return sock_fd_;
}

}
