/* stdio.h -- This file is part of OS/0 libc.
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

#ifndef _STDIO_H
#define _STDIO_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stddef.h>

#define BUFSIZ 512

#include <bits/types/file.h>
#include <bits/types/fpos.h>

__BEGIN_DECLS

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

int printf (const char *__restrict fmt, ...);
int fprintf (FILE *__restrict stream, const char *__restrict fmt, ...);
int sprintf (char *__restrict str, const char *__restrict fmt, ...);
int snprintf (char *__restrict str, size_t size, const char *__restrict fmt,
	      ...);

int vprintf (const char *__restrict fmt, va_list args);
int vfprintf (FILE *__restrict stream, const char *__restrict fmt,
	      va_list args);
int vsprintf (char *__restrict str, const char *__restrict fmt, va_list args);
int vsnprintf (char *__restrict str, size_t size, const char *__restrict fmt,
	       va_list args);

FILE *fopen (const char *__restrict path, const char *__restrict mode);
FILE *fdopen (int fd, const char *mode);
FILE *freopen (const char *__restrict path, const char *__restrict mode,
	       FILE *stream);
FILE *fmemopen (void *__restrict *buffer, size_t size,
		const char *__restrict mode);
int fclose (FILE *stream);
size_t fread (void *__restrict buffer, size_t size, size_t len,
	      FILE *__restrict stream);
size_t fwrite (const void *__restrict buffer, size_t size, size_t len,
	       FILE *__restrict stream);
int fgetpos (FILE *__restrict stream, fpos_t *__restrict pos);
int fsetpos (FILE *__restrict stream, const fpos_t *__restrict pos);
int fseek (FILE *stream, long offset, int whence);
int fseeko (FILE *stream, off_t offset, int whence);
long ftell (FILE *stream);
off_t ftello (FILE *stream);
int fflush (FILE *stream);

void setbuf (FILE *__restrict stream, char *__restrict buffer);
int setvbuf (FILE *__restrict stream, char *__restrict buffer, int mode,
	     size_t size);

__END_DECLS

#endif
