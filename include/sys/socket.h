/* socket.h -- This file is part of OS/0 libc.
   Copyright (C) 2021 XNSC

   OS/0 libc is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   OS/0 libc is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with OS/0 libc. If not, see <https://www.gnu.org/licenses/>. */

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <sys/uio.h>

#define SOCK_DGRAM     1
#define SOCK_STREAM    2
#define SOCK_SEQPACKET 3

#define SOL_SOCKET 1

#define SO_ACCEPTCONN 1
#define SO_BROADCAST  2
#define SO_DEBUG      3
#define SO_DONTROUTE  4
#define SO_ERROR      5
#define SO_KEEPALIVE  6
#define SO_LINGER     7
#define SO_OOBINLINE  8
#define SO_RCVBUF     9
#define SO_RCVLOWAT   10
#define SO_RCVTIMEO   11
#define SO_REUSEADDR  12
#define SO_SNDBUF     13
#define SO_SNDTIMEO   14
#define SO_TYPE       15

#define MSG_CTRUNC    0x0001
#define MSG_DONTROUTE 0x0002
#define MSG_EOR       0x0004
#define MSG_OOB       0x0008
#define MSG_TRUNC     0x0010
#define MSG_WAITALL   0x0020

#define AF_UNIX   1
#define AF_UNSPEC 2
#define AF_INET   3

#define SHUT_RD   1
#define SHUT_WR   2
#define SHUT_RDWR 3

typedef unsigned int sa_family_t;
typedef unsigned long socklen_t;

struct sockaddr
{
  sa_family_t sa_family;
  char sa_data[];
};

struct msghdr
{
  void *msg_name;
  socklen_t msg_namelen;
  struct iovec *msg_iov;
  int msg_iovlen;
  void *msg_control;
  socklen_t msg_controllen;
  int msg_flags;
};

struct cmsghdr
{
  socklen_t cmsg_len;
  int cmsg_level;
  int cmsg_type;
};

struct linger
{
  int l_onoff;
  int l_linger;
};

#endif
