/* time.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <sys/types.h>

struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

struct timeval
{
  time_t tv_sec;
  suseconds_t tv_usec;
};

struct itimerval
{
  struct timeval it_interval;
  struct timeval it_value;
};

struct timespec
{
  time_t tv_sec;
  long tv_nsec;
};

struct itimerspec
{
  struct timespec it_interval;
  struct timespec it_value;
};

#endif
