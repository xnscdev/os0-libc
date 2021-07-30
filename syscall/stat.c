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
#include <string.h>
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
fstatat (int fd, const char *__restrict path, struct stat *__restrict st,
	 int flags)
{
  struct stat64 st64;
  if (fstatat64 (fd, path, &st64, flags) == -1)
    return -1;
  st->st_dev = st64.st_dev;
  st->st_ino = st64.st_ino;
  st->st_mode = st64.st_mode;
  st->st_nlink = st64.st_nlink;
  st->st_uid = st64.st_uid;
  st->st_gid = st64.st_gid;
  st->st_rdev = st64.st_rdev;
  st->st_size = st64.st_size;
  memcpy (&st->st_atim, &st64.st_atim, sizeof (struct timespec));
  memcpy (&st->st_mtim, &st64.st_mtim, sizeof (struct timespec));
  memcpy (&st->st_ctim, &st64.st_ctim, sizeof (struct timespec));
  st->st_blksize = st64.st_blksize;
  st->st_blocks = st64.st_blocks;
  return 0;
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

int
fstatat64 (int fd, const char *__restrict path, struct stat64 *__restrict st,
	   int flags)
{
  return syscall (SYS_fstatat64, fd, path, st, flags);
}
