/* exec.c -- This file is part of OS/0 libc.
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

#include <sys/syslimits.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARGS_MAX 256
#define PATH_SEP ';'

extern char **environ;

int
execl (const char *path, const char *arg0, ...)
{
  va_list args;
  const char *argv[256];
  int i = 1;
  va_start (args, arg0);
  argv[0] = arg0;
  while (1)
    {
      argv[i] = va_arg (args, const char *);
      if (argv[i] == NULL)
	break;
      i++;
      if (i >= ARGS_MAX)
	{
	  errno = E2BIG;
	  return -1;
	}
    }
  va_end (args);
  return execve (path, (char *const *) argv, environ);
}

int
execle (const char *path, const char *arg0, ...)
{
  va_list args;
  const char *argv[256];
  const char *const *envp;
  int i = 1;
  va_start (args, arg0);
  argv[0] = arg0;
  while (1)
    {
      argv[i] = va_arg (args, const char *);
      if (argv[i] == NULL)
	break;
      i++;
      if (i >= ARGS_MAX)
	{
	  errno = E2BIG;
	  return -1;
	}
    }
  envp = va_arg (args, const char *const *);
  va_end (args);
  return execve (path, (char *const *) argv, (char *const *) envp);
}

int
execlp (const char *file, const char *arg0, ...)
{
  va_list args;
  const char *argv[256];
  int i = 1;
  va_start (args, arg0);
  argv[0] = arg0;
  while (1)
    {
      argv[i] = va_arg (args, const char *);
      if (argv[i] == NULL)
	break;
      i++;
      if (i >= ARGS_MAX)
        {
	  errno = E2BIG;
	  return -1;
	}
    }
  va_end (args);
  return execvp (file, (char *const *) argv);
}

int
execv (const char *path, char *const *argv)
{
  return execve (path, argv, environ);
}

int
execvp (const char *file, char *const *argv)
{
  char *path = getenv ("PATH");
  char buffer[NAME_MAX];

  /* Use given path if PATH is not set or there are path components in file */
  if (path == NULL || strchr (file, '/') != NULL)
    return execv (file, argv);

  while (*path != '\0')
    {
      char *ptr = buffer;
      while (*path != '\0' && *path != PATH_SEP)
	*ptr++ = *path++;
      *ptr = '\0';
      if (*buffer != '\0' && buffer[strlen (buffer) - 1] != '/')
	strcat (buffer, "/");
      strcat (buffer, file);
      if (execv (buffer, argv) == -1 && errno != ENOENT)
	return -1; /* File exists but failed to execute */
      while (*path != '\0' && *path != PATH_SEP)
	path++;
      if (*path == PATH_SEP)
	path++;
    }
  return -1; /* Not found in PATH */
}
