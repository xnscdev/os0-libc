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

void *memcpy (void *__restrict dest, const void *__restrict src, size_t len);
void *mempcpy (void *__restrict dest, const void *__restrict src, size_t len);
void *memmove (void *dest, const void *src, size_t len);
void *memset (void *ptr, int c, size_t len);
int memcmp (const void *a, const void *b, size_t len);
void *memchr (const void *str, int c, size_t len);
void *memrchr (const void *str, int c, size_t len);

size_t strlen (const char *s);
size_t strnlen (const char *s, size_t len);
char *strdup (const char *s);
char *stpcpy (char *dest, const char *src);
char *stpncpy (char *dest, const char *src, size_t len);
char *strcpy (char *dest, const char *src);
char *strncpy (char *dest, const char *src, size_t len);
int strcmp (const char *a, const char *b);
int strncmp (const char *a, const char *b, size_t len);
int strcasecmp (const char *a, const char *b);
int strncasecmp (const char *a, const char *b, size_t len);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strstr (const char *haystack, const char *needle);
char *strcasestr (const char *haystack, const char *needle);
char *strcat (char *__restrict a, const char *__restrict b);
char *strncat (char *__restrict a, const char *__restrict b, size_t len);
size_t strspn (const char *str, const char *accept);
size_t strcspn (const char *str, const char *reject);
char *strpbrk (const char *str, const char *accept);

char *strtok (char *__restrict s, const char *__restrict delims);
char *strtok_r (char *__restrict s, const char *__restrict delims,
		char **__restrict saveptr);

char *strerror (int err);
char *strerror_r (int err, char *buffer, size_t len);
char *strsignal (int sig);

__END_DECLS

#endif
