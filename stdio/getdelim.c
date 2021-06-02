/* getdelim.c -- This file is part of OS/0 libc.
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t
getdelim (char **__restrict line, size_t *__restrict len, int delim,
	  FILE *__restrict stream)
{
  ssize_t ret;
  size_t count = 0;

  if (line == NULL || len == NULL || stream == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  if (*line == NULL || *len == 0)
    {
      char *newline;
      *len = 120;
      newline = realloc (*line, *len);
      if (newline == NULL)
	return -1;
      *line = newline;
    }

  while (1)
    {
      int i = fgetc (stream);
      if (i == EOF)
	{
	  ret = -1;
	  break;
	}
      if (count + 1 >= *len)
	{
	  size_t maxneeded = SIZE_MAX / 2 + 1;
	  size_t needed = 2 * *len + 1;
	  char *newline;
	  if (maxneeded < needed)
	    needed = maxneeded;
	  if (count + 1 >= needed)
	    {
	      errno = EOVERFLOW;
	      return -1;
	    }

	  newline = realloc (*line, needed);
	  if (newline == NULL)
	    return -1;
	  *line = newline;
	  *len = needed;
	}
      (*line)[count++] = i;
    }

  (*line)[count] = '\0';
  if (count != 0)
    ret = count;
  return ret;
}

ssize_t
getline (char **__restrict line, size_t *__restrict len,
	 FILE *__restrict stream)
{
  return getdelim (line, len, '\n', stream);
}
