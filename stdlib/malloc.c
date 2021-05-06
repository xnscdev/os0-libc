/* malloc.c -- This file is part of OS/0 libc.
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

#include <branch.h>
#include <stdlib.h>
#include <string.h>

/* Temporary stub implementations */

void *
malloc (size_t size)
{
  abort ();
}

void *
calloc (size_t size, size_t block)
{
  void *ptr = malloc (size * block);
  if (unlikely (ptr == NULL))
    return ptr;
  memset (ptr, 0, size * block);
  return ptr;
}

void *
realloc (void *ptr, size_t size)
{
  if (ptr == NULL)
    return malloc (size);
  abort ();
}

void
free (void *ptr)
{
}
