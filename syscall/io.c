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
#include <sys/uio.h>
#include <unistd.h>

ssize_t
pread (int fd, void *buffer, size_t len, off_t offset)
{
  return pread64 (fd, buffer, len, offset);
}

ssize_t
pread64 (int fd, void *buffer, size_t len, off64_t offset)
{
  return syscall (SYS_pread64, fd, buffer, len, offset);
}

ssize_t
read (int fd, void *buffer, size_t len)
{
  return syscall (SYS_read, fd, buffer, len);
}

ssize_t
readv (int fd, const struct iovec *vec, int vlen)
{
  return syscall (SYS_readv, fd, vec, vlen);
}

ssize_t
pwrite (int fd, const void *buffer, size_t len, off_t offset)
{
  return pwrite64 (fd, buffer, len, offset);
}

ssize_t
pwrite64 (int fd, const void *buffer, size_t len, off64_t offset)
{
  return syscall (SYS_pwrite64, fd, buffer, len, offset);
}

ssize_t
write (int fd, const void *buffer, size_t len)
{
  return syscall (SYS_write, fd, buffer, len);
}

ssize_t
writev (int fd, const struct iovec *vec, int vlen)
{
  return syscall (SYS_writev, fd, vec, vlen);
}
