/* sigjmp.c -- This file is part of OS/0 libc.
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

#include <setjmp.h>
#include <signal.h>
#include <stddef.h>

void
siglongjmp (sigjmp_buf env, int val)
{
  if (env[0].__mask_saved)
    sigprocmask (SIG_SETMASK, &env[0].__mask, NULL);
  longjmp (env, val);
}

__hidden int
__sigsetjmp_maybe_save_mask (sigjmp_buf env, int save_mask)
{
  env[0].__mask_saved =
    save_mask && sigprocmask (SIG_BLOCK, NULL, &env[0].__mask) == 0;
  return 0;
}
