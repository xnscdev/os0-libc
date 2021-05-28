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

static FILE __stdin;
static FILE __stdout;
static FILE __stderr;

static char __stdout_buf[BUFSIZ];

FILE *stdin = &__stdin;
FILE *stdout = &__stdout;
FILE *stderr = &__stderr;

void
__libc_setup_stdstr (void)
{
  stdin->_flags = _IONBF;
  stdin->_fd = STDIN_FILENO;

  stdout->_flags = _IOLBF;
  stdout->_fd = STDOUT_FILENO;
  stdout->_write_buf = __stdout_buf;
  stdout->_write_buf_len = BUFSIZ;
  stdout->_write_ptr_len = 0;

  stderr->_flags = _IONBF;
  stderr->_fd = STDERR_FILENO;
}
