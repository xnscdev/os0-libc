/* stdlib.h -- This file is part of OS/0 libc.
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

#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

void *malloc (size_t size);
void *calloc (size_t size, size_t block);
void *realloc (void *ptr, size_t size);
void free (void *ptr);

int atexit (void (*func) (void));

char *getenv (const char *name);
int setenv (const char *name, const char *value, int overwrite);
int putenv (char *str);
int unsetenv (const char *name);

void abort (void) __attribute__ ((noreturn));

int atoi (const char *str);
long atol (const char *str);
long long atoll (const char *str);

__END_DECLS

#endif
