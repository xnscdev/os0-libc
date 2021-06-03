/* strtok.c -- This file is part of OS/0 libc.
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

#include <string.h>

static char *__libc_strtok_saveptr;

char *
strtok (char *__restrict s, const char *__restrict delims)
{
  return strtok_r (s, delims, &__libc_strtok_saveptr);
}

char *
strtok_r (char *__restrict s, const char *__restrict delims,
	  char **__restrict saveptr)
{
  size_t i;
  char *ptr = NULL;
  char *end;

  /* Set or restore context */
  if (s != NULL)
    *saveptr = s;
  else
    s = *saveptr + 1;

  for (end = s; *end != '\0'; end++)
    {
      int is_delim = 0;
      for (i = 0; delims[i] != '\0'; i++)
	{
	  if (*end == delims[i])
	    {
	      is_delim = 1;
	      break;
	    }
	}

      if (!is_delim && ptr == NULL)
	ptr = end;
      else if (is_delim && ptr != NULL)
	{
	  *saveptr = end;
	  *end = '\0';
	  return ptr;
	}
    }
  *saveptr = end;
  return ptr;
}
