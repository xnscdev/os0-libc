/* stat.c -- This file is part of OS/0 libc.
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
#include <unistd.h>

int
stat (const char *__restrict path, struct stat *__restrict st)
{
  return syscall (SYS_stat, path, st);
}

int
lstat (const char *__restrict path, struct stat *__restrict st)
{
  return syscall (SYS_lstat, path, st);
}

int
fstat (int fd, struct stat *st)
{
  return syscall (SYS_fstat, fd, st);
}

int
stat64 (const char *__restrict path, struct stat64 *__restrict st)
{
  return syscall (SYS_stat64, path, st);
}

int
lstat64 (const char *__restrict path, struct stat64 *__restrict st)
{
  return syscall (SYS_lstat64, path, st);
}

int
fstat64 (int fd, struct stat64 *st)
{
  return syscall (SYS_fstat64, fd, st);
}
