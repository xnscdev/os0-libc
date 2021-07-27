/* tmpnam.c -- This file is part of OS/0 libc.
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

#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char __libc_tmpnam_buf[L_tmpnam];

char *
tmpnam (char *str)
{
  if (str == NULL)
    str = __libc_tmpnam_buf;
  return tmpnam_r (str);
}

char *
tmpnam_r (char *str)
{
  unsigned int seed;
  char name[7] = {0};
  int i;
  if (str == NULL)
    return NULL;
  seed = time (NULL);
  while (1)
    {
      for (i = 0; i < 6; i++)
	name[i] = 'A' + rand_r (&seed) % 26;
      snprintf (str, L_tmpnam, "%s/file%s", P_tmpdir, name);
      if (access (str, F_OK) == -1 && errno == ENOENT)
	break;
    }
  return str;
}

FILE *
tmpfile (void)
{
  int fd = mkstemp (P_tmpdir "/fileXXXXXX");
  if (fd == -1)
    return NULL;
  return fdopen (fd, "w+");
}
