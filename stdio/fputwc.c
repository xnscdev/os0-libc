/* fputwc.c -- This file is part of OS/0 libc.
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

#include <libc-locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <string.h>
#include <symbol.h>
#include <unistd.h>
#include <wchar.h>

wint_t
fputwc (wchar_t wc, FILE *stream)
{
  wint_t ret;
  flockfile (stream);
  ret = fputwc_unlocked (wc, stream);
  funlockfile (stream);
  return ret;
}

wint_t
fputwc_unlocked (wchar_t wc, FILE *stream)
{
  char buffer[MB_LEN_MAX];
  size_t len;
  memset (&__libc_mbstate, 0, sizeof (mbstate_t));
  len = wctomb (buffer, wc);
  if (len == (size_t) -1)
    return WEOF;
  if ((stream->_flags & __IO_buf_mask) == _IONBF)
    {
      int ret = write (stream->_fd, buffer, len);
      if (ret != len)
	return WEOF;
      stream->_flags |= __IO_orient | __IO_wide;
      return wc;
    }
  if (stream->_write_ptr_len + len > stream->_write_buf_len)
    {
      if (fflush (stream) == EOF)
	return WEOF;
    }
  memcpy (stream->_write_buf + stream->_write_ptr_len, buffer, len);
  stream->_write_ptr_len += len;
  if (wc == '\n' && (stream->_flags & __IO_buf_mask) == _IOLBF)
    {
      if (fflush (stream) == EOF)
	return WEOF;
    }
  stream->_flags |= __IO_orient | __IO_wide;
  return wc;
}

wint_t
putwchar (wchar_t wc)
{
  return fputwc (wc, stdout);
}

int
putwchar_unlocked (wchar_t wc)
{
  return fputwc_unlocked (wc, stdout);
}

strong_alias (fputwc, putwc);
