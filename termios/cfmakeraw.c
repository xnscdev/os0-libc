/* cfmakeraw.c -- This file is part of OS/0 libc.
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

#include <termios.h>

void
cfmakeraw (struct termios *tp)
{
  tp->c_iflag &=
    ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  tp->c_oflag &= ~OPOST;
  tp->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tp->c_cflag &= ~(CSIZE | PARENB);
  tp->c_cflag |= CS8;
  tp->c_cc[VMIN] = 1;
  tp->c_cc[VTIME] = 0;
}
