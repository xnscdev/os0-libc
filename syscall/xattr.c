/* xattr.c -- This file is part of OS/0 libc.
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
#include <sys/xattr.h>
#include <unistd.h>

int
setxattr (const char *path, const char *name, const void *value, size_t len,
	  int flags)
{
  return syscall (SYS_setxattr, path, name, value, len, flags);
}

int
lsetxattr (const char *path, const char *name, const void *value, size_t len,
	   int flags)
{
  return syscall (SYS_lsetxattr, path, name, value, len, flags);
}

int
fsetxattr (int fd, const char *name, const void *value, size_t len, int flags)
{
  return syscall (SYS_fsetxattr, fd, name, value, len, flags);
}

ssize_t
getxattr (const char *path, const char *name, void *value, size_t len)
{
  return syscall (SYS_getxattr, path, name, value, len);
}

ssize_t
lgetxattr (const char *path, const char *name, void *value, size_t len)
{
  return syscall (SYS_lgetxattr, path, name, value, len);
}

ssize_t
fgetxattr (int fd, const char *name, void *value, size_t len)
{
  return syscall (SYS_fgetxattr, fd, name, value, len);
}

ssize_t
listxattr (const char *path, char *buffer, size_t len)
{
  return syscall (SYS_listxattr, path, buffer, len);
}

ssize_t
llistxattr (const char *path, char *buffer, size_t len)
{
  return syscall (SYS_llistxattr, path, buffer, len);
}

ssize_t
flistxattr (int fd, char *buffer, size_t len)
{
  return syscall (SYS_flistxattr, fd, buffer, len);
}

int
removexattr (const char *path, const char *name)
{
  return syscall (SYS_removexattr, path, name);
}

int
lremovexattr (const char *path, const char *name)
{
  return syscall (SYS_lremovexattr, path, name);
}

int
fremovexattr (int fd, const char *name)
{
  return syscall (SYS_fremovexattr, fd, name);
}
