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

#define STRBUF_START_SIZE 16

/* *scanf flags */

#define NOASSIGN 0x01
#define MALLOC   0x02

#define POSARG   0x1000

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
      int ret = (expr);				\
      if (ret == -1)				\
        goto end;				\
      amt += ret;				\
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
parse_preconvspec_chars (const char *fmt, int *flags, unsigned long *argpos,
			 unsigned long *max_field_width, int *lenmod)
{
  unsigned long num;
 flagcheck:
  num = 0;

  /* Check for assignment suppression character */
  if (*fmt == '*')
    {
      *flags |= NOASSIGN;
      fmt++;
    }

  /* Check for and parse maximum field width */
  if (isdigit (*fmt) && *fmt != '0')
    {
      do
	{
	  if (num > ULONG_MAX / 10)
	    num = ULONG_MAX;
	  else
	    {
	      num *= 10;
	      if (num > ULONG_MAX - *fmt + '0')
		num = ULONG_MAX;
	      else
		num += *fmt - '0';
	    }
	}
      while (isdigit (*++fmt));
    }

  /* If the next char is $, see if the previously read characters
     were of the form %n$, and set the positional argument and retry
     flag checking if so */
  if (num > 0 && *fmt == '$' && *flags == 0 /* No *flags given */)
    {
      /* The digits we read for the max field width were actually for
	 the argument position */
      *argpos = num;
      *flags |= POSARG; /* Don't allow this check to happen again */
      fmt++;
      goto flagcheck;
    }
  else
    *max_field_width = num;

  /* Check for assignment allocation character */
  if (*fmt == 'm')
    {
      *flags |= MALLOC;
      fmt++;
    }

  /* Check for length modifier characters */
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

static void *
get_argument (struct arguments *ar, va_list *args, unsigned long pos)
{
  if (pos == 0)
    {
      if (ar->curr < ar->len)
	{
	  void *ptr = ar->args[ar->curr];
	  ar->curr++;
	  return ptr;
	}
      return va_arg (*args, void *);
    }
  else
    {
      if (pos > ar->len)
	{
	  void **temp;
	  int i = ar->len;
	  ar->len = pos;
	  temp = realloc (ar->args, ar->len);
	  if (unlikely (temp == NULL))
	    {
	      errno = ENOMEM;
	      return NULL;
	    }
	  ar->args = temp;
	  for (; i < ar->len; i++)
	    ar->args[i] = va_arg (*args, void *);
	}
      return ar->args[pos - 1];
    }
}

static int
handle_fscanf_string (FILE *stream, int flags, int lenmod, struct arguments *ar,
		      va_list *args, unsigned long pos)
{
  size_t bufsize = STRBUF_START_SIZE;
  size_t count = 0;
  char *buffer;
  char c;
  if (!(flags & NOASSIGN))
    {
      if (flags & MALLOC)
	{
	  buffer = malloc (bufsize);
	  if (unlikely (buffer == NULL))
	    {
	      errno = ENOMEM;
	      return -1;
	    }
	}
      else
	buffer = get_argument (ar, args, pos);
    }
  if (pull_leading_whitespace (stream, __libc_locale) != 0)
    {
      free (buffer);
      return -1;
    }
  while (1)
    {
      c = fgetc_unlocked (stream);
      if (c == EOF || isspace (c))
	break;
      if (!(flags & NOASSIGN))
	{
	  buffer[count++] = c;
	  if ((flags & MALLOC) && count == bufsize)
	    {
	      char *temp;
	      bufsize *= 2;
	      temp = realloc (buffer, bufsize);
	      if (unlikely (temp == NULL))
		{
		  errno = ENOMEM;
		  free (buffer);
		  return -1;
		}
	      buffer = temp;
	    }
	}
    }
  __ungetc_unlocked (c, stream);
  if (!(flags & NOASSIGN))
    {
      buffer[count] = '\0';
      if (flags & MALLOC)
	*((char **) get_argument (ar, args, pos)) = buffer;
      return 1;
    }
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
	  unsigned long max_field_width;
	  fmt++;

	  /* Check for %% directive */
	  if (*fmt == '%')
	    {
	      c = fgetc_unlocked (stream);
	      if (c != '%')
		goto end;
	      fmt++;
	      continue;
	    }

	  /* Parse characters before conversion specifier */
	  fmt = parse_preconvspec_chars (fmt, &flags, &argpos, &max_field_width,
					 &lenmod);
	  if (fmt == NULL)
	    goto end;

	  /* Read conversion specifier */
	  switch (*fmt)
	    {
	    case 'd':
	    case 'i':
	      {
		int good;
	        long long value =
		  __fstrtox_l (stream, &good, (*fmt == 'd') * 10, 1,
			       max_field_width, LLONG_MIN, LLONG_MAX,
			       __libc_locale);
		void *ptr;
		if (!good)
		  goto end;
		ptr = get_argument (&ar, &args, argpos);
		if (!(flags & NOASSIGN))
		  {
		    truncate_assign_signed (value, lenmod, ptr);
		    amt++;
		  }
		break;
	      }
	    case 'o':
	    case 'u':
	    case 'x':
	      {
		int good;
		int base = *fmt == 'o' ? 8 : *fmt == 'u' ? 10 : 16;
	        unsigned long long value =
		  __fstrtoux_l (stream, &good, base, 1, max_field_width,
				ULLONG_MAX, __libc_locale);
		void *ptr;
		if (!good)
		  goto end;
		ptr = get_argument (&ar, &args, argpos);
		if (!(flags & NOASSIGN))
		  {
		    truncate_assign_unsigned (value, lenmod, ptr);
		    amt++;
		  }
		break;
	      }
	    case 's':
	      DO_OR_RETURN (handle_fscanf_string (stream, flags, lenmod, &ar,
						  &args, argpos));
	      break;
	    default:
	      goto end;
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
