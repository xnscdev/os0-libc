/* exit.c -- This file is part of OS/0 libc.
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

#include <sys/syslimits.h>
#include <assert.h>
#include <errno.h>
#include <locks.h>
#include <stdlib.h>
#include <unistd.h>

static void (*__atexit_funcs[ATEXIT_MAX]) (void);
static int __atexit_ptr;

__lock_t __atexit_lock __hidden;

int
atexit (void (*func) (void))
{
  assert (func != NULL);
  if (__atexit_ptr >= ATEXIT_MAX)
    {
      errno = ENOMEM;
      return -1;
    }
  __libc_lock (&__atexit_lock);
  __atexit_funcs[__atexit_ptr++] = func;
  __libc_unlock (&__atexit_lock);
  return 0;
}

void
exit (int code)
{
  while (__atexit_ptr-- > 0)
    __atexit_funcs[__atexit_ptr] ();
  _exit (code);
}
