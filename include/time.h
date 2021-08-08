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

#ifndef _TIME_H
#define _TIME_H

#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS

extern char *tzname[2];
extern long timezone;
extern int daylight;

char *asctime (const struct tm *tp);
char *asctime_r (const struct tm *__restrict tp, char *__restrict buffer);
char *ctime (const time_t *time);
char *ctime_r (const time_t *__restrict time, char *__restrict buffer);
double difftime (time_t a, time_t b);
struct tm *gmtime (const time_t *time);
struct tm *gmtime_r (const time_t *__restrict time, struct tm *__restrict tp);
struct tm *localtime (const time_t *time);
struct tm *localtime_r (const time_t *__restrict time,
			struct tm *__restrict tp);
time_t timegm (struct tm *tp);
time_t timelocal (struct tm *tp);
time_t mktime (struct tm *tp);

void tzset (void);

time_t time (time_t *t);

int nanosleep (const struct timespec *req, struct timespec *rem);
unsigned int sleep (unsigned int seconds);

size_t strftime (char *__restrict str, size_t len, const char *__restrict fmt,
		 const struct tm *__restrict tp);
char *strptime (const char *__restrict buffer, const char *__restrict fmt,
		struct tm *__restrict tp);

clock_t clock (void);
int clock_getres (clockid_t id, struct timespec *tp);
int clock_gettime (clockid_t id, struct timespec *tp);

__END_DECLS

#endif
