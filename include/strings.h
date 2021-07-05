/* strings.h -- This file is part of OS/0 libc.
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

#ifndef _STRINGS_H
#define _STRINGS_H

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

int ffs (int value);
int ffsl (long value);
int ffsll (long long value);
int fls (int value);
int flsl (long value);
int flsll (long long value);

void bcopy (const void *src, void *dest, size_t len);
void bzero (void *ptr, size_t len);

__END_DECLS

#endif
