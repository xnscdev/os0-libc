/* open.c -- This file is part of OS/0 libc.
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

#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

int
openat (int fd, const char *path, int flags, ...)
{
  int cwd = -1;
  int ret;
  if (fd != AT_FDCWD)
    {
      cwd = open (".", O_RDONLY);
      if (cwd == -1)
	return -1;
      if (fchdir (fd) == -1)
	return -1;
    }
  if (flags & O_CREAT)
    {
      va_list args;
      mode_t mode;
      va_start (args, flags);
      mode = va_arg (args, mode_t);
      va_end (args);
      ret = open (path, flags, mode);
    }
  else
    ret = open (path, flags);
  if (cwd != -1)
    {
      if (fchdir (cwd) == -1)
	return -1;
      close (cwd);
    }
  return ret;
}

int
creat (const char *path, mode_t mode)
{
  return open (path, O_CREAT | O_TRUNC | O_WRONLY, mode);
}
