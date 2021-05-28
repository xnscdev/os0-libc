/* fputc.c -- This file is part of OS/0 libc.
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
#include <stream.h>
#include <unistd.h>

int
fputc (int c, FILE *stream)
{
  char ch = c;
  if ((stream->_flags & __IO_buf_mask) == _IONBF)
    return write (stream->_fd, &ch, 1);
  if (stream->_write_ptr_len >= stream->_write_buf_len)
    {
      if (fflush (stream) == EOF)
	return EOF;
    }
  stream->_write_buf[stream->_write_ptr_len++] = ch;
  if (ch == '\n' && (stream->_flags & __IO_buf_mask) == _IOLBF)
    return fflush (stream);
  return 0;
}

int
putchar (int c)
{
  return fputc (c, stdout);
}
