/* strtol.c -- This file is part of OS/0 libc.
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
#include <stdlib.h>

static int
__baseval (char c, int base)
{
  if (base < 11 && c - '0' < base)
    {
      if (isdigit (c) && c - '0' < base)
	return c - '0';
    }
  else if (isdigit (c))
    return c - '0';
  else if (islower (c) && c + 10 - 'a' < base)
    return c + 10 - 'a';
  else if (isupper (c) && c + 10 - 'A' < base)
    return c + 10 - 'A';
  return -1;
}

int
atoi (const char *str)
{
  return (int) atol (str);
}

long
atol (const char *str)
{
  return strtol (str, NULL, 10);
}

long long
atoll (const char *str)
{
  return strtoll (str, NULL, 10);
}

long
strtol (const char *__restrict str, char **__restrict end, int base)
{
  long value = 0;
  int sign = 0;
  while (isspace (*str))
    str++;
  if (*str == '-')
    {
      sign = 1;
      str++;
    }
  else if (*str == '+')
    str++;
  if (*str == '0')
    {
      str++;
      if (*str == 'x' || *str == 'X')
	{
	  str++;
	  base = 16;
	}
      else
	base = 8;
    }
  for (; *str != '\0'; str++)
    {
      int b = __baseval (*str, base);
      if (b == -1)
	goto finish;
      value *= 10;
      value += b;
    }

 finish:
  *end = (char *) str;
  return sign ? -value : value;
}

long long
strtoll (const char *__restrict str, char **__restrict end, int base)
{
  long long value = 0;
  int sign = 0;
  while (isspace (*str))
    str++;
  if (*str == '-')
    {
      sign = 1;
      str++;
    }
  else if (*str == '+')
    str++;
  if (*str == '0')
    {
      str++;
      if (*str == 'x' || *str == 'X')
	{
	  str++;
	  base = 16;
	}
      else
	base = 8;
    }
  for (; *str != '\0'; str++)
    {
      int b = __baseval (*str, base);
      if (b == -1)
	goto finish;
      value *= 10;
      value += b;
    }

 finish:
  *end = (char *) str;
  return sign ? -value : value;
}
