/* fgetwc.c -- This file is part of OS/0 libc.
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
#include <libc-locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

wint_t
__ungetwc_unlocked (wint_t wc, FILE *stream)
{
  if (wc == WEOF
      || (stream->_read_buf != NULL
	  && stream->_read_ptr_len + sizeof (wint_t) > stream->_read_buf_len))
    return WEOF;
  if (stream->_read_buf == NULL)
    {
      stream->_read_buf = malloc (BUFSIZ);
      if (unlikely (stream->_read_buf == NULL))
	{
	  errno = ENOMEM;
	  return WEOF;
	}
      stream->_read_buf_len = BUFSIZ;
      stream->_flags |= __IO_rbuf_alloc;
    }
  memcpy (stream->_read_buf + stream->_read_ptr_len, &wc, sizeof (wint_t));
  stream->_read_ptr_len += sizeof (wint_t);
  stream->_flags |= __IO_orient | __IO_wide;
  stream->_flags &= ~__IO_eof;
  return wc;
}

wint_t
fgetwc (FILE *stream)
{
  wint_t ret;
  flockfile (stream);
  ret = fgetwc_unlocked (stream);
  funlockfile (stream);
  return ret;
}

wint_t
fgetwc_unlocked (FILE *stream)
{
  char buffer[MB_CUR_MAX];
  wint_t wc;
  size_t n;
  if (feof (stream) || ferror (stream))
    return WEOF;
  if (stream->_read_ptr_len >= sizeof (wint_t))
    {
      stream->_read_ptr_len -= sizeof (wint_t);
      memcpy (&wc, stream->_read_buf + stream->_read_ptr_len, sizeof (wint_t));
      return wc;
    }
  for (n = 1; n <= MB_CUR_MAX; n++)
    {
      size_t len;
      buffer[n - 1] = fgetc_unlocked (stream);
      if (buffer[n - 1] == EOF)
	return WEOF;
      memset (&__libc_mbstate, 0, sizeof (mbstate_t));
      len = mbrtowc (&wc, buffer, n, &__libc_mbstate);
      switch (len)
	{
	case 0:
	  return L'\0';
	case (size_t) -1:
	  return WEOF;
	case (size_t) -2:
	  continue;
	default:
	  stream->_flags |= __IO_orient | __IO_wide;
	  return wc;
	}
    }
  errno = EILSEQ;
  return WEOF;
}

wint_t
getwchar (void)
{
  return fgetwc (stdin);
}

wint_t
getwchar_unlocked (void)
{
  return fgetwc_unlocked (stdin);
}

wint_t
ungetwc (wint_t wc, FILE *stream)
{
  wint_t ret;
  flockfile (stream);
  ret = __ungetwc_unlocked (wc, stream);
  funlockfile (stream);
  return ret;
}
