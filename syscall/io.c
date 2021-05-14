/* io.c -- This file is part of OS/0 libc.
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

#include <sys/syscall.h>
#include <unistd.h>

ssize_t
pread (int fd, void *buffer, size_t len, off_t offset)
{
  if (lseek (fd, offset, SEEK_SET) == -1)
    return -1;
  return read (fd, buffer, len);
}

ssize_t
read (int fd, void *buffer, size_t len)
{
  return syscall (SYS_read, fd, buffer, len);
}

ssize_t
pwrite (int fd, const void *buffer, size_t len, off_t offset)
{
  if (lseek (fd, offset, SEEK_SET) == -1)
    return -1;
  return write (fd, buffer, len);
}

ssize_t
write (int fd, const void *buffer, size_t len)
{
  return syscall (SYS_write, fd, buffer, len);
}
