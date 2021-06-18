/* dirent.h -- This file is part of OS/0 libc.
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

#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stddef.h>

#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     3
#define DT_BLK     4
#define DT_REG     5
#define DT_LNK     6
#define DT_SOCK    7

struct dirent
{
  ino_t d_ino;
  unsigned short d_reclen;
  unsigned char d_type;
  char d_name[256];
};

typedef struct
{
  int _fd;
  size_t _count;
  size_t _offset;
  size_t _block;
  char *_data;
} DIR;

__BEGIN_DECLS

int closedir (DIR *dir);
DIR *opendir (const char *path);
struct dirent *readdir (DIR *dir);
int readdir_r (DIR *__restrict dir, struct dirent *__restrict entry,
	       struct dirent **__restrict saveptr);
void rewinddir (DIR *dir);
void seekdir (DIR *dir, long pos);
long telldir (DIR *dir);

__END_DECLS

#endif
