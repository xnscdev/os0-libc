/* err.c -- This file is part of OS/0 libc.
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

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
vwarn (const char *fmt, va_list args)
{
  int save_errno = errno;
  if (fmt != NULL)
    {
      fprintf (stderr, "%s: ", program_invocation_short_name);
      vfprintf (stderr, fmt, args);
      fprintf (stderr, ": %s\n", strerror (save_errno));
    }
  else
    fprintf (stderr, "%s: %s\n", program_invocation_short_name,
	     strerror (save_errno));
  errno = save_errno;
}

void
vwarnx (const char *fmt, va_list args)
{
  fprintf (stderr, "%s: ", program_invocation_short_name);
  if (fmt != NULL)
    vfprintf (stderr, fmt, args);
  fputc ('\n', stderr);
}

void
verr (int status, const char *fmt, va_list args)
{
  vwarn (fmt, args);
  exit (status);
}

void
verrx (int status, const char *fmt, va_list args)
{
  vwarnx (fmt, args);
  exit (status);
}

void
warn (const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  vwarn (fmt, args);
  va_end (args);
}

void
warnx (const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  vwarnx (fmt, args);
  va_end (args);
}

void
err (int status, const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  verr (status, fmt, args);
}

void
errx (int status, const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  verrx (status, fmt, args);
}
