/* err.h -- This file is part of OS/0 libc.
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

#ifndef _ERR_H
#define _ERR_H

#include <sys/cdefs.h>
#include <stdarg.h>

__BEGIN_DECLS

void vwarn (const char *fmt, va_list args);
void vwarnx (const char *fmt, va_list args);
void verr (int status, const char *fmt, va_list args)
  __attribute__ ((noreturn));
void verrx (int status, const char *fmt, va_list args)
  __attribute__ ((noreturn));
void warn (const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void warnx (const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void err (int status, const char *fmt, ...)
  __attribute__ ((noreturn, format (printf, 2, 3)));
void errx (int status, const char *fmt, ...)
  __attribute__ ((noreturn, format (printf, 2, 3)));

__END_DECLS

#endif
