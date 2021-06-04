/* sysconf.c -- This file is part of OS/0 libc.
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
#include <unistd.h>

long
sysconf (int name)
{
  switch (name)
    {
    case _SC_AIO_LISTIO_MAX:
      return AIO_LISTIO_MAX;
    case _SC_AIO_MAX:
      return AIO_MAX;
    case _SC_AIO_PRIO_DELTA_MAX:
      return AIO_PRIO_DELTA_MAX;
    case _SC_ARG_MAX:
      return ARG_MAX;
    case _SC_ATEXIT_MAX:
      return ATEXIT_MAX;
    case _SC_CHILD_MAX:
      return CHILD_MAX;
    case _SC_DELAYTIMER_MAX:
      return DELAYTIMER_MAX;
    case _SC_HOST_NAME_MAX:
      return HOST_NAME_MAX;
    case _SC_IOV_MAX:
      return IOV_MAX;
    case _SC_LOGIN_NAME_MAX:
      return LOGIN_NAME_MAX;
    case _SC_MQ_OPEN_MAX:
      return MQ_OPEN_MAX;
    case _SC_MQ_PRIO_MAX:
      return MQ_PRIO_MAX;
    case _SC_OPEN_MAX:
      return OPEN_MAX;
    case _SC_PAGESIZE:
      return PAGESIZE;
    case _SC_PAGE_SIZE:
      return PAGE_SIZE;
    case _SC_THREAD_DESTRUCTOR_ITERATIONS:
      return PTHREAD_DESTRUCTOR_ITERATIONS;
    case _SC_THREAD_KEYS_MAX:
      return PTHREAD_KEYS_MAX;
    case _SC_THREAD_STACK_MIN:
      return PTHREAD_STACK_MIN;
    case _SC_THREAD_THREADS_MAX:
      return PTHREAD_THREADS_MAX;
    case _SC_RE_DUP_MAX:
      return RE_DUP_MAX;
    case _SC_RTSIG_MAX:
      return RTSIG_MAX;
    case _SC_SEM_NSEMS_MAX:
      return SEM_NSEMS_MAX;
    case _SC_SEM_VALUE_MAX:
      return SEM_VALUE_MAX;
    case _SC_SIGQUEUE_MAX:
      return SIGQUEUE_MAX;
    case _SC_SS_REPL_MAX:
      return SS_REPL_MAX;
    case _SC_STREAM_MAX:
      return STREAM_MAX;
    case _SC_SYMLOOP_MAX:
      return SYMLOOP_MAX;
    case _SC_TIMER_MAX:
      return TIMER_MAX;
    case _SC_TRACE_EVENT_NAME_MAX:
      return TRACE_EVENT_NAME_MAX;
    case _SC_TRACE_NAME_MAX:
      return TRACE_NAME_MAX;
    case _SC_TRACE_SYS_MAX:
      return TRACE_SYS_MAX;
    case _SC_TRACE_USER_EVENT_MAX:
      return TRACE_USER_EVENT_MAX;
    case _SC_TTY_NAME_MAX:
      return TTY_NAME_MAX;
    case _SC_TZNAME_MAX:
      return TZNAME_MAX;
    case _SC_BC_BASE_MAX:
      return BC_BASE_MAX;
    case _SC_BC_DIM_MAX:
      return BC_DIM_MAX;
    case _SC_BC_SCALE_MAX:
      return BC_SCALE_MAX;
    case _SC_BC_STRING_MAX:
      return BC_STRING_MAX;
    case _SC_COLL_WEIGHTS_MAX:
      return COLL_WEIGHTS_MAX;
    case _SC_EXPR_NEST_MAX:
      return EXPR_NEST_MAX;
    case _SC_LINE_MAX:
      return LINE_MAX;
    case _SC_NGROUPS_MAX:
      return NGROUPS_MAX;
    default:
      errno = EINVAL;
      return -1;
    }
}
