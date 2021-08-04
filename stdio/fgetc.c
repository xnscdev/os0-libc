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

#include <branch.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <unistd.h>

int
__ungetc_unlocked (int c, FILE *stream)
{
  if (c == EOF || (stream->_read_buf != NULL
		   && stream->_read_ptr_len >= stream->_read_buf_len))
    return EOF;
  if (stream->_read_buf == NULL)
    {
      stream->_read_buf = malloc (BUFSIZ);
      if (unlikely (stream->_read_buf == NULL))
	{
	  errno = ENOMEM;
	  return EOF;
	}
      stream->_read_buf_len = BUFSIZ;
      stream->_flags |= __IO_rbuf_alloc;
    }
  stream->_read_buf[stream->_read_ptr_len++] = c;
  stream->_flags |= __IO_orient;
  stream->_flags &= ~(__IO_eof | __IO_wide);
  return c;
}

int
fgetc (FILE *stream)
{
  int ret;
  flockfile (stream);
  ret = fgetc_unlocked (stream);
  funlockfile (stream);
  return ret;
}

int
fgetc_unlocked (FILE *stream)
{
  char c;
  int ret;
  if (feof (stream) || ferror (stream))
    return EOF;
  if (stream->_read_ptr_len > 0)
    return stream->_read_buf[--stream->_read_ptr_len];
  ret = read (stream->_fd, &c, 1);
  stream->_flags |= __IO_orient;
  stream->_flags &= ~__IO_wide;
  if (ret == -1)
    {
      stream->_flags |= __IO_err;
      return EOF;
    }
  else if (ret == 0)
    {
      stream->_flags |= __IO_eof;
      return EOF;
    }
  return c;
}

int
getchar (void)
{
  return fgetc (stdin);
}

int
getchar_unlocked (void)
{
  return fgetc_unlocked (stdin);
}

int
ungetc (int c, FILE *stream)
{
  int ret;
  flockfile (stream);
  ret = __ungetc_unlocked (c, stream);
  funlockfile (stream);
  return ret;
}
