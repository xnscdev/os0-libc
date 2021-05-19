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

/* Flag characters */

#define __fmt_THSEP   0x01 /* Print integers with thousands grouping chars */
#define __fmt_LJUST   0x02 /* Left-justify conversion */
#define __fmt_FRCSGN  0x04 /* Force printing number sign */
#define __fmt_PSPACE  0x08 /* Print space if sign not printed */
#define __fmt_ALTCONV 0x10 /* Alternate form conversion */
#define __fmt_PADZERO 0x20 /* Pad with zeroes instead of spaces */

/* Length modifiers */

#define __len_CHAR    0x01
#define __len_SHORT   0x02
#define __len_LONG    0x03
#define __len_LLONG   0x04
#define __len_INTMAX  0x05
#define __len_SIZE    0x06
#define __len_PTRDIFF 0x07
#define __len_LDOUBLE 0x08

/* How to interpret `prec' parameter in __printf_padwrite() */

#define __prec_MINWRITE 0x01 /* Write a minimum amount of digits */
#define __prec_MAXWRITE 0x02 /* Write a maximum amount of bytes */
#define __prec_RDXPREC  0x03 /* Write a minimum amount of digits after radix */
#define __prec_SIGDGTS  0x04 /* Write a minimum amount of significant digits */

/* Wrapper macro to properly get and convert variadic argument based on
   length modifier specified
   Should not be used outside of vfprintf(), uses local variables */

#define __printf_cvtnum(func)						\
  do									\
    {									\
      switch (lenmod)							\
	{								\
	case __len_CHAR:						\
	  {								\
	    char value = va_arg (args, int);				\
	    func;							\
	    break;							\
	  }								\
	case __len_SHORT:						\
	  {								\
	    short value = va_arg (args, int);				\
	    func;							\
	    break;							\
	  }								\
	case __len_LONG:						\
	  {								\
	    long value = va_arg (args, long);				\
	    func;							\
	    break;							\
	  }								\
	case __len_LLONG:						\
	  {								\
	    long long value = va_arg (args, long long);			\
	    func;							\
	    break;							\
	  }								\
	case __len_INTMAX:						\
	  {								\
	    intmax_t value = va_arg (args, intmax_t);			\
	    func;							\
	    break;							\
	  }								\
	case __len_SIZE:						\
	  {								\
	    ssize_t value = va_arg (args, ssize_t);			\
	    func;							\
	    break;							\
	  }								\
	case __len_PTRDIFF:						\
	  {								\
	    ptrdiff_t value = va_arg (args, ptrdiff_t);			\
	    func;							\
	    break;							\
	  }								\
	default:							\
	  {								\
	    int value = va_arg (args, int);				\
	    func;							\
	  }								\
	}								\
    }									\
  while (0)

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
		   size_t width, size_t prec, int flags, int mode)
{
  size_t len = strlen (str);
  size_t pad = width > len ? width - len : 0;
  int count = 0;
  char c = (flags & __fmt_PADZERO) && !(flags & __fmt_LJUST) ? '0' : ' ';

  if (mode == __prec_MINWRITE && prec > len - !isdigit (*str))
    {
      size_t diff = prec - len + !isdigit (*str);
      if (pad > diff)
	pad -= diff;
      else
	pad = 0;
    }

  /* Write left padding characters if right-justifying */
  if (!(flags & __fmt_LJUST))
    {
      if (__printf_fillchars (stream, pad, c) == EOF)
	return EOF;
      count += pad;
    }

  if (prec > 0)
    {
      if (mode == __prec_MAXWRITE)
	len = prec; /* Set maximum bytes to write */
      else if (mode == __prec_MINWRITE)
	{
	  /* Pad with zeroes to print minimum precision digits */
	  if (!isdigit (*str))
	    {
	      if (fputc (*str++, stream) == EOF)
		return EOF;
	      count++;
	      len--;
	    }
	  if (prec > len)
	    {
	      if (__printf_fillchars (stream, prec - len, '0') == EOF)
		return EOF;
	      count += prec - len;
	    }
	}
    }

  if (fwrite (str, 1, len, stream) < len)
    return EOF;
  count += len;

  /* Write right padding characters if left-justifying */
  if (flags & __fmt_LJUST)
    {
      if (__printf_fillchars (stream, pad, c) == EOF)
	return EOF;
      count += pad;
    }
  return count;
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
	  int width = 0;
	  int prec = 0;
	  int lenmod = 0;
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
	      if (width < 0)
		{
		  width = -width;
		  flags |= __fmt_LJUST;
		}
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

	  /* Check for precision */
	  if (*fmt == '.')
	    {
	      fmt++;
	      if (*fmt == '*')
		{
		  prec = va_arg (args, int);
		  if (prec < 0)
		    prec = 0;
		  fmt++;
		}
	      else
		{
		  while (isdigit (*fmt))
		    {
		      prec *= 10;
		      prec += *fmt++ - '0';
		    }
		}
	    }

	spec:
	  if (*fmt == 'c')
	    {
	      /* TODO Wide character printing */
	      unsigned char c = va_arg (args, int);
	      if (fputc (c, stream) == EOF)
		return EOF;
	    }
	  else if (*fmt == 'd' || *fmt == 'i')
	    {
	      int len;
	      __printf_cvtnum (__cvtnums (value, 10, __cvtdgl, flags));
	      len = __printf_padwrite (stream, __cvtbuf, width, prec, flags,
				       __prec_MINWRITE);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'h')
	    {
	      if (lenmod == 0)
		lenmod = __len_SHORT;
	      else if (lenmod == __len_SHORT)
		lenmod = __len_CHAR;
	      fmt++;
	      goto spec;
	    }
	  else if (*fmt == 'j')
	    {
	      if (lenmod == 0)
		lenmod = __len_INTMAX;
	      fmt++;
	      goto spec;
	    }
	  else if (*fmt == 'l')
	    {
	      if (lenmod == 0)
		lenmod = __len_LONG;
	      else if (lenmod == __len_LONG)
		lenmod = __len_LLONG;
	      fmt++;
	      goto spec;
	    }
	  else if (*fmt == 'n')
	    *va_arg (args, int *) = count + count;
	  else if (*fmt == 'o')
	    {
	      int len;
	      __printf_cvtnum (__cvtnumu (value, 8, __cvtdgl));
	      len = __printf_padwrite (stream, __cvtbuf, width, prec, flags,
				       __prec_MINWRITE);
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
	      len = __printf_padwrite (stream, __cvtbuf, width, prec, flags, 0);
	      if (len == EOF)
		return EOF;
	      count += len + 2;
	    }
	  else if (*fmt == 's')
	    {
	      const char *str = va_arg (args, const char *);
	      int len;
	      if (str == NULL)
	        str = "(null)";
	      len = __printf_padwrite (stream, str, width, prec, flags,
				       __prec_MAXWRITE);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 't')
	    {
	      if (lenmod == 0)
		lenmod = __len_PTRDIFF;
	      fmt++;
	      goto spec;
	    }
	  else if (*fmt == 'u')
	    {
	      int len;
	      __printf_cvtnum (__cvtnumu (value, 10, __cvtdgl));
	      len = __printf_padwrite (stream, __cvtbuf, width, prec, flags,
				       __prec_MINWRITE);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'x' || *fmt == 'X')
	    {
	      int len;
	      char *digits;
	      if (flags & __fmt_ALTCONV)
		{
		  /* Alternate conversion of %x or %X prints hex prefix */
		  if (__printf_write (stream, *fmt == 'x' ? "0x" : "0X") == EOF)
		    return EOF;
		  count += 2;
		}
	      if (*fmt == 'x')
		digits = __cvtdgl;
	      else
		digits = __cvtdgu;
	      __printf_cvtnum (__cvtnumu (value, 16, digits));
	      len = __printf_padwrite (stream, __cvtbuf, width, prec, flags,
				       __prec_MINWRITE);
	      if (len == EOF)
		return EOF;
	      count += len;
	    }
	  else if (*fmt == 'z')
	    {
	      if (lenmod == 0)
		lenmod = __len_SIZE;
	      fmt++;
	      goto spec;
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
