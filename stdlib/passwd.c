/* passwd.c -- This file is part of OS/0 libc.
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
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWD_FILE   "/etc/passwd"
#define PASSWD_BUFSIZ 512

static struct passwd __libc_passwd;
static char __libc_pwbuf[PASSWD_BUFSIZ];

struct passwd *
getpwnam (const char *name)
{
  struct passwd *ignore;
  if (getpwnam_r (name, &__libc_passwd, __libc_pwbuf, PASSWD_BUFSIZ, &ignore)
      == -1)
    return NULL;
  return &__libc_passwd;
}

struct passwd *
getpwuid (uid_t uid)
{
  struct passwd *ignore;
  if (getpwuid_r (uid, &__libc_passwd, __libc_pwbuf, PASSWD_BUFSIZ, &ignore)
      == -1)
    return NULL;
  return &__libc_passwd;
}

int
getpwnam_r (const char *__restrict name, struct passwd *__restrict pwd,
	    char *__restrict buffer, size_t len,
	    struct passwd **__restrict result)
{
  FILE *file = fopen (PASSWD_FILE, "r");
  if (file == NULL)
    {
      errno = ENOENT;
      *result = NULL;
      return -1;
    }
  while (fgets (buffer, len, file) != NULL)
    {
      char *ptr;
      if (buffer[strlen (buffer) - 1] != '\n')
	{
	  /* Not enough space in buffer to read entire line */
	  errno = ERANGE;
	  fclose (file);
	  *result = NULL;
	  return -1;
	}

      pwd->pw_name = buffer;
      ptr = strchr (buffer, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';
      if (strcmp (buffer, name) != 0)
	continue;

      pwd->pw_passwd = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_uid = atoi (ptr + 1);
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_gid = atoi (ptr + 1);
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_gecos = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_dir = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';
      pwd->pw_shell = ptr + 1;

      fclose (file);
      *result = pwd;
      return 0;
    }
  if (ferror (file))
    {
      fclose (file);
      *result = NULL;
      return -1;
    }
  fclose (file);
  return 0;
}

int
getpwuid_r (uid_t uid, struct passwd *__restrict pwd, char *__restrict buffer,
	    size_t len, struct passwd **__restrict result)
{
  FILE *file = fopen (PASSWD_FILE, "r");
  if (file == NULL)
    {
      errno = ENOENT;
      *result = NULL;
      return -1;
    }
  while (fgets (buffer, len, file) != NULL)
    {
      char *ptr;
      if (buffer[strlen (buffer) - 1] != '\n')
	{
	  /* Not enough space in buffer to read entire line */
	  errno = ERANGE;
	  fclose (file);
	  *result = NULL;
	  return -1;
	}

      pwd->pw_name = buffer;
      ptr = strchr (buffer, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_passwd = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_uid = atoi (ptr + 1);
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';
      if (pwd->pw_uid != uid)
	continue;

      pwd->pw_gid = atoi (ptr + 1);
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_gecos = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';

      pwd->pw_dir = ptr + 1;
      ptr = strchr (ptr + 1, ':');
      if (ptr == NULL)
	continue;
      *ptr = '\0';
      pwd->pw_shell = ptr + 1;

      fclose (file);
      *result = pwd;
      return 0;
    }
  if (ferror (file))
    {
      fclose (file);
      *result = NULL;
      return -1;
    }
  fclose (file);
  return 0;
}
