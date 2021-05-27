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

#include <sys/cdefs.h>
#include <stdarg.h>
#include <stddef.h>

#define __IO_buf_mask  0x03
#define __IO_buf_alloc 0x04
#define __IO_eof       0x08
#define __IO_err       0x10
#define __IO_stt_alloc 0x20

__BEGIN_DECLS

void __libc_setup_stdstr (void) __hidden;

__END_DECLS

#endif
