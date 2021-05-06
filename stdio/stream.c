/* stream.c -- This file is part of OS/0 libc.
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

#include <branch.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>

FILE *stdin;
FILE *stdout;
FILE *stderr;

FILE *
create_stream (int bufsize)
{
  FILE *stream = malloc (sizeof (FILE));
  stream->_flags = 0;
  stream->_buffer = calloc (bufsize, 1);
  if (unlikely (stream->_buffer == NULL))
    {
      free (stream);
      return NULL;
    }
  stream->_ptr = stream->_buffer;
  stream->_buf_len = bufsize;
  stream->_read_len = 0;
  stream->_write_len = 0;
  return stream;
}
