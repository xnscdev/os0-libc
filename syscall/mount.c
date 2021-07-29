/* mount.c -- This file is part of OS/0 libc.
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

#include <sys/mount.h>
#include <sys/syscall.h>
#include <unistd.h>

int
mount (const char *src, const char *dir, const char *type, unsigned long flags,
       const void *data)
{
  return syscall (SYS_mount, src, dir, type, flags, data);
}

int
umount (const char *dir)
{
  return syscall (SYS_umount, dir);
}
