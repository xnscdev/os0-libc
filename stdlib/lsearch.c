/* lsearch.c -- This file is part of OS/0 libc.
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
#include <string.h>

void *
lfind (const void *key, const void *base, size_t *len, size_t size,
       int (*cmp) (const void *, const void *))
{
  const void *ptr = base;
  size_t count = 0;
  while (count < *len && cmp (key, ptr) != 0)
    {
      ptr += size;
      count++;
    }
  return count < *len ? (void *) ptr : NULL;
}

void *
lsearch (const void *key, const void *base, size_t *len, size_t size,
	 int (*cmp) (const void *, const void *))
{
  void *ptr = lfind (key, base, len, size, cmp);
  if (ptr == NULL)
    {
      ptr = (void *) ((uintptr_t) base + *len * size);
      /* Copy the key value into the end of the array */
      memcpy (ptr, key, size);
      (*len)++;
    }
  return ptr;
}
