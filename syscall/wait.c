/* wait.c -- This file is part of OS/0 libc.
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
#include <sys/wait.h>
#include <unistd.h>

pid_t
wait (int *status)
{
  return wait4 (-1, status, 0, NULL);
}

pid_t
wait3 (int *status, int options, struct rusage *usage)
{
  return wait4 (-1, status, options, usage);
}

pid_t
wait4 (pid_t pid, int *status, int options, struct rusage *usage)
{
  return syscall (SYS_wait4, pid, status, options, usage);
}

pid_t
waitpid (pid_t pid, int *status, int options)
{
  return syscall (SYS_waitpid, pid, status, options, NULL);
}
