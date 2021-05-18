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
__cvtnums (intmax_t value, int base, const char *digits)
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
	  if (*fmt == 'c')
	    {
	      unsigned char c = va_arg (args, int);
	      if (fputc (c, stream) == EOF)
		return EOF;
	    }
	  else if (*fmt == 's')
	    {
	      const char *str = va_arg (args, const char *);
	      for (; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
	    }
	  else if (*fmt == 'd' || *fmt == 'i')
	    {
	      int value = va_arg (args, int);
	      const char *str;
	      if (value > 0 && (flags & __fmt_FRCSGN))
		{
		  if (fputc ('+', stream) == EOF)
		    return EOF;
		  count++;
		}
	      __cvtnums (value, 10, __cvtdgl);
	      for (str = __cvtbuf; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
	    }
	  else if (*fmt == 'n')
	    *va_arg (args, int *) = count + count;
	  else if (*fmt == 'o')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      const char *str;
	      __cvtnumu (value, 8, __cvtdgl);
	      for (str = __cvtbuf; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
	    }
	  else if (*fmt == 'p')
	    {
	      void *ptr = va_arg (args, void *);
	      const char *str;
	      __cvtnumu ((uintptr_t) ptr, 16, __cvtdgl);
	      if (fwrite ("0x", 1, 2, stream) < 2)
		return EOF;
	      for (str = __cvtbuf; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
	    }
	  else if (*fmt == 'u')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      const char *str;
	      __cvtnumu (value, 10, __cvtdgl);
	      for (str = __cvtbuf; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
	    }
	  else if (*fmt == 'x' || *fmt == 'X')
	    {
	      unsigned int value = va_arg (args, unsigned int);
	      const char *str;
	      if (flags & __fmt_ALTCONV)
		{
		  /* Alternate conversion of %x or %X prints hex prefix */
		  if (fwrite (*fmt == 'x' ? "0x" : "0X", 1, 2, stream) < 2)
		    return EOF;
		  count += 2;
		}
	      __cvtnumu (value, 16, *fmt == 'x' ? __cvtdgl : __cvtdgu);
	      for (str = __cvtbuf; *str != '\0'; str++, count++)
		{
		  if (fputc (*str, stream) == EOF)
		    return EOF;
		}
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
