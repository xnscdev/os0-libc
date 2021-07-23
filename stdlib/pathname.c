/* pathname.c -- This file is part of OS/0 libc.
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

#include <libgen.h>
#include <string.h>

char *
basename (char *path)
{
  char *ptr = strrchr (path, '/');
  if (ptr == NULL)
    return path;
  return ptr + 1;
}

char *
dirname (char *path)
{
  char *last_slash = path != NULL ? strrchr (path, '/') : NULL;
  char *ptr;
  if (last_slash != NULL && last_slash != path && last_slash[1] == '\0')
    {
      /* Check if the slash is a trailing slash and locate the next-to-last
	 one if so */
      for (ptr = last_slash; ptr != path; ptr--)
	{
	  if (ptr[-1] != '/')
	    break;
	}
      if (ptr != path)
	last_slash = memrchr (path, '/', ptr - path);
    }

  /* If there is no directory part, return a single dot */
  if (last_slash == NULL)
    return ".";

  for (ptr = last_slash; ptr != path; ptr--)
    {
      if (ptr[-1] != '/')
	break;
    }
  if (ptr == path)
    {
      if (last_slash == path + 1)
	last_slash++;
      else
	last_slash = path + 1;
    }
  else
    last_slash = ptr;
  *last_slash = '\0';
  return path;
}
