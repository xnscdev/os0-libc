/* utimes.c -- This file is part of OS/0 libc.
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

#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>

int
utime (const char *path, const struct utimbuf *times)
{
  return syscall (SYS_utime, path, times);
}

int
utimes (const char *path, const struct timeval times[2])
{
  return syscall (SYS_utimes, path, times);
}

int
lutimes (const char *path, const struct timeval times[2])
{
  struct timespec timespecs[2];
  if (times[0].tv_usec >= 1000000 || times[1].tv_usec >= 1000000)
    {
      errno = EINVAL;
      return -1;
    }
  timespecs[0].tv_sec = times[0].tv_sec;
  timespecs[0].tv_nsec = times[0].tv_usec;
  timespecs[1].tv_sec = times[1].tv_sec;
  timespecs[1].tv_nsec = times[1].tv_usec;
  return utimensat (AT_FDCWD, path, timespecs, AT_SYMLINK_NOFOLLOW);
}

int
futimes (int fd, const struct timeval times[2])
{
  struct timespec timespecs[2];
  if (times[0].tv_usec >= 1000000 || times[1].tv_usec >= 1000000)
    {
      errno = EINVAL;
      return -1;
    }
  timespecs[0].tv_sec = times[0].tv_sec;
  timespecs[0].tv_nsec = times[0].tv_usec;
  timespecs[1].tv_sec = times[1].tv_sec;
  timespecs[1].tv_nsec = times[1].tv_usec;
  return utimensat (fd, NULL, timespecs, 0);
}

int
futimesat (int dir, const char *path, const struct timeval times[2])
{
  return syscall (SYS_futimesat, dir, path, times);
}

int
utimensat (int fd, const char *path, const struct timespec times[2], int flags)
{
  return syscall (SYS_utimensat, fd, path, times, flags);
}

int
futimens (int fd, const struct timespec times[2])
{
  return utimensat (fd, NULL, times, 0);
}
