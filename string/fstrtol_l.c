/* fstrtol_l.c -- This file is part of OS/0 libc.
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
#include <ctype.h>
#include <libc-locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <string.h>
#include <strtol.h>
#include <wctype.h>

#define BUFFER_STEP 16

static int
matches_sep (FILE *stream, char *sepseq, size_t seplen)
{
  char buffer[seplen + 1];
  if (seplen == 0)
    return 0;
  fgets (buffer, seplen + 1, stream);
  if (strncmp (buffer, sepseq, seplen) == 0)
    return 1;
  else
    {
      int i;
      for (i = seplen - 1; i >= 0; i--)
	__ungetc_unlocked (buffer[i], stream);
      return 0;
    }
}

long long
__fstrtox_l (FILE *__restrict stream, int *good, int base, int group,
	     long long min, long long max, locale_t loc)
{
  unsigned char c;
  size_t size = BUFFER_STEP;
  char *buffer = malloc (size);
  char *ptr = buffer;
  char *sepseq = loc->__lconv.thousands_sep;
  size_t seplen = strlen (sepseq);
  long long ret;
  char *end;

  if (unlikely (buffer == NULL))
    {
      *good = 0;
      return 0;
    }

  /* Ignore leading whitespace */
  do
    c = fgetc_unlocked (stream);
  while (isspace_l (c, loc));

  if (c == '+' || c == '-')
    {
      *ptr++ = c;
      c = fgetc_unlocked (stream);
    }
  while (1)
    {
      if (likely (isdigit_l (c, loc)))
	{
	  if (ptr >= buffer + size)
	    {
	      char *temp;
	      size += BUFFER_STEP;
	      temp = realloc (buffer, size);
	      if (unlikely (temp == NULL))
		goto err;
	      buffer = temp;
	    }
	  *ptr++ = c;
	}
      else if (matches_sep (stream, sepseq, seplen))
	{
	  if (ptr + seplen >= buffer + size)
	    {
	      char *temp;
	      size = (((size_t) (ptr - buffer) + seplen - 1)
		      | (BUFFER_STEP - 1)) + 1;
	      temp = realloc (buffer, size);
	      if (unlikely (temp == NULL))
		goto err;
	      buffer = temp;
	    }
	  strncpy (ptr, sepseq, seplen);
	  ptr += seplen;
	}
      else
	break;
      c = fgetc_unlocked (stream);
    }
  __ungetc_unlocked (c, stream);

  *ptr = '\0';
  ret = __strtox_l (buffer, &end, base, group, min, max, loc);
  *good = *end == '\0';
  free (buffer);
  return ret;

 err:
  free (buffer);
  *good = 0;
  return 0;
}

unsigned long long
__fstrtoux_l (FILE *__restrict stream, int *good, int base, int group,
	      unsigned long long max, locale_t loc)
{
  unsigned char c;
  size_t size = BUFFER_STEP;
  char *buffer = malloc (size);
  char *ptr = buffer;
  char *sepseq = loc->__lconv.thousands_sep;
  size_t seplen = strlen (sepseq);
  unsigned long long ret;
  char *end;

  if (unlikely (buffer == NULL))
    {
      *good = 0;
      return 0;
    }

  /* Ignore leading whitespace */
  do
    c = fgetc_unlocked (stream);
  while (isspace_l (c, loc));

  if (c == '+' || c == '-')
    {
      *ptr++ = c;
      c = fgetc_unlocked (stream);
    }
  while (1)
    {
      if (likely (isdigit_l (c, loc)))
	{
	  if (ptr >= buffer + size)
	    {
	      char *temp;
	      size += BUFFER_STEP;
	      temp = realloc (buffer, size);
	      if (unlikely (temp == NULL))
		goto err;
	      buffer = temp;
	    }
	  *ptr++ = c;
	}
      else if (matches_sep (stream, sepseq, seplen))
	{
	  if (ptr + seplen >= buffer + size)
	    {
	      char *temp;
	      size = (((size_t) (ptr - buffer) + seplen - 1)
		      | (BUFFER_STEP - 1)) + 1;
	      temp = realloc (buffer, size);
	      if (unlikely (temp == NULL))
		goto err;
	      buffer = temp;
	    }
	  strncpy (ptr, sepseq, seplen);
	  ptr += seplen;
	}
      else
	break;
      c = fgetc_unlocked (stream);
    }
  __ungetc_unlocked (c, stream);

  *ptr = '\0';
  ret = __strtoux_l (buffer, &end, base, group, max, loc);
  *good = *end == '\0';
  free (buffer);
  return ret;

 err:
  free (buffer);
  *good = 0;
  return 0;
}
