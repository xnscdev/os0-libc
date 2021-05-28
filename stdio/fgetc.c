/* fgetc.c -- This file is part of OS/0 libc.
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

#include <stdio.h>
#include <unistd.h>

int
fgetc (FILE *stream)
{
  char c;
  if (feof (stream) || ferror (stream))
    return EOF;
  if (stream->_read_buf_len > 0)
    return stream->_read_buf[--stream->_read_buf_len];
  return read (stream->_fd, &c, 1);
}

int
getchar (void)
{
  return fgetc (stdin);
}

int
ungetc (int c, FILE *stream)
{
  if (c == EOF || stream->_read_buf == NULL
      || stream->_read_ptr_len >= stream->_read_buf_len)
    return EOF;
  stream->_read_buf[stream->_read_ptr_len++] = c;
  return c;
}
