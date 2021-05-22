/* wait.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <bits/wait.h>
#include <sys/resource.h>

__BEGIN_DECLS

pid_t wait (int *status);
pid_t wait3 (int *status, int options, struct rusage *usage);
pid_t wait4 (pid_t pid, int *status, int options, struct rusage *usage);
pid_t waitpid (pid_t pid, int *status, int options);

__END_DECLS

#endif
