/* types.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <stdint.h>

/* POSIX types */

typedef int32_t blkcnt_t;
typedef int32_t blksize_t;
typedef int32_t clock_t;
typedef int32_t clockid_t;
typedef uint16_t dev_t;
typedef uint32_t fsblkcnt_t;
typedef uint32_t fsfilcnt_t;
typedef uint16_t gid_t;
typedef uint16_t id_t;
typedef uint32_t ino_t;
typedef int32_t key_t;
typedef uint32_t mode_t;
typedef uint32_t nlink_t;
typedef int32_t off_t;
typedef int64_t loff_t;
typedef int16_t pid_t;
typedef int32_t time_t; /* Year 2038 problem */
typedef int32_t timer_t;
typedef uint16_t uid_t;

#endif
