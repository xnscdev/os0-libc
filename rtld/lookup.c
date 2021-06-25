/* lookup.c -- This file is part of OS/0 libc.
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

#include <branch.h>
#include <fcntl.h>
#include <rtld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *rtld_trusted_dirs[] = {
  "/usr/lib",
  "/lib",
  NULL
};

static inline int
rtld_lib_lookup (int dir, const char *name)
{
  return openat (dir, name, O_RDONLY);
}

int
rtld_cache_lookup (FILE *cache, const char *name)
{
  return -1; /* TODO Support cache lookup */
}

int
rtld_search_lib (const char *name)
{
  int dir;
  int fd;
  char *libpath = getenv ("LD_LIBRARY_PATH");
  FILE *conf;
  int i;

  /* Search in LD_LIBRARY_PATH first */
  if (libpath != NULL)
    {
      char *save;
      char *ptr;
      libpath = strdup (libpath);
      if (unlikely (libpath == NULL))
	RTLD_NO_MEMORY (name);
      for (ptr = strtok_r (libpath, ":", &save); ptr != NULL;
	   ptr = strtok_r (NULL, ":", &save))
	{
	  dir = open (ptr, O_RDONLY);
	  if (dir != -1)
	    {
	      fd = rtld_lib_lookup (dir, name);
	      close (dir);
	      if (fd != -1)
		{
		  free (libpath);
		  return fd;
		}
	    }
	}
      free (libpath);
    }

  /* Search directories listed in /etc/ld.so.conf */
  conf = fopen ("/etc/ld.so.conf", "r");
  if (likely (conf != NULL))
    {
      char *line = NULL;
      size_t len = 0;
      while (getline (&line, &len, conf) > 0)
	{
	  /* Lines starting with # are comments */
	  if (line[strspn (line, " \f\n\r\t\v")] == '#')
	    continue;

	  dir = open (line, O_RDONLY);
	  if (dir != -1)
	    {
	      fd = rtld_lib_lookup (dir, name);
	      close (dir);
	      if (fd != -1)
		{
		  free (line);
		  fclose (conf);
		  return fd;
		}
	    }
	}
      free (line);
      fclose (conf);
    }

  /* Search trusted library directories */
  for (i = 0; rtld_trusted_dirs[i] != NULL; i++)
    {
      dir = open (rtld_trusted_dirs[i], O_RDONLY);
      if (likely (dir != -1))
	{
	  fd = rtld_lib_lookup (dir, name);
	  close (dir);
	  if (fd != -1)
	    return fd;
	}
    }
  return -1;
}
