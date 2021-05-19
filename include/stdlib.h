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

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define __atexit_LIMIT 32

#include <bits/types/div.h>

__BEGIN_DECLS

void *malloc (size_t size);
void *calloc (size_t size, size_t block);
void *realloc (void *ptr, size_t size);
void free (void *ptr);

char *getenv (const char *name);
int setenv (const char *name, const char *value, int overwrite);
int putenv (char *str);
int unsetenv (const char *name);

int atexit (void (*func) (void));
void exit (int code) __attribute__ ((noreturn));
void abort (void) __attribute__ ((noreturn));

int abs (int n);
div_t div (int num, int denom);
ldiv_t ldiv (long num, long denom);
lldiv_t lldiv (long long num, long long denom);
imaxdiv_t imaxdiv (intmax_t num, intmax_t denom);

int atoi (const char *str);
long atol (const char *str);
long long atoll (const char *str);
long strtol (const char *__restrict str, char **__restrict end, int base);
long long strtoll (const char *__restrict str, char **__restrict end, int base);

void *bsearch (const void *key, const void *base, size_t len, size_t size,
	       int (*cmp) (const void *, const void *));
void *lfind (const void *key, const void *base, size_t *len, size_t size,
	     int (*cmp) (const void *, const void *));
void *lsearch (const void *key, const void *base, size_t *len, size_t size,
	       int (*cmp) (const void *, const void *));
void qsort (void *const pbase, size_t len, size_t size,
	    int (*cmp) (const void *, const void *));

__END_DECLS

#endif
