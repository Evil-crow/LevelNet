/**
 * Created by Crow on 11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Derived Class IPAddress,
 *       inherite from Class Address
 */

#include "net/ip_address.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utility/logger/logger.h"

namespace levelnet {

IPAddress::IPAddress(in_port_t port)
    : family_(AF_INET),
      port_(port) {
  in_addr temp{INADDR_ANY};
  auto str = ::inet_ntoa(temp);
  ip_ = std::string(str);
  addr_ = ToSockaddrIn();
}

IPAddress::IPAddress(sockaddr_in sockaddr)
    : family_(sockaddr.sin_family),
      port_(::ntohs(sockaddr.sin_port)) {
  in_addr temp{sockaddr.sin_addr};
  auto str = ::inet_ntoa(temp);
  ip_ = std::string(str);
  addr_ = ToSockaddrIn();
}

IPAddress::IPAddress(const IPAddress &address) noexcept
    : family_(address.family_) {
  port_ = address.port_;
  ip_ = address.ip_;
  addr_ = ToSockaddrIn();
}

IPAddress &IPAddress::operator=(const IPAddress &address) noexcept = default;

const in_port_t &IPAddress::Port() const {
  return port_;
}

const std::string &IPAddress::IP() const {
  return ip_;
}

void IPAddress::SetPort(in_port_t port) {
  port_ = port;
}

void IPAddress::SetIP(const std::string &str) {
  ip_ = str;
}

sockaddr_in IPAddress::ToSockaddrIn() const {
  sockaddr_in addr{};

  addr.sin_family = family_;
  addr.sin_port = ::htons(port_);
  const char *str = ip_.c_str();
  if (::inet_pton(family_, str, &addr.sin_addr) < 1) {
    LOG(ERROR) << "IPAddress::ToSockaddrIn() => Convert IP Error";
    std::abort();
  }

  return addr;
}

const sockaddr *IPAddress::SockaddrPtr() const {
  return reinterpret_cast<const sockaddr *>(&addr_);
}

const socklen_t IPAddress::SockaddrLen() const {
  return sizeof(addr_);
}

}
