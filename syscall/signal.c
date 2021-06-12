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

#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>

int
sigprocmask (int how, const sigset_t *__restrict set, sigset_t *__restrict old)
{
  return syscall (SYS_sigprocmask, how, set, old);
}

int
sigsuspend (const sigset_t *mask)
{
  return syscall (SYS_sigsuspend, mask);
}

int
sigpending (sigset_t *set)
{
  return syscall (SYS_sigpending, set);
}

unsigned int
alarm (unsigned int seconds)
{
  return syscall (SYS_alarm, seconds);
}

int
pause (void)
{
  return syscall (SYS_pause);
}
