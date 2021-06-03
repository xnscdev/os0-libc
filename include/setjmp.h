/* setjmp.h -- This file is part of OS/0 libc.
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

#ifndef _SETJMP_H
#define _SETJMP_H

#include <sys/cdefs.h>
#include <sys/signal.h>

struct __jmp_buf
{
  int __env[6];
  int __mask_saved;
  sigset_t __mask;
};

typedef struct __jmp_buf jmp_buf[1];
typedef struct __jmp_buf sigjmp_buf[1];

__BEGIN_DECLS

void _longjmp (jmp_buf env, int val) __attribute__ ((noreturn));
int _setjmp (jmp_buf env) __attribute__ ((returns_twice));
void longjmp (jmp_buf env, int val) __attribute__ ((noreturn));
int setjmp (jmp_buf env) __attribute__ ((returns_twice));
void siglongjmp (sigjmp_buf env, int val) __attribute__ ((noreturn));
int sigsetjmp (sigjmp_buf env, int save_mask) __attribute__ ((returns_twice));

__END_DECLS

#endif
