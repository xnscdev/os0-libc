/* strtol_l.c -- This file is part of OS/0 libc.
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
#include <libc-locale.h>
#include <limits.h>
#include <stdlib.h>
#include <wctype.h>

/* Gets the digit value of the character in the specified base. */
static int
__baseval (char c, int base, locale_t loc)
{
  if (base < 11 && c - '0' < base)
    {
      if (isdigit_l (c, loc) && c - '0' < base)
	return c - '0';
    }
  else if (isdigit_l (c, loc))
    return c - '0';
  else if (islower_l (c, loc) && c + 10 - 'a' < base)
    return c + 10 - 'a';
  else if (isupper_l (c, loc) && c + 10 - 'A' < base)
    return c + 10 - 'A';
  return -1;
}

/* Determines the base of the number if base was specified as zero by checking
   the first characters of the input string */
static const char *
__get_base (const char *str, int *sign, int *base)
{
  while (isspace (*str))
    str++;
  if (*str == '-')
    {
      *sign = 1;
      str++;
    }
  else if (*str == '+')
    str++;
  if (*str == '0')
    {
      str++;
      if (*str == 'x' || *str == 'X')
	{
	  if (*base != 0 && *base != 16)
	    return str;
	  str++;
	  *base = 16;
	}
      else if (*base == 0)
	*base = 8;
    }
  return str;
}

/* Determines how many digits should be in the next group of digits based
   on grouping rules for the current locale. A return value of -1 indicates
   no grouping should be performed. */
static char
__get_grouping (int group, locale_t loc)
{
  int i;
  for (i = 0; ; i++)
    {
      char c = loc->__lconv.grouping[i];
      if (c == '\0')
	return i > 0 ? loc->__lconv.grouping[i - 1] : -1;
      else if (c == CHAR_MAX)
	return -1;
      if (i == group)
	return c;
    }
}

/* Calculates the number of groups that the input string has. */
static int
__calc_groups (const char *str, locale_t loc)
{
  int groups = 0;
  const char *ptr;
  for (ptr = str; *ptr == ',' || isdigit_l (*ptr, loc); ptr++)
    {
      if (*ptr == ',')
	groups++;
    }
  return groups;
}

static long long
__strtox_l (const char *__restrict str, char **__restrict end, int base,
	    int group, long long min, long long max, locale_t loc)
{
  long long value = 0;
  int sign = 0;
  int groups = 0;
  long long temp = 0;
  int grouplen = -1;
  int currlen = 0;
  const char *comma;
  int i;
  str = __get_base (str, &sign, &base);

  if (group)
    groups = __calc_groups (str, loc);

  for (; *str != '\0'; str++)
    {
      int b;
      if (*str == ',')
	{
	  if (!groups)
	    goto finish;
	  str++;
	  if (grouplen == -1)
	    goto nextgroup;
	  if (currlen != grouplen)
	    {
	      str = comma;
	      goto finish;
	    }
	  for (i = 0; i < grouplen; i++)
	    {
	      if (value > max / 10)
		goto overflow;
	      value *= 10;
	    }
	  value += temp;
	nextgroup:
	  temp = 0;
	  currlen = 0;
	  grouplen = __get_grouping (groups--, loc);
	  comma = str;
	}
      b = __baseval (*str, base, loc);
      if (b == -1)
	goto finish;
      if (grouplen == -1)
	{
	  if (value > max / 10)
	    goto overflow;
	  value *= 10;
	  value += b;
	}
      else
	{
	  if (currlen == grouplen)
	    goto finish; /* Should have reached another thousands separator */
	  if (temp > max / 10)
	    goto overflow;
	  temp *= 10;
	  temp += b;
	  currlen++;
	}
    }

 finish:
  *end = (char *) str;
  if (currlen == grouplen)
    {
      /* Append any remaining satisfied group of digits to the final result */
      for (i = 0; i < grouplen; i++)
	{
	  if (value > max / 10)
	    goto overflow;
	  value *= 10;
	}
      value += temp;
    }
  return sign ? -value : value;

 overflow:
  *end = (char *) str;
  errno = ERANGE;
  return sign ? min : max;
}

static unsigned long long
__strtoux_l (const char *__restrict str, char **__restrict end, int base,
	     int group, unsigned long long max, locale_t loc)
{
  unsigned long long value = 0;
  int sign = 0;
  int groups = 0;
  unsigned long long temp = 0;
  int grouplen = -1;
  int currlen = 0;
  const char *comma;
  int i;
  str = __get_base (str, &sign, &base);

  if (group)
    groups = __calc_groups (str, loc);

  for (; *str != '\0'; str++)
    {
      int b;
      if (*str == ',')
	{
	  if (!groups)
	    goto finish;
	  str++;
	  if (grouplen == -1)
	    goto nextgroup;
	  if (currlen != grouplen)
	    {
	      str = comma;
	      goto finish;
	    }
	  for (i = 0; i < grouplen; i++)
	    {
	      if (value > max / 10)
		goto overflow;
	      value *= 10;
	    }
	  value += temp;
	nextgroup:
	  temp = 0;
	  currlen = 0;
	  grouplen = __get_grouping (groups--, loc);
	  comma = str;
	}
      b = __baseval (*str, base, loc);
      if (b == -1)
	goto finish;
      if (grouplen == -1)
	{
	  if (value > max / 10)
	    goto overflow;
	  value *= 10;
	  value += b;
	}
      else
	{
	  if (currlen == grouplen)
	    goto finish; /* Should have reached another thousands separator */
	  if (temp > max / 10)
	    goto overflow;
	  temp *= 10;
	  temp += b;
	  currlen++;
	}
    }

 finish:
  *end = (char *) str;
  if (currlen == grouplen)
    {
      /* Append any remaining satisfied group of digits to the final result */
      for (i = 0; i < grouplen; i++)
	{
	  if (value > max / 10)
	    goto overflow;
	  value *= 10;
	}
      value += temp;
    }
  return sign ? -value : value;

 overflow:
  *end = (char *) str;
  errno = ERANGE;
  return sign ? 0 : max;
}

long
__strtol_l (const char *__restrict str, char **__restrict end, int base,
	    int group, locale_t loc)
{
  return __strtox_l (str, end, base, group, LONG_MIN, LONG_MAX, loc);
}

unsigned long
__strtoul_l (const char *__restrict str, char **__restrict end, int base,
	     int group, locale_t loc)
{
  return __strtoux_l (str, end, base, group, ULONG_MAX, loc);
}

long long
__strtoll_l (const char *__restrict str, char **__restrict end, int base,
	     int group, locale_t loc)
{
  return __strtox_l (str, end, base, group, LLONG_MIN, LLONG_MAX, loc);
}

unsigned long long
__strtoull_l (const char *__restrict str, char **__restrict end, int base,
	      int group, locale_t loc)
{
  return __strtoux_l (str, end, base, group, ULLONG_MAX, loc);
}

long
strtol_l (const char *__restrict str, char **__restrict end, int base,
	  locale_t loc)
{
  return __strtol_l (str, end, base, 0, loc);
}

unsigned long
strtoul_l (const char *__restrict str, char **__restrict end, int base,
	   locale_t loc)
{
  return __strtoul_l (str, end, base, 0, loc);
}

long long
strtoll_l (const char *__restrict str, char **__restrict end, int base,
	   locale_t loc)
{
  return __strtoll_l (str, end, base, 0, loc);
}

unsigned long long
strtoull_l (const char *__restrict str, char **__restrict end, int base,
	    locale_t loc)
{
  return __strtoull_l (str, end, base, 0, loc);
}
