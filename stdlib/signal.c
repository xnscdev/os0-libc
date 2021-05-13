/* signal.c -- This file is part of OS/0 libc.
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
#include <unistd.h>

sighandler_t
signal (int sig, sighandler_t func)
{
  struct sigaction old;
  struct sigaction act;
  if (sig < 0 || sig >= NR_signals || sig == SIGKILL || sig == SIGSTOP)
    {
      errno = EINVAL;
      return SIG_ERR;
    }
  act.sa_handler = func;
  act.sa_sigaction = NULL;
  act.sa_flags = 0;
  memset (&act.sa_mask, 0, sizeof (sigset_t));
  if (sigaction (sig, &act, &old) == -1)
    return SIG_ERR;
  return old.sa_handler; /* FIXME If SIG_SETINFO is set? */
}

int
raise (int sig)
{
  return kill (getpid (), sig);
}
