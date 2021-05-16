/* stdstr.c -- This file is part of OS/0 libc.
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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <unistd.h>

FILE *stdin;
FILE *stdout;
FILE *stderr;

void
__libc_setup_stdstr (void)
{
  stdin = calloc (1, sizeof (FILE));
  if (stdin == NULL)
    raise (SIGSEGV);
  stdin->_flags = _IONBF;
  stdin->_fd = STDIN_FILENO;

  stdout = malloc (sizeof (FILE));
  if (stdout == NULL)
    raise (SIGSEGV);
  stdout->_flags = _IOLBF | __IO_buf_alloc;
  stdout->_fd = STDOUT_FILENO;
  stdout->_buffer = malloc (BUFSIZ);
  if (stdout->_buffer == NULL)
    raise (SIGSEGV);
  stdout->_ptr = stdout->_buffer;
  stdout->_buf_len = BUFSIZ;
  stdout->_ptr_len = 0;

  stderr = calloc (1, sizeof (FILE));
  if (stderr == NULL)
    raise (SIGSEGV);
  stderr->_flags = _IONBF;
  stderr->_fd = STDERR_FILENO;
}
