/* stream.h -- This file is part of OS/0 libc.
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

#ifndef _STREAM_H
#define _STREAM_H

#include <locks.h>

#define __IO_buf_mask   0x0003 /* Mask for stream mode */
#define __IO_rbuf_alloc 0x0004 /* If the read buffer was allocated */
#define __IO_wbuf_alloc 0x0008 /* If the write buffer was allocated */
#define __IO_eof        0x0010 /* If the stream is at the end of input */
#define __IO_err        0x0020 /* If an error occurred */
#define __IO_stt_alloc  0x0040 /* If the FILE structure itself was allocated */
#define __IO_orient     0x0080 /* If stream orientation has been determined */
#define __IO_wide       0x0100 /* If stream orientation is wide */

struct __FILE
{
  int _flags;
  int _fd;
  __lock_t _lock;
  char *_read_buf;
  int _read_buf_len;
  int _read_ptr_len;
  char *_write_buf;
  int _write_buf_len;
  int _write_ptr_len;
};

typedef struct __FILE FILE;

__BEGIN_DECLS

int __ungetc_unlocked (int c, FILE *stream) __hidden;
wint_t __ungetwc_unlocked (wint_t wc, FILE *stream) __hidden;

__END_DECLS

#endif
