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

#include <ctype.h>

int
isalnum (int c)
{
  return isalpha (c) || isdigit (c);
}

int
isalpha (int c)
{
  return islower (c) || isupper (c);
}

int
isascii (int c)
{
  return c >= 0 && c < 0x80;
}

int
isblank (int c)
{
  return c == ' ' || c == '\t';
}

int
iscntrl (int c)
{
  return (unsigned char) c < ' ' || c == '\177';
}

int
isdigit (int c)
{
  return c >= '0' && c <= '9';
}

int
isgraph (int c)
{
  return c > ' ' && c <= '~';
}

int
islower (int c)
{
  return c >= 'a' && c <= 'z';
}

int
isprint (int c)
{
  return c == ' ' || isgraph (c);
}

int
ispunct (int c)
{
  return isgraph (c) && !isalnum (c);
}

int
isspace (int c)
{
  return isblank (c) || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int
isupper (int c)
{
  return c >= 'A' && c <= 'Z';
}

int
isxdigit (int c)
{
  return isdigit (c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int
toascii (int c)
{
  return c & 0x7f;
}

int
tolower (int c)
{
  return isupper (c) ? c + 0x20 : c;
}

int
toupper (int c)
{
  return islower (c) ? c - 0x20 : c;
}
