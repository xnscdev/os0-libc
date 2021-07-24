/* syscall.c -- This file is part of OS/0 libc.
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

#include <sys/mman.h>
#include <sys/syscall.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>

/* Handle special cases of setting errno */

__hidden long
__syscall_handle_err (long num, long ret)
{
  uintptr_t rptr = ret;
  if (ret >= 0)
    return ret;
  switch (num)
    {
    case SYS_signal:
      errno = EINVAL;
      return (long) SIG_ERR;
    case SYS_nice:
    case SYS_getpriority:
      if (ret < 20)
	errno = -ret - 20;
      else
	return ret;
      break;
    case SYS_mmap:
      if (rptr >= UINTPTR_MAX - __NR_errno)
	{
	  errno = UINTPTR_MAX - rptr + 1;
	  return (long) MAP_FAILED;
	}
      return rptr;
    default:
      errno = -ret;
    }
  return -1;
}
