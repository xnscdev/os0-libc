/* signal.h -- This file is part of OS/0 libc.
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

#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>
#include <sys/signal.h>

typedef int sig_atomic_t;

__BEGIN_DECLS

int kill (pid_t pid, int sig);
int raise (int sig);

int sigaction (int sig, const struct sigaction *__restrict act,
	       struct sigaction *__restrict old);
sighandler_t signal (int sig, sighandler_t func);
int sigprocmask (int how, const sigset_t *__restrict set,
		 sigset_t *__restrict old);

int sigemptyset (sigset_t *set);
int sigfillset (sigset_t *set);
int sigaddset (sigset_t *set, int sig);
int sigdelset (sigset_t *set, int sig);
int sigismember (const sigset_t *set, int sig);

__END_DECLS

#endif
