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

static char strtok_save;
static char *strtok_ptr;

char *
strtok (char *__restrict s, const char *__restrict delims)
{
  size_t i;
  char *ptr = NULL;
  char *end;
  if (s != NULL)
    strtok_ptr = s;
  else if (strtok_ptr == NULL)
    return NULL; /* No string initialized */
  else
    s = strtok_ptr;
  if (strtok_save != '\0')
    *strtok_ptr = strtok_save; /* Restore state from previous call */

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
	  strtok_save = *end;
	  strtok_ptr = end;
	  *end = '\0';
	  return ptr;
	}
    }
  strtok_save = '\0';
  strtok_ptr = end;
  return ptr;
}
