/* dirent.c -- This file is part of OS/0 libc.
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
#include <branch.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

static struct dirent __libc_readdir_entry;
static struct dirent *__libc_readdir_saveptr;

int
closedir (DIR *dir)
{
  if (dir->_close)
    {
      if (close (dir->_fd) == -1)
	return -1;
    }
  free (dir);
  return 0;
}

DIR *
opendir (const char *path)
{
  int fd = open (path, O_RDONLY);
  DIR *dir;
  if (fd == -1)
    return NULL;
  dir = malloc (sizeof (DIR));
  if (unlikely (dir == NULL))
    {
      close (fd);
      return NULL;
    }
  dir->_fd = fd;
  dir->_close = 1;
  return dir;
}

DIR *
fdopendir (int fd)
{
  DIR *dir = malloc (sizeof (DIR));
  if (unlikely (dir == NULL))
    return NULL;
  dir->_fd = fd;
  dir->_close = 0;
  return dir;
}

struct dirent *
readdir (DIR *dir)
{
  if (readdir_r (dir, &__libc_readdir_entry, &__libc_readdir_saveptr) == -1)
    return NULL;
  return &__libc_readdir_entry;
}

int
readdir_r (DIR *__restrict dir, struct dirent *__restrict entry,
	   struct dirent **__restrict saveptr)
{
  int ret = syscall (SYS_getdents, dir->_fd, entry, 1);
  if (ret == -1)
    {
      *saveptr = NULL;
      return -1;
    }
  *saveptr = entry;
  return 0;
}

void
rewinddir (DIR *dir)
{
  lseek (dir->_fd, 0, SEEK_SET);
}

void
seekdir (DIR *dir, long pos)
{
  lseek (dir->_fd, pos, SEEK_SET);
}

long
telldir (DIR *dir)
{
  return lseek (dir->_fd, 0, SEEK_CUR);
}
