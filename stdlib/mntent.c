/* mntent.c -- This file is part of OS/0 libc.
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

#include <ctype.h>
#include <errno.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MNTENT_BUFSIZE 512

static struct mntent __libc_mntent;
static char __libc_mntbuf[MNTENT_BUFSIZE];

static struct mntent *
__getmntent (struct mntent *__restrict result, FILE *__restrict stream,
	     char *__restrict buffer, size_t len)
{
  char *ptr;
  if (fgets (buffer, len, stream) == NULL)
    return NULL;
  if (buffer[strlen (buffer) - 1] != '\n')
    {
      /* Not enough space in buffer to read entire line */
      errno = ERANGE;
      return NULL;
    }
  else
    buffer[strlen (buffer) - 1] = '\0';

  result->mnt_fsname = buffer;
  ptr = strpbrk (buffer, " \t");
  if (ptr == NULL)
    return NULL;
  *ptr = '\0';

  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  result->mnt_dir = ptr;
  ptr = strpbrk (ptr, " \t");
  if (ptr == NULL)
    return NULL;
  *ptr = '\0';

  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  result->mnt_type = ptr;
  ptr = strpbrk (ptr, " \t");
  if (ptr == NULL)
    return NULL;
  *ptr = '\0';

  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  result->mnt_opts = ptr;
  ptr = strpbrk (ptr, " \t");
  if (ptr == NULL)
    return NULL;
  *ptr = '\0';

  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  result->mnt_freq = atoi (ptr);
  ptr = strpbrk (ptr, " \t");
  if (ptr == NULL)
    return NULL;
  *ptr = '\0';

  while (*ptr == ' ' || *ptr == '\t')
    ptr++;
  result->mnt_passno = atoi (ptr);
  return result;
}

FILE *
setmntent (const char *name, const char *type)
{
  return fopen (name, type);
}

struct mntent *
getmntent (FILE *stream)
{
  return getmntent_r (stream, &__libc_mntent, __libc_mntbuf, MNTENT_BUFSIZE);
}

struct mntent *
getmntent_r (FILE *__restrict stream, struct mntent *__restrict mnt,
	     char *__restrict buffer, int bufsize)
{
  return __getmntent (mnt, stream, buffer, bufsize);
}

int
addmntent (FILE *__restrict stream, const struct mntent *__restrict mnt)
{
  off64_t offset = ftello64 (stream);
  int ret = fseek (stream, 0, SEEK_END);
  if (ret != 0)
    {
      fseeko64 (stream, offset, SEEK_SET);
      return 1;
    }
  ret = fprintf (stream, "%s\t%s\t%s\t%s\t%d\t%d\n", mnt->mnt_fsname,
		 mnt->mnt_dir, mnt->mnt_type, mnt->mnt_opts, mnt->mnt_freq,
		 mnt->mnt_passno);
  fseeko64 (stream, offset, SEEK_SET);
  return ret < 0 ? 1 : 0;
}

int
endmntent (FILE *stream)
{
  return fclose (stream);
}

char *
hasmntopt (const struct mntent *mnt, const char *opt)
{
  char *ptr = mnt->mnt_opts;
  size_t len = strlen (opt);
  while (1)
    {
      if (strncmp (ptr, opt, len) == 0 && !isalnum (ptr[len]))
	return ptr;
      ptr = strchr (ptr, ',');
      if (ptr == NULL)
	return NULL;
      ptr++;
    }
}
