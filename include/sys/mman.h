/* mman.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <bits/mman.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

void *mmap (void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap (void *addr, size_t len);
int mprotect (void *addr, size_t len, int prot);

__END_DECLS

#endif
