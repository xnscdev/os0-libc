/* sigset.c -- This file is part of OS/0 libc.
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

#include <errno.h>
#include <signal.h>
#include <string.h>

int
sigemptyset (sigset_t *set)
{
  memset (set, 0, sizeof (sigset_t));
  return 0;
}

int
sigfillset (sigset_t *set)
{
  memset (set, 0xff, sizeof (sigset_t));
  return 0;
}

int
sigaddset (sigset_t *set, int sig)
{
  if (sig < 0 || sig >= NR_signals)
    {
      errno = EINVAL;
      return -1;
    }
  set->sig[sig / CHAR_BIT] |= 1 << (7 - sig % CHAR_BIT);
  return 0;
}

int
sigdelset (sigset_t *set, int sig)
{
  if (sig < 0 || sig >= NR_signals)
    {
      errno = EINVAL;
      return -1;
    }
  set->sig[sig / CHAR_BIT] &= ~(1 << (7 - sig % CHAR_BIT));
  return 0;
}

int
sigismember (const sigset_t *set, int sig)
{
  if (sig < 0 || sig >= NR_signals)
    {
      errno = EINVAL;
      return -1;
    }
  return set->sig[sig / CHAR_BIT] & 1 << (7 - sig % CHAR_BIT) ? 1 : 0;
}
