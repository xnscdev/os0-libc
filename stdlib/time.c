/* time.c -- This file is part of OS/0 libc.
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

#include <stdio.h>
#include <time.h>

static char *__day_names[] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

static char *__month_names[] = {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

static char __time_buf[26];

char *
asctime (const struct tm *tp)
{
  return asctime_r (tp, __time_buf);
}

char *
asctime_r (const struct tm *__restrict tp, char *__restrict buffer)
{
  sprintf (buffer, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
	   __day_names[tp->tm_wday], __month_names[tp->tm_mon], tp->tm_mday,
	   tp->tm_hour, tp->tm_min, tp->tm_sec, tp->tm_year + 1900);
  return buffer;
}
