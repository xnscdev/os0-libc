/* stat.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <bits/stat.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int stat (const char *__restrict path, struct stat *__restrict st);
int lstat (const char *__restrict path, struct stat *__restrict st);
int fstat (int fd, struct stat *st);
int fstatat (int fd, const char *__restrict path, struct stat *__restrict st,
	     int flags);
int stat64 (const char *__restrict path, struct stat64 *__restrict st);
int lstat64 (const char *__restrict path, struct stat64 *__restrict st);
int fstat64 (int fd, struct stat64 *st);
int fstatat64 (int fd, const char *__restrict path,
	       struct stat64 *__restrict st, int flags);

mode_t umask (mode_t mask);
int chmod (const char *path, mode_t mode);
int fchmod (int fd, mode_t mode);
int fchmodat (int fd, const char *path, mode_t mode, int flags);

int mkdir (const char *path, mode_t mode);
int mkdirat (int fd, const char *path, mode_t mode);
int mknod (const char *path, mode_t mode, dev_t dev);
int mknodat (int fd, const char *path, mode_t mode, dev_t dev);
int mkfifo (const char *path, mode_t mode);

int utimensat (int fd, const char *path, const struct timespec times[2],
	       int flags);
int futimens (int fd, const struct timespec times[2]);

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define stat    stat64
#define lstat   lstat64
#define fstat   fstat64
#define fstatat fstatat64
#endif

__END_DECLS

#endif
