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

#include <stdlib.h>
#include <string.h>
#include <symbol.h>

void *dlmalloc (size_t size);
void *dlcalloc (size_t size, size_t block);
void *dlrealloc (void *ptr, size_t size);
void dlfree (void *ptr);

void *
__malloc (size_t size)
{
  return dlmalloc (size);
}

weak_alias (__malloc, malloc);

void *
__calloc (size_t size, size_t block)
{
  return dlcalloc (size, block);
}

weak_alias (__calloc, calloc);

void *
__realloc (void *ptr, size_t size)
{
  return dlrealloc (ptr, size);
}

weak_alias (__realloc, realloc);

void
__free (void *ptr)
{
  dlfree (ptr);
}

weak_alias (__free, free);
