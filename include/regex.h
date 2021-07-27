/* regex.h -- This file is part of OS/0 libc.
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

#ifndef _REGEX_H
#define _REGEX_H

#include <sys/types.h>
#include <wctype.h>

#define	REG_BASIC    0000
#define	REG_EXTENDED 0001
#define	REG_ICASE    0002
#define	REG_NOSUB    0004
#define	REG_NEWLINE  0010
#define	REG_NOSPEC   0020
#define	REG_PEND     0040
#define	REG_DUMP     0200

#define REG_NOTBOL   00001
#define REG_NOTEOL   00002
#define REG_STARTEND 00004
#define REG_TRACE    00400
#define REG_LARGE    01000
#define REG_BACKR    02000

#define REG_OKAY     0
#define REG_NOMATCH  1
#define REG_BADPAT   2
#define REG_ECOLLATE 3
#define REG_ECTYPE   4
#define REG_EESCAPE  5
#define REG_ESUBREG  6
#define REG_EBRACK   7
#define REG_EPAREN   8
#define REG_EBRACE   9
#define REG_BADBR    10
#define REG_ERANGE   11
#define REG_ESPACE   12
#define REG_BADRPT   13
#define REG_EMPTY    14
#define REG_ASSERT   15
#define REG_INVARG   16
#define REG_ATOI     255
#define REG_ITOA     256

typedef off_t regoff_t;

typedef struct
{
  int re_magic;
  size_t re_nsub;
  const char *re_endp;
  struct re_guts *re_g;
} regex_t;

typedef struct
{
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

__BEGIN_DECLS

int regcomp (regex_t *__restrict preg, const char *__restrict pattern,
	     int cflags);
int regexec (const regex_t *__restrict preg, const char *__restrict str,
	     size_t nmatch, regmatch_t pmatch[__restrict], int eflags);
size_t regerror (int errcode, const regex_t *__restrict preg,
		 char *__restrict errbuf, size_t errbuf_size);
void regfree (regex_t *preg);

__END_DECLS

#endif
