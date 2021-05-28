/* fopen.c -- This file is part of OS/0 libc.
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <unistd.h>

FILE *
fopen (const char *__restrict path, const char *__restrict mode)
{
  FILE *stream;
  int cloexec = 0;
  int flags;
  int fd;

  /* Parse mode string */
  switch (*mode++)
    {
    case 'r':
      flags = O_RDONLY;
      break;
    case 'w':
      flags = O_WRONLY;
      break;
    case 'a':
      flags = O_RDONLY | O_APPEND;
      break;
    default:
      errno = EINVAL;
      return NULL;
    }

  while (1)
    {
      switch (*mode++)
	{
	case '+':
	  flags &= ~O_ACCMODE;
	  flags |= O_RDWR;
	  break;
	case 'b':
	  break;
	case 'e':
	  cloexec = 1;
	  break;
	case 'x':
	  if ((flags & O_ACCMODE) == O_RDONLY)
	    {
	      errno = EINVAL;
	      return NULL;
	    }
	  flags |= O_EXCL;
	  break;
	case '\0':
	  goto start;
	default:
	  errno = EINVAL;
	  return NULL;
	}
    }

 start:
  fd = open (path, flags);
  if (fd == -1)
    return NULL;
  if (cloexec)
    {
      if (fcntl (fd, F_SETFD, FD_CLOEXEC) == -1)
	{
	  close (fd);
	  return NULL;
	}
    }

  stream = malloc (sizeof (FILE));
  if (stream == NULL)
    {
      close (fd);
      return NULL;
    }
  stream->_flags = _IOFBF | __IO_wbuf_alloc | __IO_stt_alloc;
  stream->_fd = fd;
  stream->_write_buf = malloc (BUFSIZ);
  if (stream->_write_buf == NULL)
    {
      close (fd);
      free (stream);
      return NULL;
    }
  stream->_write_buf_len = BUFSIZ;
  stream->_write_ptr_len = 0;
  return stream;
}

FILE *
fdopen (int fd, const char *mode)
{
  FILE *stream;
  int cloexec = 0;
  int flags;

  /* Parse mode string */
  switch (*mode++)
    {
    case 'r':
      flags = O_RDONLY;
      break;
    case 'w':
      flags = O_WRONLY;
      break;
    case 'a':
      flags = O_RDONLY | O_APPEND;
      break;
    default:
      errno = EINVAL;
      return NULL;
    }

  while (1)
    {
      switch (*mode++)
	{
	case '+':
	  flags &= ~O_ACCMODE;
	  flags |= O_RDWR;
	  break;
	case 'b':
	case 'x':
	  break;
	case 'e':
	  cloexec = 1;
	  break;
	case '\0':
	  goto start;
	default:
	  errno = EINVAL;
	  return NULL;
	}
    }

 start:
  if (cloexec)
    {
      if (fcntl (fd, F_SETFD, FD_CLOEXEC) == -1)
	return NULL;
    }

  stream = malloc (sizeof (FILE));
  if (stream == NULL)
    return NULL;
  stream->_flags = _IOFBF | __IO_wbuf_alloc | __IO_stt_alloc;
  stream->_fd = fd;
  stream->_write_buf = malloc (BUFSIZ);
  if (stream->_write_buf == NULL)
    {
      free (stream);
      errno = ENOMEM;
      return NULL;
    }
  stream->_write_buf_len = BUFSIZ;
  stream->_write_ptr_len = 0;
  return stream;
}

FILE *
freopen (const char *__restrict path, const char *__restrict mode,
	 FILE *stream)
{
  errno = ENOSYS;
  return NULL;
}

FILE *
fmemopen (void *__restrict *buffer, size_t size, const char *__restrict mode)
{
  errno = ENOSYS;
  return NULL;
}

int
fclose (FILE *stream)
{
  if (close (stream->_fd) == -1)
    return EOF;
  if (stream->_flags & __IO_rbuf_alloc)
    free (stream->_read_buf);
  if (stream->_flags & __IO_wbuf_alloc)
    free (stream->_write_buf);
  if (stream->_flags & __IO_stt_alloc)
    free (stream);
  return 0;
}
