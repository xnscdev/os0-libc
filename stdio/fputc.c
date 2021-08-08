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
#include <symbol.h>
#include <unistd.h>

int
fputc (int c, FILE *stream)
{
  int ret;
  flockfile (stream);
  ret = fputc_unlocked (c, stream);
  funlockfile (stream);
  return ret;
}

int
fputc_unlocked (int c, FILE *stream)
{
  unsigned char ch = c;
  if ((stream->_flags & __IO_buf_mask) == _IONBF)
    {
      int ret = write (stream->_fd, &ch, 1);
      if (ret != 1)
	return EOF;
      stream->_flags |= __IO_orient;
      stream->_flags &= ~__IO_wide;
      return ch;
    }
  if (stream->_write_ptr_len >= stream->_write_buf_len)
    {
      if (fflush (stream) == EOF)
	return EOF;
    }
  stream->_write_buf[stream->_write_ptr_len++] = ch;
  if (ch == '\n' && (stream->_flags & __IO_buf_mask) == _IOLBF)
    {
      if (fflush (stream) == EOF)
	return EOF;
    }
  stream->_flags |= __IO_orient;
  stream->_flags &= ~__IO_wide;
  return ch;
}

int
putchar (int c)
{
  return fputc (c, stdout);
}

int
putchar_unlocked (int c)
{
  return fputc_unlocked (c, stdout);
}

strong_alias (fputc, putc);
strong_alias (fputc_unlocked, putc_unlocked);
