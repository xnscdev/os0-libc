/* chown.c -- This file is part of OS/0 libc.
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

int
chown (const char *path, uid_t uid, gid_t gid)
{
  return syscall (SYS_chown, path, uid, gid);
}

int
fchown (int fd, uid_t uid, gid_t gid)
{
  return syscall (SYS_fchown, fd, uid, gid);
}

int
lchown (const char *path, uid_t uid, gid_t gid)
{
  /* TODO Implement lchown(2) */
  return chown (path, uid, gid);
}

int
fchownat (int fd, const char *path, uid_t uid, gid_t gid, int flags)
{
  return syscall (SYS_fchownat, fd, path, uid, gid, flags);
}
