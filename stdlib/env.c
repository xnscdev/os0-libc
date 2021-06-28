/* env.c -- This file is part of OS/0 libc.
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

#include <assert.h>
#include <errno.h>
#include <locks.h>
#include <setup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **__environ;

extern char **environ __attribute__ ((alias ("__environ")));

static __lock_t __libc_env_lock;

char *
getenv (const char *name)
{
  size_t i;
  if (name == NULL || strchr (name, '=') != NULL)
    {
      errno = EINVAL;
      return NULL;
    }

  if (environ == NULL)
    return NULL;
  __libc_lock (&__libc_env_lock);
  for (i = 0; environ[i] != NULL; i++)
    {
      char *eq = strchr (environ[i], '=');
      int match;
      if (eq == NULL)
	continue; /* Invalid environment value? */
      *eq = '\0';
      match = strcmp (environ[i], name) == 0;
      *eq = '=';
      if (match)
	{
	  __libc_unlock (&__libc_env_lock);
	  return eq + 1;
	}
    }
  __libc_unlock (&__libc_env_lock);
  return NULL;
}

int
setenv (const char *name, const char *value, int overwrite)
{
  char **temp;
  size_t i;
  if (name == NULL || *name == '=' || strchr (name, '=') != NULL)
    {
      errno = EINVAL;
      return -1;
    }

  /* Search for an existing environment variable and replace it if desired */
  __libc_lock (&__libc_env_lock);
  if (environ != NULL)
    {
      for (i = 0; environ[i] != NULL; i++)
	{
	  char *eq = strchr (environ[i], '=');
	  if (eq == NULL)
	    continue; /* Invalid environment value? */
	  *eq = '\0';
	  if (strcmp (environ[i], name) == 0)
	    {
	      if (overwrite)
		{
		  char *buffer = malloc (eq - environ[i] + strlen (value) + 2);
		  if (buffer == NULL)
		    {
		      errno = ENOMEM;
		      return -1;
		    }
		  sprintf (buffer, "%s=%s", environ[i], value);
		  free (environ[i]);
		  environ[i] = buffer;
		}
	      else
		*eq = '=';
	      __libc_unlock (&__libc_env_lock);
	      return 0;
	    }
	  *eq = '=';
	}
    }

  /* Create a new entry in the environment variable array */
  temp = realloc (environ, sizeof (char *) * (i + 2));
  if (temp == NULL)
    {
      errno = ENOMEM;
      __libc_unlock (&__libc_env_lock);
      return -1;
    }
  environ = temp;
  environ[i] = malloc (strlen (name) + strlen (value) + 2);
  if (environ[i] == NULL)
    {
      errno = ENOMEM;
      __libc_unlock (&__libc_env_lock);
      return -1;
    }
  sprintf (environ[i], "%s=%s", name, value);
  environ[i + 1] = NULL;
  __libc_unlock (&__libc_env_lock);
  return 0;
}

int
putenv (char *str)
{
  char *eq;
  int ret;
  if (str == NULL)
    {
      errno = EINVAL;
      return -1;
    }
  eq = strchr (str, '=');
  if (eq == NULL)
    {
      errno = EINVAL;
      return -1;
    }
  *eq = '\0';
  ret = setenv (str, eq + 1, 1);
  *eq = '=';
  return ret;
}

int
unsetenv (const char *name)
{
  size_t len;
  char **ptr;
  if (name == NULL || strchr (name, '=') != NULL)
    {
      errno = EINVAL;
      return -1;
    }
  len = strlen (name);

  __libc_lock (&__libc_env_lock);
  ptr = environ;
  if (ptr != NULL)
    {
      while (*ptr != NULL)
	{
	  if (strncmp (*ptr, name, len) == 0 && (*ptr)[len] == '=')
	    {
	      char **temp = ptr;
	      do
		temp[0] = temp[1];
	      while (*temp++);
	    }
	  else
	    ptr++;
	}
    }
  __libc_unlock (&__libc_env_lock);
  return 0;
}

int
clearenv (void)
{
  size_t i;
  __libc_lock (&__libc_env_lock);
  for (i = 0; environ[i] != NULL; i++)
    free (environ[i]);
  free (environ);
  environ = NULL;
  __libc_unlock (&__libc_env_lock);
  return 0;
}

void
__libc_setup_env (void)
{
  char **buffer;
  size_t len;
  size_t i;
  if (environ == NULL)
    return;
  for (len = 0; environ[len] != NULL; len++)
    ;

  buffer = malloc (sizeof (char *) * (len + 1));
  for (i = 0; i < len; i++)
    {
      char *var = strdup (environ[i]);
      assert (var != NULL);
      buffer[i] = var;
    }
  buffer[len] = NULL;
  environ = buffer;
}
