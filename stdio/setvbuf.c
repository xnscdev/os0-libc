/* setvbuf.c -- This file is part of OS/0 libc.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>

void
setbuf (FILE *__restrict stream, char *__restrict buffer)
{
  if (buffer == NULL)
    setvbuf (stream, NULL, _IONBF, 0);
  else
    setvbuf (stream, buffer, _IOFBF, BUFSIZ);
}

void
setbuffer (FILE *__restrict stream, char *__restrict buffer, size_t size)
{
  if (buffer == NULL)
    setvbuf (stream, NULL, _IONBF, 0);
  else
    setvbuf (stream, buffer, _IOFBF, size);
}

void
setlinebuf (FILE *stream)
{
  char *buffer;
  if (stream->_flags & __IO_buf_alloc)
    free (stream->_buffer);
  buffer = malloc (BUFSIZ);
  if (buffer == NULL)
    {
      errno = ENOMEM;
      return;
    }
  stream->_flags &= ~__IO_buf_mask;
  stream->_flags |= _IOLBF | __IO_buf_alloc;
  stream->_buffer = buffer;
  stream->_ptr = buffer;
  stream->_buf_len = BUFSIZ;
  stream->_ptr_len = 0;
}

int
setvbuf (FILE *__restrict stream, char *__restrict buffer, int mode,
	 size_t size)
{
  mode &= __IO_buf_mask;
  if (mode != _IONBF && buffer == NULL && size > 0)
    {
      buffer = malloc (size);
      if (buffer == NULL)
	{
	  errno = ENOMEM;
	  return -1;
	}
    }
  if (stream->_flags & __IO_buf_alloc)
    {
      free (stream->_buffer);
      stream->_flags &= ~__IO_buf_alloc;
    }
  stream->_flags &= ~__IO_buf_mask;
  stream->_flags |= mode;
  stream->_buffer = buffer;
  stream->_ptr = buffer;
  stream->_buf_len = size;
  stream->_ptr_len = 0;
  return 0;
}
