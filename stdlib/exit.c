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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static void (*__atexit_funcs[ATEXIT_MAX]) (void);
static int __atexit_ptr;

int
atexit (void (*func) (void))
{
  if (__atexit_ptr >= ATEXIT_MAX)
    {
      errno = ENOMEM;
      return -1;
    }
  __atexit_funcs[__atexit_ptr++] = func;
  return 0;
}

void
exit (int code)
{
  int i;
  for (i = 0; i < __atexit_ptr; i++)
    __atexit_funcs[i] ();
  _exit (code);
}
