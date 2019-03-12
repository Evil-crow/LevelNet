/**
 * Created by Crow on  11/25/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the Derived class IPAddress
 */

#ifndef LEVELNET_NET_IP_ADDRESS_H
#define LEVELNET_NET_IP_ADDRESS_H

#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <utility>

namespace levelnet {

class IPAddress {
 public:
  IPAddress() : family_(AF_INET) {}
  ~IPAddress() = default;

  explicit IPAddress(in_port_t port);
  explicit IPAddress(sockaddr_in sockaddr);
  IPAddress(const IPAddress &address) noexcept;
  IPAddress &operator=(const IPAddress &address) noexcept;

  IPAddress(in_port_t port, std::string ip)
      : family_(AF_INET),
        port_(port),
        ip_(std::move(ip))
  {
     addr_ = ToSockaddrIn();
  }

  void SetPort(in_port_t port);
  void SetIP(const std::string &str);
  const in_port_t &Port() const;
  const std::string &IP() const;
  const sockaddr *SockaddrPtr() const ;
  const socklen_t SockaddrLen() const ;

 private:
  sockaddr_in ToSockaddrIn() const;

  sa_family_t family_;
  in_port_t port_{};
  std::string ip_;
  sockaddr_in addr_{};
};

}

#endif //LEVELNET_NET_IP_ADDRESS_H
