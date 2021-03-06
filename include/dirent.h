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

#include <bits/dirent.h>
#include <sys/cdefs.h>
#include <sys/types.h>

typedef struct
{
  int _fd;
  int _close;
} DIR;

__BEGIN_DECLS

int closedir (DIR *dir);
DIR *opendir (const char *path);
DIR *fdopendir (int fd);
struct dirent *readdir (DIR *dir);
int readdir_r (DIR *__restrict dir, struct dirent *__restrict entry,
	       struct dirent **__restrict saveptr);
void rewinddir (DIR *dir);
void seekdir (DIR *dir, long pos);
long telldir (DIR *dir);
int dirfd (DIR *dir);

int IFTODT (mode_t mode);
mode_t DTTOIF (int type);

__END_DECLS

#endif
