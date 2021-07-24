/* fts.h -- This file is part of OS/0 libc.
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

/* File tree traversal functions declarations.
   Copyright (C) 1994-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)fts.h	8.3 (Berkeley) 8/14/94
 */

#ifndef _FTS_H
#define _FTS_H

#include <sys/cdefs.h>
#include <sys/types.h>

#define FTS_COMFOLLOW  0x0001
#define FTS_LOGICAL    0x0002
#define FTS_NOCHDIR    0x0004
#define FTS_NOSTAT     0x0008
#define FTS_PHYSICAL   0x0010
#define FTS_SEEDOT     0x0020
#define FTS_XDEV       0x0040
#define FTS_WHITEOUT   0x0080
#define FTS_OPTIONMASK 0x00ff

#define FTS_NAMEONLY   0x0100
#define FTS_STOP       0x0200

#define FTS_ROOTPARENTLEVEL -1
#define FTS_ROOTLEVEL       0

#define FTS_D       1
#define FTS_DC      2
#define FTS_DEFAULT 3
#define FTS_DNR     4
#define FTS_DOT     5
#define FTS_DP      6
#define FTS_ERR     7
#define FTS_F       8
#define FTS_INIT    9
#define FTS_NS      10
#define FTS_NSOK    11
#define FTS_SL      12
#define FTS_SLNONE  13
#define FTS_W       14

#define FTS_DONTCHDIR 0x01
#define FTS_SYMFOLLOW 0x02

#define FTS_AGAIN   1
#define FTS_FOLLOW  2
#define FTS_NOINSTR 3
#define FTS_SKIP    4

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
typedef struct _ftsent64 FTSENT;
#else
typedef struct _ftsent FTSENT;
#endif
typedef struct _ftsent64 FTSENT64;

typedef struct
{
  struct _ftsent *fts_cur;
  struct _ftsent *fts_child;
  struct _ftsent **fts_array;
  dev_t fts_dev;
  char *fts_path;
  int fts_rfd;
  int fts_pathlen;
  int fts_nitems;
  int (*fts_compar) (const void *, const void *);
  int fts_options;
} FTS;

typedef struct
{
  struct _ftsent64 *fts_cur;
  struct _ftsent64 *fts_child;
  struct _ftsent64 **fts_array;
  dev_t fts_dev;
  char *fts_path;
  int fts_rfd;
  int fts_pathlen;
  int fts_nitems;
  int (*fts_compar) (const void *, const void *);
  int fts_options;
} FTS64;

struct _ftsent
{
  struct _ftsent *fts_cycle;
  struct _ftsent *fts_parent;
  struct _ftsent *fts_link;
  long fts_number;
  void *fts_pointer;
  char *fts_accpath;
  char *fts_path;
  int fts_errno;
  int fts_symfd;
  unsigned short fts_pathlen;
  unsigned short fts_namelen;
  ino_t fts_ino;
  dev_t fts_dev;
  nlink_t fts_nlink;
  short fts_level;
  unsigned short fts_info;
  unsigned short fts_flags;
  unsigned short fts_instr;
  struct stat *fts_statp;
  char fts_name[1];
};

struct _ftsent64
{
  struct _ftsent64 *fts_cycle;
  struct _ftsent64 *fts_parent;
  struct _ftsent64 *fts_link;
  long fts_number;
  void *fts_pointer;
  char *fts_accpath;
  char *fts_path;
  int fts_errno;
  int fts_symfd;
  unsigned short fts_pathlen;
  unsigned short fts_namelen;
  ino64_t fts_ino;
  dev_t fts_dev;
  nlink_t fts_nlink;
  short fts_level;
  unsigned short fts_info;
  unsigned short fts_flags;
  unsigned short fts_instr;
  struct stat64 *fts_statp;
  char fts_name[1];
};

__BEGIN_DECLS

FTS *fts_open (char *const *argv, int instr,
	       int (*compare) (const FTSENT **, const FTSENT **));
int fts_close (FTS *ftsp);
FTSENT *fts_read (FTS *ftsp);
FTSENT *fts_children (FTS *ftsp, int instr);
int fts_set (FTS *ftsp, FTSENT *ptr, int instr);

FTS64 *fts64_open (char *const *argv, int instr,
		   int (*compare) (const FTSENT64 **, const FTSENT64 **));
int fts64_close (FTS64 *ftsp);
FTSENT64 *fts64_read (FTS64 *ftsp);
FTSENT64 *fts64_children (FTS64 *ftsp, int instr);
int fts64_set (FTS64 *ftsp, FTSENT64 *ptr, int instr);

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define FTS FTS64
#define FTSENT FTSENT64
#define fts_open(argv, instr, compare) fts64_open (argv, instr, compare)
#define fts_close(ftsp) fts64_close (ftsp)
#define fts_read(ftsp) fts64_read (ftsp)
#define fts_children(ftsp, instr) fts64_children (ftsp, instr)
#define fts_set(ftsp, ptr, instr) fts64_set (ftsp, ptr, instr)
#endif

__END_DECLS

#endif
