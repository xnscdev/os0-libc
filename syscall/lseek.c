/* lseek.c -- This file is part of OS/0 libc.
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

off_t
lseek (int fd, off_t offset, int whence)
{
  return syscall (SYS_lseek, fd, offset, whence);
}

off64_t
lseek64 (int fd, off64_t offset, int whence)
{
  off64_t result;
  if (syscall (SYS__llseek, fd, (unsigned long) (offset >> 32) & 0xffffffff,
	       (unsigned long) offset & 0xffffffff, &result, whence) == -1)
    return -1;
  return result;
}
