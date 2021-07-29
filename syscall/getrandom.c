/* getrandom.c -- This file is part of OS/0 libc.
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

#include <sys/random.h>
#include <sys/syscall.h>
#include <errno.h>
#include <unistd.h>

ssize_t
getrandom (void *buffer, size_t len, unsigned int flags)
{
  return syscall (SYS_getrandom, buffer, len, flags);
}

int
getentropy (void *buffer, size_t len)
{
  if (len > 256)
    {
      errno = EIO;
      return -1;
    }
  return getrandom (buffer, len, 0) == len ? 0 : -1;
}
