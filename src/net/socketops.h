/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_SOCKETOPS_H
#define LEVELNET_NET_SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>

namespace levelnet {

class IPAddress;
namespace detail {

int CreateSocket();
bool BindOrDie(int sockfd, const struct sockaddr *addr);
bool ListenOrDie(int sockfd);
int AcceptConn(int sockfd, IPAddress &address);
bool CloseFd(int sockfd);
bool ShutdownWrite(int sockfd);

}
}

#endif //LEVELNET_NET_SOCKETOPS_H
