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

#define REG_BASIC    0x00
#define REG_EXTENDED 0x01
#define REG_ICASE    0x02
#define REG_NOSUB    0x04
#define REG_NEWLINE  0x08

#define REG_NOMATCH  1
#define REG_BADPAT   2
#define REG_ECOLLATE 3
#define REG_ECTYPE   4
#define REG_EESCAPE  5
#define REG_ESUBREG  6
#define REG_EBRACK   7
#define REG_ENOSYS   8
#define REG_EPAREN   9
#define REG_EBRACE   10
#define REG_BADBR    11
#define REG_ERANGE   12
#define REG_ESPACE   13
#define REG_BADRPT   14

typedef ssize_t regoff_t;

typedef enum
{
  __RE_CHARS,
  __RE_ANY,
  __RE_START,
  __RE_END,
  __RE_SUBEXPR
} __re_node_type;

struct __re_chars
{
  wctype_t wctype;
  int min;
  int max;
};

struct __re_regex
{
  size_t count;
  struct __re_node *nodes;
};

struct __re_node
{
  __re_node_type type;
  size_t nitems;
  int exclude;
  struct __re_chars *chars;
  struct __re_regex *subexpr;
  size_t min_rep;
  size_t max_rep;
  int can_rep;
};

typedef struct
{
  size_t re_nsub;
  int __re_cflags;
  struct __re_regex __re_regex;
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
