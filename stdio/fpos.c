/* fpos.c -- This file is part of OS/0 libc.
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
fgetpos (FILE *__restrict stream, fpos_t *__restrict pos)
{
  off_t ret = ftello (stream);
  if (ret == -1)
    return -1;
  *pos = ret;
  return 0;
}

int
fsetpos (FILE *__restrict stream, const fpos_t *__restrict pos)
{
  return fseeko (stream, *pos, SEEK_SET);
}

int
fseek (FILE *stream, long offset, int whence)
{
  return fseeko (stream, offset, whence);
}

long
ftell (FILE *stream)
{
  return (long) ftello (stream);
}

int
fseeko (FILE *stream, off_t offset, int whence)
{
  off_t ret = lseek (stream->_fd, offset, whence);
  if (ret == -1)
    return -1;
  stream->_flags &= ~__IO_eof;
  stream->_read_buf_len = 0;
  return 0;
}

off_t
ftello (FILE *stream)
{
  return lseek (stream->_fd, 0, SEEK_CUR);
}

void
rewind (FILE *stream)
{
  fseek (stream, 0, SEEK_SET);
}

int
fseeko64 (FILE *stream, off64_t offset, int whence)
{
  off_t ret = lseek64 (stream->_fd, offset, whence);
  if (ret == -1)
    return -1;
  stream->_flags &= ~__IO_eof;
  stream->_read_buf_len = 0;
  return 0;
}

off64_t
ftello64 (FILE *stream)
{
  return lseek64 (stream->_fd, 0, SEEK_CUR);
}
