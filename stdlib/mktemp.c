/* mktemp.c -- This file is part of OS/0 libc.
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
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *
mktemp (char *tmpl)
{
  size_t len = strlen (tmpl);
  char *ptr;
  unsigned int seed;
  int i;
  if (len < 6 || (ptr = tmpl + len - 7, strcmp (ptr, "XXXXXX") != 0))
    {
      errno = EINVAL;
      *tmpl = '\0';
      return tmpl;
    }

  /* Make sure we can actually access the supplied path */
  if (access (tmpl, F_OK) == -1 && errno != ENOENT)
    {
      *tmpl = '\0';
      return tmpl;
    }

  /* Generate random filenames until one is found */
  seed = time (NULL);
  while (1)
    {
      for (i = 0; i < 6; i++)
        ptr[i] = 'A' + rand_r (&seed) % 26;
      if (access (tmpl, F_OK) == -1 && errno == ENOENT)
	break;
    }
  return tmpl;
}

int
mkstemp (char *tmpl)
{
  size_t len = strlen (tmpl);
  char *ptr;
  unsigned int seed;
  int i;
  if (len < 6 || (ptr = tmpl + len - 7, strcmp (ptr, "XXXXXX") != 0))
    {
      errno = EINVAL;
      return -1;
    }

  /* Make sure we can actually access the supplied path */
  if (access (tmpl, F_OK) == -1 && errno != ENOENT)
    return -1;

  /* Generate random filenames until one can be opened */
  seed = time (NULL);
  while (1)
    {
      int fd;
      for (i = 0; i < 6; i++)
        ptr[i] = 'A' + rand_r (&seed) % 26;
      fd = open (tmpl, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
      if (fd != -1)
	return fd;
    }
}

char *
mkdtemp (char *tmpl)
{
  mktemp (tmpl);
  if (*tmpl == '\0')
    return NULL;
  if (mkdir (tmpl, S_IRWXU) == -1)
    return NULL;
  return tmpl;
}
