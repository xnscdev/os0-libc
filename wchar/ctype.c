/* ctype.c -- This file is part of OS/0 libc.
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

#include <wctype.h>

int
isalnum (int c)
{
  return isalpha (c) || isdigit (c);
}

int
isalnum_l (int c, locale_t loc)
{
  return isalnum (c);
}

int
isalpha (int c)
{
  return islower (c) || isupper (c);
}

int
isalpha_l (int c, locale_t loc)
{
  return isalpha (c);
}

int
isblank (int c)
{
  return c == '\t' || c == ' ';
}

int
isblank_l (int c, locale_t loc)
{
  return isblank (c);
}

int
iscntrl (int c)
{
  return (c >= '\0' && c <= '\037') || c == '\177';
}

int
iscntrl_l (int c, locale_t loc)
{
  return iscntrl (c);
}

int
isdigit (int c)
{
  return c >= '0' && c <= '9';
}

int
isdigit_l (int c, locale_t loc)
{
  return isdigit (c);
}

int
isgraph (int c)
{
  return c >= '!' && c < '\177';
}

int
isgraph_l (int c, locale_t loc)
{
  return isgraph (c);
}

int
islower (int c)
{
  return c >= 'a' && c <= 'z';
}

int
islower_l (int c, locale_t loc)
{
  return islower (c);
}

int
isprint (int c)
{
  return c == ' ' || isgraph (c);
}

int
isprint_l (int c, locale_t loc)
{
  return isprint (c);
}

int
ispunct (int c)
{
  return isgraph (c) && !isalnum (c);
}

int
ispunct_l (int c, locale_t loc)
{
  return ispunct (c);
}

int
isspace (int c)
{
  return isblank (c) || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int
isspace_l (int c, locale_t loc)
{
  return isspace (c);
}

int
isupper (int c)
{
  return c >= 'A' && c <= 'Z';
}

int
isupper_l (int c, locale_t loc)
{
  return isupper (c);
}

int
isxdigit (int c)
{
  return isdigit (c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int
isxdigit_l (int c, locale_t loc)
{
  return isxdigit (c);
}

int
tolower (int c)
{
  return isupper (c) ? c ^ 0x20 : c;
}

int
tolower_l (int c, locale_t loc)
{
  return tolower (c);
}

int
toupper (int c)
{
  return islower (c) ? c ^ 0x20 : c;
}

int
toupper_l (int c, locale_t loc)
{
  return toupper (c);
}

int
isascii (int c)
{
  return c >= '0' && c <= '\177';
}

int
toascii (int c)
{
  return c & '\177';
}
