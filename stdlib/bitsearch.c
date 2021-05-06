/* bitsearch.c -- This file is part of OS/0 libc.
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

#include <limits.h>
#include <string.h>

int
ffs (int value)
{
  int i;
  for (i = 0; i < CHAR_BIT * sizeof (int); i++)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}

int
ffsl (long value)
{
  int i;
  for (i = 0; i < CHAR_BIT * sizeof (long); i++)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}

int
ffsll (long long value)
{
  int i;
  for (i = 0; i < CHAR_BIT * sizeof (long long); i++)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}

int
fls (int value)
{
  int i;
  for (i = CHAR_BIT * sizeof (int) - 1; i >= 0; i--)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}

int
flsl (long value)
{
  int i;
  for (i = CHAR_BIT * sizeof (long) - 1; i >= 0; i--)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}

int
flsll (long long value)
{
  int i;
  for (i = CHAR_BIT * sizeof (long long) - 1; i >= 0; i--)
    {
      if (value & (1 << i))
	return i + 1;
    }
  return 0;
}
