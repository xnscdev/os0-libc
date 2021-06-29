/* popen.c -- This file is part of OS/0 libc.
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
#include <branch.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct pid
{
  FILE *stream;
  pid_t pid;
  struct pid *next;
};

static struct pid *pidlist;

FILE *
popen (const char *cmd, const char *mode)
{
  struct pid *curr;
  FILE *stream;
  int fd[2];
  pid_t pid;
  if ((*mode != 'r' && *mode != 'w')
      || (mode[1] != '\0' && (mode[2] != '\0' || mode[1] != 'e')))
    {
      errno = EINVAL;
      return NULL;
    }
  curr = malloc (sizeof (struct pid));
  if (unlikely (curr == NULL))
    {
      errno = ENOMEM;
      return NULL;
    }
  if (pipe (fd) == -1)
    {
      free (curr);
      return NULL;
    }

  pid = fork ();
  if (pid == -1)
    {
      close (fd[0]);
      close (fd[1]);
      free (curr);
      return NULL;
    }
  else if (pid == 0)
    {
      if (*mode == 'r')
	{
	  if (fd[1] != STDOUT_FILENO)
	    {
	      dup2 (fd[1], STDOUT_FILENO);
	      close (fd[1]);
	    }
	  if (fd[0] != STDOUT_FILENO)
	    close (fd[0]);
	}
      else
	{
	  if (fd[0] != STDIN_FILENO)
	    {
	      dup2 (fd[0], STDIN_FILENO);
	      close (fd[0]);
	    }
	  close (fd[1]);
	}
      for (curr = pidlist; curr != NULL; curr = curr->next)
	close (fileno (curr->stream));
      execl ("/bin/sh", "sh", "-c", cmd, NULL);
      _exit (127);
    }

  if (*mode == 'r')
    {
      stream = fdopen (fd[0], mode);
      close (fd[1]);
    }
  else
    {
      stream = fdopen (fd[1], mode);
      close (fd[0]);
    }

  /* Set close-on-exec if specified in the mode */
  if (mode[1] == 'e')
    {
      int flags = fcntl (fileno (stream), F_GETFD);
      fcntl (fileno (stream), F_SETFD, flags | FD_CLOEXEC);
    }

  curr->stream = stream;
  curr->pid = pid;
  curr->next = pidlist;
  pidlist = curr;
  return stream;
}

int
pclose (FILE *stream)
{
  struct pid *curr;
  struct pid *last;
  int status;
  pid_t pid;

  fclose (stream);
  for (last = NULL, curr = pidlist; curr != NULL;
       last = curr, curr = curr->next)
    {
      if (curr->stream == stream)
	break;
    }
  if (unlikely (curr == NULL))
    return -1;

  /* Wait for process to terminate */
  do
    pid = waitpid (curr->pid, &status, 0);
  while (pid == -1 && errno == EINTR);

  if (last == NULL)
    pidlist = curr->next;
  else
    last->next = curr->next;
  free (curr);
  return pid == -1 ? -1 : status;
}
