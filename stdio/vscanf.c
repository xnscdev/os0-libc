/* vscanf.c -- This file is part of OS/0 libc.
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
#include <errno.h>
#include <libc-locale.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <strtol.h>

/* *scanf flags */

#define NOASSIGN 0x01
#define MALLOC   0x02

#define POSARG   0x1000
#define ASSIGNS  0x2000
#define ASSIGNU  0x4000

/* Length modifiers */

enum
{
  LEN_INT = 0,
  LEN_CHAR,
  LEN_SHORT,
  LEN_LONG,
  LEN_LLONG,
  LEN_INTMAX,
  LEN_SIZE,
  LEN_PTRDIFF,
  LEN_LDOUBLE
};

struct arguments
{
  void **args;
  size_t len;
  size_t curr;
};

#define DO_OR_RETURN(expr) do			\
    {						\
      if ((expr) != 0)				\
        goto end;				\
    }						\
  while (0)

static int
pull_leading_whitespace (FILE *stream, locale_t loc)
{
  char c;
  do
    {
      c = fgetc_unlocked (stream);
      if (c == EOF)
	return EOF;
    }
  while (isspace_l (c, loc));
  return __ungetc_unlocked (c, stream) == EOF ? EOF : 0;
}

static const char *
parse_lenmod_chars (const char *fmt, int *lenmod)
{
 lencheck:
  switch (*fmt)
    {
    case 'h':
      if (*lenmod == LEN_SHORT)
	*lenmod = LEN_CHAR;
      else if (*lenmod == LEN_INT)
	*lenmod = LEN_SHORT;
      else
	return NULL;
      fmt++;
      goto lencheck;
    case 'l':
      if (*lenmod == LEN_LONG)
	*lenmod = LEN_LLONG;
      else if (*lenmod == LEN_INT)
	*lenmod = LEN_LONG;
      else
	return NULL;
      fmt++;
      goto lencheck;
    case 'j':
      *lenmod = LEN_INTMAX;
      fmt++;
      break;
    case 'z':
      *lenmod = LEN_SIZE;
      fmt++;
      break;
    case 't':
      *lenmod = LEN_PTRDIFF;
      fmt++;
      break;
    case 'L':
      *lenmod = LEN_LDOUBLE;
      fmt++;
      break;
    }
  return fmt;
}

static void
truncate_assign_signed (long long value, int lenmod, void *ptr)
{
  switch (lenmod)
    {
    case LEN_CHAR:
      *((char *) ptr) = (char) value;
      break;
    case LEN_SHORT:
      *((short *) ptr) = (short) value;
      break;
    case LEN_INT:
      *((int *) ptr) = (int) value;
      break;
    case LEN_LONG:
    case LEN_PTRDIFF:
      *((long *) ptr) = (long) value;
      break;
    default:
      *((long long *) ptr) = (long long) value;
    }
}

static void
truncate_assign_unsigned (unsigned long long value, int lenmod, void *ptr)
{
  switch (lenmod)
    {
    case LEN_CHAR:
      *((unsigned char *) ptr) = (unsigned char) value;
      break;
    case LEN_SHORT:
      *((unsigned short *) ptr) = (unsigned short) value;
      break;
    case LEN_INT:
      *((unsigned int *) ptr) = (unsigned int) value;
      break;
    case LEN_LONG:
    case LEN_PTRDIFF:
      *((unsigned long *) ptr) = (unsigned long) value;
      break;
    default:
      *((unsigned long long *) ptr) = (unsigned long long) value;
    }
}

static int
assign_numval (int flags, unsigned long argpos, int lenmod,
	       struct arguments *ar, va_list *args, long long sval,
	       unsigned long long uval)
{
  void *ptr;
  if (argpos == 0)
    {
      if (ar->curr < ar->len)
	{
	  ptr = ar->args[ar->curr];
	  ar->curr++;
	}
      else
	ptr = va_arg (*args, void *);
    }
  else
    {
      if (argpos > ar->len)
	{
	  void **temp;
	  int i = ar->len;
	  ar->len = argpos;
	  temp = realloc (ar->args, ar->len);
	  if (unlikely (temp == NULL))
	    {
	      errno = ENOMEM;
	      return -1;
	    }
	  ar->args = temp;
	  for (; i < ar->len; i++)
	    ar->args[i] = va_arg (*args, void *);
	}
      ptr = ar->args[argpos - 1];
    }

  if (flags & ASSIGNS)
    truncate_assign_signed (sval, lenmod, ptr);
  else
    truncate_assign_unsigned (uval, lenmod, ptr);
  return 0;
}

int
vscanf (const char *__restrict fmt, va_list args)
{
  return vfscanf (stdin, fmt, args);
}

int
vfscanf (FILE *__restrict stream, const char *__restrict fmt, va_list args)
{
  struct arguments ar;
  int amt = 0;
  flockfile (stream);
  ar.args = NULL;
  ar.len = 0;
  ar.curr = 0;

  while (*fmt != '\0')
    {
      char c;
      if (isspace (*fmt))
	/* Whitespace character directive */
	DO_OR_RETURN (pull_leading_whitespace (stream, __libc_locale));
      else if (*fmt == '%')
	{
	  int flags = 0;
	  int lenmod = LEN_INT;
	  unsigned long argpos = 0;
	  unsigned long max_field_width = 0;
	  long long sval;
	  unsigned long long uval;
	  fmt++;

	  /* Check for %% directive */
	  if (*fmt == '%')
	    {
	      c = fgetc_unlocked (stream);
	      if (c != '%')
		goto end;
	    }

	flagcheck:
	  /* Check for assignment suppression character */
	  if (*fmt == '*')
	    {
	      flags |= NOASSIGN;
	      fmt++;
	    }

	  /* Check for and parse maximum field width */
	  if (isdigit (*fmt) && *fmt != '0')
	    {
	      do
		{
		  if (max_field_width > ULONG_MAX / 10)
		    max_field_width = ULONG_MAX;
		  else
		    {
		      max_field_width *= 10;
		      if (max_field_width > ULONG_MAX - *fmt + '0')
			max_field_width = ULONG_MAX;
		      else
			max_field_width += *fmt - '0';
		    }
		}
	      while (isdigit (*++fmt));
	    }

	  /* If the next char is $, see if the previously read characters
	     were of the form %n$, and set the positional argument and retry
	     flag checking if so */
	  if (*fmt == '$' && flags == 0 /* No flags given */)
	    {
	      /* The digits we read for the max field width were actually for
		 the argument position */
	      argpos = max_field_width;
	      max_field_width = 0;
	      flags |= POSARG; /* Don't allow this check to happen again */
	      fmt++;
	      goto flagcheck;
	    }

	  /* Check for assignment allocation character */
	  if (*fmt == 'm')
	    {
	      flags |= MALLOC;
	      fmt++;
	    }

	  /* Check for length modifier characters */
	  fmt = parse_lenmod_chars (fmt, &lenmod);
	  if (fmt == NULL)
	    goto end;

	  /* Read conversion specifier */
	  switch (*fmt)
	    {
	    case 'd':
	      {
		int good;
	        sval = __fstrtox_l (stream, &good, 10, 1, LLONG_MIN, LLONG_MAX,
				    __libc_locale);
		if (!good)
		  goto end;
		flags |= ASSIGNS;
		break;
	      }
	    default:
	      goto end;
	    }

	  if ((flags & ASSIGNS || flags & ASSIGNU) && !(flags & NOASSIGN))
	    {
	      DO_OR_RETURN (assign_numval (flags, argpos, lenmod, &ar, &args,
					   sval, uval));
	      amt++;
	    }
	}
      else
	{
	  /* Ordinary character directive */
	  c = fgetc_unlocked (stream);
	  if (c == EOF || c != *fmt)
	    goto end;
	}
      fmt++;
    }

 end:
  free (ar.args);
  funlockfile (stream);
  return amt;
}

int
vsscanf (const char *__restrict str, const char *__restrict fmt, va_list args)
{
  return -1;
}
