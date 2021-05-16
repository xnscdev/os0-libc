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

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
      const char *fmt_begin;
      if (fmt[0] != '%' || fmt[1] == '%')
	{
	  /* Copy characters directly */
	  size_t len = 1;
	  if (*fmt == '%')
	    len++;
	  while (fmt[len] != '\0' && fmt[len] != '%')
	    len++;
	  if (fwrite (fmt, 1, len, stream) < len)
	    return EOF;
	  fmt += len;
	  count += len;
	  continue;
	}

      fmt_begin = fmt++;
      if (*fmt == 'c')
	{
	  char c = (char) va_arg (args, int);
	  if (fputc (c, stream) == EOF)
	    return EOF;
	  fmt++;
	}
      else if (*fmt == 's')
	{
	  const char *str = va_arg (args, const char *);
	  for (; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (*fmt == 'd' || *fmt == 'i')
	{
	  int value = va_arg (args, int);
	  const char *str;
	  __cvtnums (value, 10, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (*fmt == 'o')
	{
	  unsigned int value = va_arg (args, unsigned int);
	  const char *str;
	  __cvtnumu (value, 8, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (*fmt == 'p')
	{
	  void *ptr = va_arg (args, void *);
	  const char *str;
	  __cvtnumu ((uintptr_t) ptr, 16, __cvtdgl);
	  if (fputs ("0x", stream) == EOF)
	    return EOF;
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (*fmt == 'u')
	{
	  unsigned int value = va_arg (args, unsigned int);
	  const char *str;
	  __cvtnumu (value, 10, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (*fmt == 'x' || *fmt == 'X')
	{
	  unsigned int value = va_arg (args, unsigned int);
	  const char *str;
	  __cvtnumu (value, 16, *fmt == 'x' ? __cvtdgl : __cvtdgu);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt++;
	}
      else if (strncmp (fmt, "ld", 2) == 0 || strncmp (fmt, "li", 2) == 0)
	{
	  long value = va_arg (args, long);
	  const char *str;
	  __cvtnums (value, 10, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt += 2;
	}
      else if (strncmp (fmt, "lo", 2) == 0)
	{
	  unsigned long value = va_arg (args, long);
	  const char *str;
	  __cvtnumu (value, 8, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt += 2;
	}
      else if (strncmp (fmt, "lu", 2) == 0)
	{
	  unsigned long value = va_arg (args, long);
	  const char *str;
	  __cvtnumu (value, 10, __cvtdgl);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt += 2;
	}
      else if (strncmp (fmt, "lx", 2) == 0 || strncmp (fmt, "lX", 2) == 0)
	{
	  unsigned long value = va_arg (args, long);
	  const char *str;
	  __cvtnumu (value, 16, fmt[1] == 'x' ? __cvtdgl : __cvtdgu);
	  for (str = __cvtbuf; *str != '\0'; str++)
	    {
	      if (fputc (*str, stream) == EOF)
		return EOF;
	      count++;
	    }
	  fmt += 2;
	}
      else
	{
	  /* TODO Check for padding characters */
	  for (fmt = fmt_begin; *fmt != '\0'; fmt++)
	    {
	      if (fputc (*fmt, stream) == EOF)
		return EOF;
	      count++;
	    }
	}
    }
  return count;
}
