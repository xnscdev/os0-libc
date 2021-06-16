/* fio.c -- This file is part of OS/0 libc.
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

size_t
fread (void *__restrict buffer, size_t size, size_t len,
       FILE *__restrict stream)
{
  size_t ret;
  flockfile (stream);
  ret = fread_unlocked (buffer, size, len, stream);
  funlockfile (stream);
  return ret;
}

size_t
fread_unlocked (void *__restrict buffer, size_t size, size_t len,
		FILE *__restrict stream)
{
  size_t i;
  for (i = 0; i < len; i++)
    {
      if (read (stream->_fd, buffer + i * size, size) == -1)
	{
	  stream->_flags |= __IO_err;
	  return i;
	}
    }
  return i;
}

size_t
fwrite (const void *__restrict buffer, size_t size, size_t len,
	FILE *__restrict stream)
{
  size_t ret;
  flockfile (stream);
  ret = fwrite_unlocked (buffer, size, len, stream);
  funlockfile (stream);
  return ret;
}

size_t
fwrite_unlocked (const void *__restrict buffer, size_t size, size_t len,
		 FILE *__restrict stream)
{
  size_t i;
  for (i = 0; i < len; i++)
    {
      size_t j;
      for (j = 0; j < size; j++)
	{
	  unsigned char c = ((unsigned char *) buffer)[i * size + j];
	  if (fputc (c, stream) == EOF)
	    {
	      stream->_flags |= __IO_err;
	      return i;
	    }
	}
    }
  return i;
}
