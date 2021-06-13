/* getcwd.c -- This file is part of OS/0 libc.
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

#include <sys/param.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *
getcwd (char *buffer, size_t len)
{
  if (buffer == NULL)
    {
      size_t len = syscall (SYS_getcwd, NULL, 0);
      buffer = malloc (len + 1);
      if (buffer == NULL)
	{
	  errno = ENOMEM;
	  return NULL;
	}
      if (syscall (SYS_getcwd, buffer, len) == -1)
	{
	  free (buffer);
	  return NULL;
	}
      return buffer;
    }
  else
    return syscall (SYS_getcwd, buffer, len) == -1 ? NULL : buffer;
}

char *
getwd (char *buffer)
{
  char *ptr = getcwd (buffer, MAXPATHLEN);
  if (ptr == NULL)
    strcpy (buffer, strerror (errno));
  return ptr;
}
