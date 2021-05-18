/* vfprintf.c -- This file is part of OS/0 libc.
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
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define __fmt_THSEP   0x01 /* Print integers with thousands grouping chars */
#define __fmt_LJUST   0x02 /* Left-justify conversion */
#define __fmt_FRCSGN  0x04 /* Force printing number sign */
#define __fmt_PSPACE  0x08 /* Print space if sign not printed */
#define __fmt_ALTCONV 0x10 /* Alternate form conversion */
#define __fmt_PADZERO 0x20 /* Pad with zeroes instead of spaces */

static char *__cvtdgl =
  "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
static char *__cvtdgu =
  "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char __cvtbuf[64];

static void
__cvtnums (intmax_t value, int base, const char *digits, int flags)
{
  char *ptr = __cvtbuf;
  char *rev = __cvtbuf;
  intmax_t temp;
  char c;

  do
    {
      temp = value;
      value /= base;
      *ptr++ = digits[35 + (temp - value * base)];
    }
  while (value != 0);

  if (temp < 0)
    *ptr++ = '-';
  else if (flags & __fmt_FRCSGN)
    *ptr++ = '+';
  else if (flags & __fmt_PSPACE)
    *ptr++ = ' ';
  *ptr-- = '\0';
  while (rev < ptr)
    {
      c = *ptr;
      *ptr-- = *rev;
      *rev++ = c;
    }
}

static void
__cvtnumu (uintmax_t value, int base, const char *digits)
{
  char *ptr = __cvtbuf;
  char *rev = __cvtbuf;
  uintmax_t temp;
  char c;

  do
    {
      temp = value;
      value /= base;
      *ptr++ = digits[35 + (temp - value * base)];
    }
  while (value != 0);

  *ptr-- = '\0';
  while (rev < ptr)
    {
      c = *ptr;
      *ptr-- = *rev;
      *rev++ = c;
    }
}

static int
__printf_write (FILE *__restrict stream, const char *__restrict str)
{
  int count;
  for (; *str != '\0'; str++, count++)
    {
      if (fputc (*str, stream) == EOF)
	return EOF;
    }
  return count;
}

static int
__printf_fillchars (FILE *stream, size_t len, char c)
{
  size_t i;
  for (i = 0; i < len; i++)
    {
      if (fputc (c, stream) == EOF)
	return EOF;
    }
  return 0;
}

static int
__printf_padwrite (FILE *__restrict stream, const char *__restrict str,
		   size_t width, int flags)
{
  size_t len = strlen (str);
  size_t pad = width > len ? width - len : 0;
  char c = flags & __fmt_PADZERO ? '0' : ' ';
  if (!(flags & __fmt_LJUST))
    {
      if (__printf_fillchars (stream, pad, c) == EOF)
	return EOF;
    }
  if (__printf_write (stream, str) == EOF)
    return EOF;
  if (flags & __fmt_LJUST)
    {
      if (__printf_fillchars (stream, pad, c) == EOF)
	return EOF;
    }
  return len + pad;
}

int
vprintf (const char *__restrict fmt, va_list args)
{
  return vfprintf (stdout, fmt, args);
}

int
vfprintf (FILE *__restrict stream, const char *__restrict fmt, va_list args)
{
  int count = 0;
  while (*fmt != '\0')
    {
      if (*fmt == '%')
	{
	  int flags = 0;
	  size_t width = 0;
	  fmt++;

	  /* Check for flag characters */
	  while (1)
	    {
	      switch (*fmt)
		{
		case '\'':
		  flags |= __fmt_THSEP;
		  break;
		case '-':
		  flags |= __fmt_LJUST;
		  break;
		case '+':
		  flags |= __fmt_FRCSGN;
		  break;
		case ' ':
		  flags |= __fmt_PSPACE;
		  break;
		case '#':
		  flags |= __fmt_ALTCONV;
		  break;
		case '0':
		  flags |= __fmt_PADZERO;
		  break;
		case '\0':
		  return count;
		default:
		  goto conv;
		}
	      fmt++;
	    }

	conv:
	  /* Check for field width */
	  if (*fmt == '*')
	    {
	      width = va_arg (args, int);
	      fmt++;
	    }
	  else
	    {
	      while (isdigit (*fmt))
		{
		  width *= 10;
		  width += *fmt++ - '0';
		}
	    }

	  if (*fmt == 'c')
	    {
	      unsigned char c = va_arg (args, int);
	      if (fputc (c, stream) == EOF)
		return EOF;
	    }
	  else if (*fmt == 's')
	    {
	      const char *str = va_arg (args, const char *);
	      int len;
	      if (str == NULL)
		len = __printf_write (stream, "(null)");
	      else
	        len = __printf_padwrite (stream, str, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'd' || *fmt == 'i')
	    {
	      int value = va_arg (args, int);
	      int len;
	      __cvtnums (value, 10, __cvtdgl, flags);
	      len = __printf_padwrite (stream, __cvtbuf, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'n')
	    *va_arg (args, int *) = count + count;
	  else if (*fmt == 'o')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      int len;
	      __cvtnumu (value, 8, __cvtdgl);
	      len = __printf_padwrite (stream, __cvtbuf, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'p')
	    {
	      void *ptr = va_arg (args, void *);
	      int len;
	      __cvtnumu ((uintptr_t) ptr, 16, __cvtdgl);
	      if (__printf_write (stream, "0x") == EOF)
		return EOF;
	      len = __printf_padwrite (stream, __cvtbuf, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len + 2;
	    }
	  else if (*fmt == 'u')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      int len;
	      __cvtnumu (value, 10, __cvtdgl);
	      len = __printf_padwrite (stream, __cvtbuf, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'x' || *fmt == 'X')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      int len;
	      if (flags & __fmt_ALTCONV)
		{
		  /* Alternate conversion of %x or %X prints hex prefix */
		  if (__printf_write (stream, *fmt == 'x' ? "0x" : "0X") == EOF)
		    return EOF;
		  count += 2;
		}
	      __cvtnumu (value, 16, *fmt == 'x' ? __cvtdgl : __cvtdgu);
	      len = __printf_padwrite (stream, __cvtbuf, width, flags);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else
	    {
	      /* Also handles '%%' conversion specifier */
	      if (fputc (*fmt, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else
	{
	  if (fputc (*fmt++, stream) == EOF)
	    return EOF;
	  count++;
	}
    }
  return count;
}
