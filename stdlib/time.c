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

#include <errno.h>
#include <stdio.h>
#include <time.h>

/* Adapted from the GNU C Library */

#define SECSPERHOUR 3600
#define SECSPERDAY  86400

#define TDIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPTHROUGH(y) (TDIV (y, 4) - TDIV (y, 100) + TDIV (y, 400))
#define LEAPYEAR(y) ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))

static const char *__day_names[] = {
  "Sun",
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat"
};

static const char *__month_names[] = {
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

static const unsigned short __month_doys[2][13] = {
  {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
  {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

static char __time_buf[26];
static struct tm __tm_buf;

char *tzname[2] = {"GMT", "GMT"};
long timezone;
int daylight;

static int
__time_calc (time_t time, long offset, struct tm *tp)
{
  time_t days = time / SECSPERDAY;
  time_t rem = time % SECSPERDAY;
  time_t year;
  const unsigned short *ip;
  int i;
  rem += offset;
  while (rem < 0)
    {
      rem += SECSPERDAY;
      days--;
    }
  while (rem >= SECSPERDAY)
    {
      rem -= SECSPERDAY;
      days++;
    }

  tp->tm_hour = rem / SECSPERHOUR;
  rem %= SECSPERHOUR;
  tp->tm_min = rem / 60;
  tp->tm_sec = rem % 60;
  tp->tm_wday = (days + 4) % 7;
  if (tp->tm_wday < 0)
    tp->tm_wday += 7;
  year = 1970;

  while (days < 0 || days >= (LEAPYEAR (year) ? 366 : 365))
    {
      time_t yg = year + days / 365 - (days % 365 < 0);
      days -= ((yg - year) * 365 + LEAPTHROUGH (yg - 1) -
	       LEAPTHROUGH (year - 1));
      year = yg;
    }
  tp->tm_year = year - 1900;
  if (tp->tm_year != year - 1900)
    {
      errno = EOVERFLOW;
      return 0;
    }
  tp->tm_yday = days;

  ip = __month_doys[LEAPYEAR (year)];
  for (i = 11; days < ip[i]; i--)
    ;
  days -= ip[i];
  tp->tm_mon = i;
  tp->tm_mday = days + 1;
  return 1;
}

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

char *
ctime (const time_t *time)
{
  return ctime_r (time, __time_buf);
}

char *
ctime_r (const time_t *__restrict time, char *__restrict buffer)
{
  struct tm tp;
  localtime_r (time, &tp);
  return asctime_r (&tp, buffer);
}

double
difftime (time_t a, time_t b)
{
  return a > b ? a - b : b - a;
}

struct tm *
gmtime (const time_t *time)
{
  return gmtime_r (time, &__tm_buf);
}

struct tm *
gmtime_r (const time_t *__restrict time, struct tm *__restrict tp)
{
  if (!__time_calc (*time, 0, tp))
    return NULL;
  tp->tm_isdst = 0;
  return tp;
}

/* For now we only support UTC local time
   TODO Support local time */

struct tm *
localtime (const time_t *time)
{
  return gmtime (time);
}

struct tm *
localtime_r (const time_t *__restrict time, struct tm *__restrict tp)
{
  return gmtime_r (time, tp);
}

void
tzset (void)
{
  tzname[0] = "GMT";
  tzname[1] = "GMT";
  timezone = 0;
  daylight = 0;
}
