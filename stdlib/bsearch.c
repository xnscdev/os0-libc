/* bsearch.c -- This file is part of OS/0 libc.
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

#include <stdlib.h>

void *
bsearch (const void *key, const void *base, size_t len, size_t size,
	 int (*cmp) (const void *, const void *))
{
  size_t low = 0;
  size_t high = len;
  while (low < high)
    {
      size_t mid = (low + high) / 2;
      void *ptr = (void *) ((const char *) base + (mid * size));
      int ret = cmp (key, ptr);
      if (ret < 0)
	high = mid;
      else if (ret > 0)
	low = mid + 1;
      else
	return ptr;
    }
  return NULL;
}
