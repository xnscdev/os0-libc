/* tcattr.c -- This file is part of OS/0 libc.
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

#include <sys/ioctl.h>
#include <errno.h>
#include <termios.h>

int
tcgetattr (int fd, struct termios *tp)
{
  return ioctl (fd, TCGETS, tp);
}

int
tcsetattr (int fd, int optacts, const struct termios *tp)
{
  unsigned long req;
  switch (optacts)
    {
    case TCSANOW:
      req = TCSETS;
      break;
    case TCSADRAIN:
      req = TCSETSW;
      break;
    case TCSAFLUSH:
      req = TCSETSF;
      break;
    default:
      errno = EINVAL;
      return -1;
    }
  return ioctl (fd, req, tp);
}
