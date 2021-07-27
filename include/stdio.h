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
#include <sys/unistd.h>
#include <stdarg.h>
#include <stddef.h>

#define EOF (-1)

#define BUFSIZ 512

/* tmpnam(3) macros */
#define TMP_MAX  1000000
#define P_tmpdir "/tmp"
#define L_tmpnam 20

/* Stream buffering modes */

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

#include <bits/errlist.h>

#ifndef __FILE_defined
typedef struct __FILE FILE;
#define __FILE_defined
#endif

#ifndef __wint_defined
typedef int wint_t;
#define __wint_defined
#endif

typedef off_t fpos_t;

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
int dprintf (int fd, const char *__restrict fmt, ...);
int asprintf (char **ret, const char *__restrict fmt, ...);

int vprintf (const char *__restrict fmt, va_list args);
int vfprintf (FILE *__restrict stream, const char *__restrict fmt,
	      va_list args);
int vsprintf (char *__restrict str, const char *__restrict fmt, va_list args);
int vsnprintf (char *__restrict str, size_t size, const char *__restrict fmt,
	       va_list args);
int vdprintf (int fd, const char *__restrict fmt, va_list args);
int vasprintf (char **ret, const char *__restrict fmt, va_list args);

void perror (const char *str);

int fputc (int c, FILE *stream);
int fputc_unlocked (int c, FILE *stream);
#define putc(c, stream) fputc (c, stream)
#define putc_unlocked(c, stream) fputc_unlocked (c, stream)
int putchar (int c);
int putchar_unlocked (int c);

wint_t fputwc (wchar_t wc, FILE *stream);
wint_t fputwc_unlocked (wchar_t wc, FILE *stream);
#define putwc(wc, stream) fputwc (c, stream);
wint_t putwchar (wchar_t wc);
wint_t putwchar_unlocked (wchar_t wc);

int fputs (const char *__restrict str, FILE *__restrict stream);
int fputs_unlocked (const char *__restrict str, FILE *__restrict stream);
int puts (const char *str);

int fputws (const wchar_t *__restrict ws, FILE *__restrict stream);
int fputws_unlocked (const wchar_t *__restrict ws, FILE *__restrict stream);

int scanf (const char *__restrict fmt, ...);
int fscanf (FILE *__restrict stream, const char *__restrict fmt, ...);
int sscanf (const char *__restrict str, const char *__restrict fmt, ...);

int vscanf (const char *__restrict fmt, va_list args);
int vfscanf (FILE *__restrict stream, const char *__restrict fmt, va_list args);
int vsscanf (const char *__restrict str, const char *__restrict fmt,
	     va_list args);

int fgetc (FILE *stream);
int fgetc_unlocked (FILE *stream);
#define getc(stream) fgetc (stream)
#define getc_unlocked(stream) fgetc_unlocked (stream)
int getchar (void);
int getchar_unlocked (void);
int ungetc (int c, FILE *stream);

wint_t fgetwc (FILE *stream);
wint_t fgetwc_unlocked (FILE *stream);
#define getwc(stream) fgetwc (stream)
#define getwc_unlocked(stream) fgetwc_unlocked (stream)
wint_t getwchar (void);
wint_t getwchar_unlocked (void);
wint_t ungetwc (wint_t wc, FILE *stream);

char *fgets (char *__restrict str, int size, FILE *__restrict stream);
char *fgets_unlocked (char *__restrict str, int size, FILE *__restrict stream);
char *gets (char *str);

wchar_t *fgetws (wchar_t *__restrict ws, int len, FILE *__restrict stream);
wchar_t *fgetws_unlocked (wchar_t *__restrict ws, int len,
			  FILE *__restrict stream);

FILE *fopen (const char *__restrict path, const char *__restrict mode);
FILE *fdopen (int fd, const char *mode);
FILE *freopen (const char *__restrict path, const char *__restrict mode,
	       FILE *stream);
FILE *fmemopen (void *__restrict *buffer, size_t size,
		const char *__restrict mode);
int fclose (FILE *stream);

size_t fread (void *__restrict buffer, size_t size, size_t len,
	      FILE *__restrict stream);
size_t fread_unlocked (void *__restrict buffer, size_t size, size_t len,
		       FILE *__restrict stream);
size_t fwrite (const void *__restrict buffer, size_t size, size_t len,
	       FILE *__restrict stream);
size_t fwrite_unlocked (const void *__restrict buffer, size_t size, size_t len,
			FILE *__restrict stream);

void clearerr (FILE *stream);
void clearerr_unlocked (FILE *stream);
int feof (FILE *stream);
#define feof_unlocked(stream) feof (stream)
int ferror (FILE *stream);
#define ferror_unlocked(stream) ferror (stream)
int fileno (FILE *stream);
#define fileno_unlocked(stream) fileno (stream)

int fgetpos (FILE *__restrict stream, fpos_t *__restrict pos);
int fsetpos (FILE *__restrict stream, const fpos_t *__restrict pos);
int fseek (FILE *stream, long offset, int whence);
long ftell (FILE *stream);
void rewind (FILE *stream);

int fseeko (FILE *stream, off_t offset, int whence);
off_t ftello (FILE *stream);

int fseeko64 (FILE *stream, off64_t offset, int whence);
off64_t ftello64 (FILE *stream);

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define fseeko(stream, offset, whence) fseeko64 (stream, offset, whence)
#define ftello(stream) ftello64 (stream)
#endif

int fflush (FILE *stream);
int fpurge (FILE *stream);

int fwide (FILE *stream, int mode);

void flockfile (FILE *stream);
int ftrylockfile (FILE *stream);
void funlockfile (FILE *stream);

void setbuf (FILE *__restrict stream, char *__restrict buffer);
void setbuffer (FILE *__restrict stream, char *__restrict buffer, size_t size);
void setlinebuf (FILE *stream);
int setvbuf (FILE *__restrict stream, char *__restrict buffer, int mode,
	     size_t size);

FILE *popen (const char *cmd, const char *mode);
int pclose (FILE *stream);

ssize_t getdelim (char **__restrict line, size_t *__restrict len, int delim,
		  FILE *__restrict stream);
ssize_t getline (char **__restrict line, size_t *__restrict len,
		 FILE *__restrict stream);

char *tmpnam (char *str);
char *tmpnam_r (char *str);
FILE *tmpfile (void);

__END_DECLS

#endif
