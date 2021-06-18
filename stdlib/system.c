/* system.c -- This file is part of OS/0 libc.
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

#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int
system (const char *cmd)
{
  char *argv[4] = {"sh", "-c", (char *) cmd, NULL};
  pid_t pid;

  if (cmd == NULL)
    return getenv ("SHELL") != NULL;

  pid = fork ();
  if (pid == -1)
    return -1;
  else if (pid == 0)
    {
      execv ("/bin/sh", argv);
      exit (127);
    }
  else
    {
      int status;
      sigset_t save_sigset;
      sigset_t sigset;
      sig_t int_handler;
      sig_t quit_handler;
      int ret;

      /* Block SIGCHLD, ignore SIGINT and SIGQUIT */
      sigemptyset (&sigset);
      sigaddset (&sigset, SIGCHLD);
      sigprocmask (SIG_BLOCK, &sigset, &save_sigset);
      int_handler = signal (SIGINT, SIG_IGN);
      quit_handler = signal (SIGQUIT, SIG_IGN);

      /* Wait for child to terminate */
      ret = waitpid (pid, &status, 0);

      /* Restore signal mask and handlers */
      sigprocmask (SIG_SETMASK, &save_sigset, NULL);
      signal (SIGINT, int_handler);
      signal (SIGQUIT, quit_handler);

      /* Return exit status of child process */
      return ret == -1 ? -1 : WEXITSTATUS (status);
    }
}
