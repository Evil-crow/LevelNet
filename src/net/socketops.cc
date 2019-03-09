/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "net/socketops.h"

#include <unistd.h>

#include "net/ip_address.h"
#include "utility/logger/logger.h"

namespace levelnet {
namespace detail {

constexpr int KBacklog = SOMAXCONN;

int CreateSocket() {
  int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if (fd < 0) {
    LOG(ERROR) << "detail::CreateSocket() => Create Socket Error";
    std::abort();
  }
  return fd;
}

bool BindOrDie(int sockfd, const struct sockaddr *addr) {
  if (::bind(sockfd, addr, sizeof(sockaddr))) {
    LOG(ERROR) << "detail::BindOrDie() => Bind Error";
    std::abort();
  }
  return true;
}

bool ListenOrDie(int sockfd) {
  if (::listen(sockfd, KBacklog)) {
    LOG(ERROR) << "detail::ListenOrDie() => Listen Error";
    std::abort();
  }
  return true;
}

int AcceptConn(int sockfd, IPAddress &address) {
  sockaddr_in addr{};
  socklen_t len = sizeof(sockaddr);

  int connfd = ::accept4(sockfd,
      reinterpret_cast<sockaddr *>(&addr),
      &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (connfd < 0) {
    if (errno == EAGAIN) {
      return -1;
    } else {
      LOG(ERROR) << "detail::AcceptConn() => Accept Error";
      std::abort();
    }
  } else {
    address.SetPort(::ntohs(addr.sin_port));
    address.SetIP(::inet_ntoa(addr.sin_addr));
    return connfd;
  }
}

bool CloseFd(int sockfd) {
  if (::close(sockfd)) {
    LOG(ERROR) << "detail::CloseFd() => Close Fd Error";
    std::abort();
  }
  return true;
}

bool ShutdownWrite(int sockfd) {
  if (::shutdown(sockfd, SHUT_WR)) {
    LOG(ERROR) << "detail::Shutdown() => SHUT_WR Error";
    std::abort();
  }
  return true;
}

}
}