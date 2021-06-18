/* termios.h -- This file is part of OS/0 libc.
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

#ifndef _TERMIOS_H
#define _TERMIOS_H

#include <bits/termios.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

speed_t cfgetispeed (const struct termios *tp);
speed_t cfgetospeed (const struct termios *tp);
void cfmakeraw (struct termios *tp);
int cfsetispeed (struct termios *tp, speed_t speed);
int cfsetospeed (struct termios *tp, speed_t speed);
int cfsetspeed (struct termios *tp, speed_t speed);

int tcgetattr (int fd, struct termios *tp);
int tcsetattr (int fd, int optacts, const struct termios *tp);

int tcdrain (int fd);
int tcflush (int fd, int action);

pid_t tcgetpgrp (int fd);
int tcsetpgrp (int fd, pid_t pgid);

__END_DECLS

#endif
