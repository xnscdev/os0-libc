/* realpath.c -- This file is part of OS/0 libc.
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
#include <branch.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int
__resolve_path (char *path, char *resolved, char *pos)
{
  if (*path == '/')
    {
      *resolved = '/';
      pos = resolved + 1;
      path++;
    }
  *pos = '\0';
  if (*path == '\0')
    return 0;
  while (1)
    {
      char *sep;
      struct stat st;
      sep = strchr (path, '/');
      if (sep != NULL)
	*sep = '\0';
      if (path[0] == '\0'
	  || (path[0] == '.' && (path[1] == '\0'
				 || (path[1] == '.' && path[2] == '\0'))))
	{
	  pos--;
	  if (pos != resolved && path[0] != '\0' && path[1] != '\0')
	    {
	      while (*--pos != '/')
		;
	    }
	}
      else
	{
	  strcpy (pos, path);
	  if (lstat (resolved, &st) == -1)
	    return -1;
	  if (S_ISLNK (st.st_mode))
	    {
	      char buffer[PATH_MAX];
	      if (readlink (resolved, buffer, PATH_MAX) == -1)
		return -1;
	      *pos = '\0';
	      if (sep != NULL)
		{
		  *sep = '/';
		  strcat (buffer, sep);
		}
	      strcpy (path, buffer);
	      if (*path == '/')
		resolved[1] = '\0';
	      pos = strchr (resolved, '\0');
	      continue;
	    }
	  pos = strchr (resolved, '\0');
	}
      if (sep != NULL)
	{
	  *pos++ = '/';
	  path = sep + 1;
	}
      *pos = '\0';
      if (sep == NULL)
	break;
    }
  return 0;
}

char *
realpath (const char *__restrict path, char *__restrict resolved)
{
  char *cwd;
  char *buffer;
  int ret;
  if (unlikely (*path == '\0'))
    {
      errno = ENOENT;
      return NULL;
    }
  cwd = getwd (NULL);
  if (unlikely (cwd == NULL))
    return NULL;
  if (resolved == NULL)
    {
      resolved = malloc (PATH_MAX);
      if (unlikely (resolved == NULL))
	{
	  errno = ENOMEM;
	  return NULL;
	}
    }
  strcpy (resolved, "/");
  if (__resolve_path (cwd, resolved, resolved) != 0)
    return NULL;
  strcat (resolved, "/");
  buffer = strdup (path);
  if (unlikely (buffer == NULL))
    {
      errno = ENOMEM;
      return NULL;
    }
  ret = __resolve_path (buffer, resolved, strchr (resolved, '\0'));
  free (buffer);
  if (ret != 0)
    return NULL;
  return resolved;
}
