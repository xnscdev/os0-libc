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

#include <sys/cdefs.h>
#include <sys/types.h>

#define S_IFMT   0170000
#define S_IFIFO  0010000
#define S_IFCHR  0020000
#define S_IFDIR  0040000
#define S_IFBLK  0060000
#define S_IFREG  0100000
#define S_IFLNK  0120000
#define S_IFSOCK 0140000

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100
#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010
#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000

#define S_ISBLK(x) (((x) & S_IFMT) == S_IFBLK)
#define S_ISCHR(x) (((x) & S_IFMT) == S_IFCHR)
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#define S_ISFIFO(x) (((x) & S_IFMT) == S_IFIFO)
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#define S_ISLNK(x) (((x) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(x) (((x) & S_IFMT) == S_IFSOCK)

struct stat
{
  dev_t st_dev;
  ino_t st_ino;
  mode_t st_mode;
  nlink_t st_nlink;
  uid_t st_uid;
  gid_t st_gid;
  dev_t st_rdev;
  off_t st_size;
  time_t st_atime;
  time_t st_mtime;
  time_t st_ctime;
  blksize_t st_blksize;
  blkcnt_t st_blocks;
};

__BEGIN_DECLS

int fstat (int fd, struct stat *st);
int lstat (const char *__restrict path, struct stat *__restrict st);
int stat (const char *__restrict path, struct stat *__restrict st);
int fstatat (int fd, const char *__restrict path, struct stat *__restrict st,
	     int flags);

__END_DECLS

#endif
