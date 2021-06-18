/* statvfs.c -- This file is part of OS/0 libc.
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

#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <sys/syscall.h>
#include <sys/sysmacros.h>
#include <unistd.h>

static void
__libc_conv_statvfs (const struct statfs *st, struct statvfs *vst)
{
  vst->f_bsize = st->f_bsize;
  vst->f_frsize = st->f_bsize;
  vst->f_blocks = st->f_blocks;
  vst->f_bfree = st->f_bfree;
  vst->f_bavail = st->f_bavail;
  vst->f_files = st->f_files;
  vst->f_ffree = st->f_ffree;
  vst->f_favail = st->f_ffree;
  vst->f_fsid = makedev (st->f_fsid.f_val[0], st->f_fsid.f_val[1]);
  vst->f_flag = st->f_flags;
  vst->f_namemax = st->f_namelen;
}

int
statvfs (const char *path, struct statvfs *st)
{
  struct statfs _st;
  if (statfs (path, &_st) == -1)
    return -1;
  __libc_conv_statvfs (&_st, st);
  return 0;
}

int
fstatvfs (int fd, struct statvfs *st)
{
  struct statfs _st;
  if (fstatfs (fd, &_st) == -1)
    return -1;
  __libc_conv_statvfs (&_st, st);
  return 0;
}
