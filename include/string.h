/* string.h -- This file is part of OS/0 libc.
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

#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

int ffs (int value);
int ffsl (long value);
int ffsll (long long value);
int fls (int value);
int flsl (long value);
int flsll (long long value);

void *memcpy (void *__restrict dest, const void *__restrict src, size_t len);
void *memmove (void *dest, const void *src, size_t len);
void *memset (void *ptr, int c, size_t len);
int memcmp (const void *a, const void *b, size_t len);

size_t strlen (const char *s);
char *strdup (const char *s);
char *stpcpy (char *dest, const char *src);
char *stpncpy (char *dest, const char *src, size_t len);
char *strcpy (char *dest, const char *src);
char *strncpy (char *dest, const char *src, size_t len);
int strcmp (const char *a, const char *b);
int strncmp (const char *a, const char *b, size_t len);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strtok (char *__restrict s, const char *__restrict delims);

const char *strerror (int errno);

__END_DECLS

#endif
