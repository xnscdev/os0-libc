/* fts.c -- This file is part of OS/0 libc.
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

/* File tree traversal functions.
   Copyright (C) 1994-2019 Free Software Foundation, Inc.
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
/*-
 * Copyright (c) 1990, 1993, 1994
 *        The Regents of the University of California.  All rights reserved.
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
 */

#include <sys/param.h>
#include <sys/stat.h>
#include <branch.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef STAT
#define STAT stat
#endif

#ifndef ALIGNBYTES
#define ALIGNBYTES (__alignof__ (long double) - 1)
#endif

#ifndef ALIGN
#define ALIGN(p) (((unsigned long) (p) + ALIGNBYTES) & ~ALIGNBYTES)
#endif

#define ISDOT(c)							\
  (c[0] == '.' && (c[1] == '\0' || (c[1] == '.' && c[2] == '\0')))

#define FCHDIR(ftsp, fd) (!(ftsp->fts_options & FTS_NOCHDIR) && fchdir (fd))

#define NAPPEND(ptr) (ptr->fts_path[ptr->fts_pathlen - 1] == '/' ?	\
		      ptr->fts_pathlen - 1 : ptr->fts_pathlen)

#define BCHILD 1
#define BNAMES 2
#define BREAD  3

static size_t
__fts_maxarglen (char *const *argv)
{
  size_t len;
  size_t max;
  for (max = 0; *argv != NULL; argv++)
    {
      len = strlen (*argv);
      if (len > max)
	max = len;
    }
  return max + 1;
}

static int
__fts_path_alloc (FTS *ftsp, size_t more)
{
  char *ptr;
  ftsp->fts_pathlen += more + 256;
  if (ftsp->fts_pathlen < 0 || ftsp->fts_pathlen >= USHRT_MAX)
    {
      free (ftsp->fts_path);
      ftsp->fts_path = NULL;
      errno = ENAMETOOLONG;
      return 1;
    }
  ptr = realloc (ftsp->fts_path, ftsp->fts_pathlen);
  if (unlikely (ptr == NULL))
    {
      free (ftsp->fts_path);
      ftsp->fts_path = NULL;
      return 1;
    }
  ftsp->fts_path = ptr;
  return 0;
}

static void
__fts_path_adjust (FTS *ftsp, FTSENT *head)
{
#define ADJUST(ptr) do							\
    {									\
      if ((ptr)->fts_accpath != (ptr)->fts_name)			\
	{								\
	  (ptr)->fts_accpath =						\
	    (char *) addr + ((ptr)->fts_accpath - (ptr)->fts_path);	\
	}								\
      (ptr)->fts_path = addr;						\
    }									\
  while (0)

  FTSENT *ptr;
  char *addr = ftsp->fts_path;
  for (ptr = ftsp->fts_child; ptr != NULL; ptr = ptr->fts_link)
    ADJUST (ptr);
  for (ptr = head; ptr->fts_level >= FTS_ROOTLEVEL;)
    {
      ADJUST (ptr);
      ptr = ptr->fts_link != NULL ? ptr->fts_link : ptr->fts_parent;
    }

#undef ADJUST
}

static FTSENT *
__fts_alloc (FTS *ftsp, const char *name, size_t namelen)
{
  FTSENT *ptr;
  size_t len = sizeof (FTSENT) + namelen;
  if (!(ftsp->fts_options & FTS_NOSTAT))
    len += sizeof (struct stat) + ALIGNBYTES;
  ptr = malloc (len);
  if (unlikely (ptr == NULL))
    return NULL;
  memmove (ptr->fts_name, name, namelen);
  ptr->fts_name[namelen] = '\0';
  if (!(ftsp->fts_options & FTS_NOSTAT))
    ptr->fts_statp = (struct STAT *) ALIGN (ptr->fts_name + namelen + 2);
  ptr->fts_namelen = namelen;
  ptr->fts_path = ftsp->fts_path;
  ptr->fts_errno = 0;
  ptr->fts_flags = 0;
  ptr->fts_instr = FTS_NOINSTR;
  ptr->fts_number = 0;
  ptr->fts_pointer = NULL;
  return ptr;
}

static void
__fts_lfree (FTSENT *head)
{
  FTSENT *ptr;
  while (head != NULL)
    {
      ptr = head;
      head = head->fts_link;
      free (ptr);
    }
}

static unsigned short
__fts_stat (FTS *ftsp, FTSENT *ptr, int follow)
{
  FTSENT *t;
  dev_t dev;
  ino_t ino;
  struct STAT sb;
  struct STAT *sbp = ftsp->fts_options & FTS_NOSTAT ? &sb : ptr->fts_statp;
  int saved_errno;

  if ((ftsp->fts_options & FTS_LOGICAL) || follow)
    {
      if (stat (ptr->fts_accpath, sbp) == -1)
	{
	  saved_errno = errno;
	  if (lstat (ptr->fts_accpath, sbp) == 0)
	    {
	      errno = 0;
	      return FTS_SLNONE;
	    }
	  ptr->fts_errno = saved_errno;
	  memset (sbp, 0, sizeof (struct stat));
	  return FTS_NS;
	}
    }
  else if (lstat (ptr->fts_accpath, sbp) == -1)
    {
      ptr->fts_errno = errno;
      memset (sbp, 0, sizeof (struct stat));
      return FTS_NS;
    }

  if (S_ISDIR (sbp->st_mode))
    {
      dev = sbp->st_dev;
      ptr->fts_dev = dev;
      ino = sbp->st_ino;
      ptr->fts_ino = ino;
      ptr->fts_nlink = sbp->st_nlink;
      if (ISDOT (ptr->fts_name))
	return FTS_DOT;
      for (t = ptr->fts_parent; t->fts_level >= FTS_ROOTLEVEL;
	   t = t->fts_parent)
	{
	  if (ino == t->fts_ino && dev == t->fts_dev)
	    {
	      ptr->fts_cycle = t;
	      return FTS_DC;
	    }
	}
      return FTS_D;
    }
  if (S_ISLNK (sbp->st_mode))
    return FTS_SL;
  if (S_ISREG (sbp->st_mode))
    return FTS_F;
  return FTS_DEFAULT;
}

static FTSENT *
__fts_sort (FTS *ftsp, FTSENT *head, int nitems)
{
  FTSENT **ap;
  FTSENT *ptr;
  if (nitems > ftsp->fts_nitems)
    {
      FTSENT **a;
      ftsp->fts_nitems = nitems + 40;
      a = realloc (ftsp->fts_array, ftsp->fts_nitems * sizeof (FTSENT *));
      if (unlikely (a == NULL))
	{
	  free (ftsp->fts_array);
	  ftsp->fts_array = NULL;
	  ftsp->fts_nitems = 0;
	  return head;
	}
      ftsp->fts_array = a;
    }
  for (ap = ftsp->fts_array, ptr = head; ptr != NULL; ptr = ptr->fts_link)
    *ap++ = ptr;
  qsort (ftsp->fts_array, nitems, sizeof (FTSENT *), ftsp->fts_compar);
  for (ap = ftsp->fts_array, head = *ap; --nitems > 0; ap++)
    ap[0]->fts_link = ap[1];
  ap[0]->fts_link = NULL;
  return head;
}

static int
__fts_chdir (FTS *ftsp, FTSENT *ptr, int fd, const char *path)
{
  int old_errno;
  int newfd = fd;
  struct __STAT st;
  int ret;
  if (ftsp->fts_options & FTS_NOCHDIR)
    return 0;

  if (fd < 0)
    {
      newfd = open (path, O_RDONLY, 0);
      if (newfd == -1)
	return -1;
    }

  if (fstat64 (newfd, &st) == -1)
    {
      ret = -1;
      goto end;
    }
  if (ptr->fts_dev != st.st_dev || ptr->fts_ino != st.st_ino)
    {
      errno = ENOENT;
      ret = -1;
      goto end;
    }
  ret = fchdir (newfd);

 end:
  old_errno = errno;
  if (fd < 0)
    close (newfd);
  errno = old_errno;
  return ret;
}

static void
__fts_load (FTS *ftsp, FTSENT *ptr)
{
  char *c;
  int len;
  ptr->fts_pathlen = ptr->fts_namelen;
  len = ptr->fts_namelen;
  memmove (ftsp->fts_path, ptr->fts_name, len + 1);
  c = strrchr (ptr->fts_name, '/');
  if (c != NULL && (c != ptr->fts_name || c[1] != '\0'))
    {
      len = strlen (++c);
      memmove (ptr->fts_name, c, len + 1);
      ptr->fts_namelen = len;
    }
  ptr->fts_path = ftsp->fts_path;
  ptr->fts_accpath = ftsp->fts_path;
  ftsp->fts_dev = ptr->fts_dev;
}

static FTSENT *
__fts_build (FTS *ftsp, int type)
{
  struct dirent *dp;
  DIR *dirp;
  FTSENT *ptr;
  FTSENT *head;
  FTSENT *cur;
  FTSENT *tail;
  void *oldaddr;
  int nitems;
  int saved_errno;
  int descend;
  int len;
  int level;
  int nlinks;
  int nostat;
  int doadjust;
  size_t maxlen;
  char *c;

  cur = ftsp->fts_cur;
  dirp = opendir (cur->fts_accpath);
  if (dirp == NULL)
    {
      if (type == BREAD)
	{
	  cur->fts_info = FTS_DNR;
	  cur->fts_errno = errno;
	}
      return NULL;
    }

  if (type == BNAMES)
    {
      nlinks = 0;
      nostat = 0;
    }
  else if ((ftsp->fts_options & FTS_NOSTAT)
	   && (ftsp->fts_options & FTS_PHYSICAL))
    {
      nlinks = cur->fts_nlink - (ftsp->fts_options & FTS_SEEDOT ? 0 : 2);
      nostat = 1;
    }
  else
    {
      nlinks = -1;
      nostat = 0;
    }

  if (nlinks != 0 || type == BREAD)
    {
      if (__fts_chdir (ftsp, cur, dirfd (dirp), NULL) == -1)
	{
	  if (nlinks != 0 && type == BREAD)
	    cur->fts_errno = errno;
	  cur->fts_flags |= FTS_DONTCHDIR;
	  descend = 0;
	  closedir (dirp);
	  dirp = NULL;
	}
      else
	descend = 1;
    }
  else
    descend = 0;

  len = NAPPEND (cur);
  if (ftsp->fts_options & FTS_NOCHDIR)
    {
      c = ftsp->fts_path + len;
      *c++ = '/';
    }
  else
    c = NULL;
  len++;
  maxlen = ftsp->fts_pathlen - len;
  level = cur->fts_level + 1;
  doadjust = 0;

  for (head = NULL, tail = NULL, nitems = 0;
       dirp != NULL && (dp = readdir (dirp)) != NULL;)
    {
      if (!(ftsp->fts_options & FTS_SEEDOT) && ISDOT (dp->d_name))
	continue;
      ptr = __fts_alloc (ftsp, dp->d_name, strlen (dp->d_name));
      if (ptr == NULL)
	goto err;
      if (strlen (dp->d_name) >= maxlen)
	{
	  oldaddr = ftsp->fts_path;
	  if (__fts_path_alloc (ftsp, strlen (dp->d_name) + len + 1) != 0)
	    {
	    err:
	      saved_errno = errno;
	      free (ptr);
	      __fts_lfree (head);
	      closedir (dirp);
	      cur->fts_info = FTS_ERR;
	      ftsp->fts_options |= FTS_STOP;
	      errno = saved_errno;
	      return NULL;
	    }

	  if (oldaddr != ftsp->fts_path)
	    {
	      doadjust = 1;
	      if (ftsp->fts_options & FTS_NOCHDIR)
		c = ftsp->fts_path + len;
	    }
	  maxlen = ftsp->fts_pathlen - len;
	}

      if (len + strlen (dp->d_name) >= USHRT_MAX)
	{
	  errno = ENAMETOOLONG;
	  goto err;
	}
      ptr->fts_level = level;
      ptr->fts_parent = ftsp->fts_cur;
      ptr->fts_pathlen = len + strlen (dp->d_name);

      if (nlinks == 0
	  || (nostat && dp->d_type != DT_DIR && dp->d_type != DT_UNKNOWN))
	{
	  ptr->fts_accpath =
	    ftsp->fts_options & FTS_NOCHDIR ? ptr->fts_path : ptr->fts_name;
	  ptr->fts_info = FTS_NSOK;
	}
      else
	{
	  if (ftsp->fts_options & FTS_NOCHDIR)
	    {
	      ptr->fts_accpath = ptr->fts_path;
	      memmove (c, ptr->fts_name, ptr->fts_namelen + 1);
	    }
	  else
	    ptr->fts_accpath = ptr->fts_name;
	  ptr->fts_info = __fts_stat (ftsp, ptr, 0);

	  if (nlinks > 0 && (ptr->fts_info == FTS_D || ptr->fts_info == FTS_DC
			     || ptr->fts_info == FTS_DOT))
	    nlinks--;
	}

      ptr->fts_link = NULL;
      if (head == NULL)
	{
	  head = ptr;
	  tail = ptr;
	}
      else
	{
	  tail->fts_link = ptr;
	  tail = ptr;
	}
      nitems++;
    }

  if (dirp != NULL)
    closedir (dirp);
  if (doadjust)
    __fts_path_adjust (ftsp, head);
  if (ftsp->fts_options & FTS_NOCHDIR)
    {
      if (len == ftsp->fts_pathlen || nitems == 0)
	c--;
      *c = '\0';
    }

  if (descend && (type == BCHILD || nitems == 0)
      && (cur->fts_level == FTS_ROOTLEVEL ? FCHDIR (ftsp, ftsp->fts_rfd) :
	  __fts_chdir (ftsp, cur->fts_parent, -1, "..")))
    {
      cur->fts_info = FTS_ERR;
      ftsp->fts_options |= FTS_STOP;
      __fts_lfree (head);
      return NULL;
    }

  if (nitems == 0)
    {
      if (type == BREAD)
	cur->fts_info = FTS_DP;
      __fts_lfree (head);
      return NULL;
    }
  if (ftsp->fts_compar != NULL && nitems > 1)
    head = __fts_sort (ftsp, head, nitems);
  return head;
}

FTS *
fts_open (char *const *argv, int options,
	  int (*compare) (const FTSENT **, const FTSENT **))
{
  FTS *ftsp;
  FTSENT *ptr;
  FTSENT *root;
  FTSENT *parent = NULL;
  FTSENT *temp;
  int nitems;
  size_t maxarglen;
  if (options & ~FTS_OPTIONMASK)
    {
      errno = EINVAL;
      return NULL;
    }

  ftsp = malloc (sizeof (FTS));
  if (unlikely (ftsp == NULL))
    return NULL;
  memset (ftsp, 0, sizeof (FTS));
  ftsp->fts_compar = (int (*) (const void *, const void *)) compare;
  ftsp->fts_options = options;

  if (ftsp->fts_options & FTS_LOGICAL)
    ftsp->fts_options |= FTS_NOCHDIR;

  maxarglen = __fts_maxarglen (argv);
  if (__fts_path_alloc (ftsp, MAX (maxarglen, MAXPATHLEN)) != 0)
    {
      free (ftsp);
      return NULL;
    }

  if (*argv != NULL)
    {
      parent = __fts_alloc (ftsp, "", 0);
      if (unlikely (parent == NULL))
	{
	  free (ftsp->fts_path);
	  free (ftsp);
	  return NULL;
	}
      parent->fts_level = FTS_ROOTPARENTLEVEL;
    }

  for (root = NULL, nitems = 0; *argv != NULL; argv++, nitems++)
    {
      size_t len = strlen (*argv);
      if (len == 0)
	{
	  errno = ENOENT;
	  __fts_lfree (root);
	  free (parent);
	  free (ftsp->fts_path);
	  free (ftsp);
	  return NULL;
	}
      ptr = __fts_alloc (ftsp, *argv, len);
      ptr->fts_level = FTS_ROOTLEVEL;
      ptr->fts_parent = parent;
      ptr->fts_accpath = ptr->fts_name;
      ptr->fts_info = __fts_stat (ftsp, ptr, ftsp->fts_options & FTS_COMFOLLOW);
      if (ptr->fts_info == FTS_DOT)
	ptr->fts_info = FTS_D;

      if (compare != NULL)
	{
	  ptr->fts_link = root;
	  root = ptr;
	}
      else
	{
	  ptr->fts_link = NULL;
	  if (root == NULL)
	    {
	      root = ptr;
	      temp = ptr;
	    }
	  else
	    {
	      temp->fts_link = ptr;
	      temp = ptr;
	    }
	}
    }
  if (compare && nitems > 1)
    root = __fts_sort (ftsp, root, nitems);

  ftsp->fts_cur = __fts_alloc (ftsp, "", 0);
  if (unlikely (ftsp->fts_cur == NULL))
    {
      __fts_lfree (root);
      free (parent);
      free (ftsp->fts_path);
      free (ftsp);
      return NULL;
    }
  ftsp->fts_cur->fts_link = root;
  ftsp->fts_cur->fts_info = FTS_INIT;

  if (!(ftsp->fts_options & FTS_NOCHDIR))
    {
      ftsp->fts_rfd = open (".", O_RDONLY, 0);
      if (ftsp->fts_rfd == -1)
	ftsp->fts_options |= FTS_NOCHDIR;
    }
  return ftsp;
}

int
fts_close (FTS *ftsp)
{
  FTSENT *freeptr;
  FTSENT *ptr;
  int saved_errno;
  if (ftsp->fts_cur != NULL)
    {
      for (ptr = ftsp->fts_cur; ptr->fts_level >= FTS_ROOTLEVEL;)
	{
	  freeptr = ptr;
	  ptr = ptr->fts_link != NULL ? ptr->fts_link : ptr->fts_parent;
	  free (freeptr);
	}
      free (ptr);
    }

  if (ftsp->fts_child != NULL)
    __fts_lfree (ftsp->fts_child);
  free (ftsp->fts_array);
  free (ftsp->fts_path);

  if (!(ftsp->fts_options & FTS_NOCHDIR))
    {
      saved_errno = fchdir (ftsp->fts_rfd) == -1 ? errno : 0;
      close (ftsp->fts_rfd);
      if (saved_errno != 0)
	{
	  free (ftsp);
	  errno = saved_errno;
	  return -1;
	}
    }

  free (ftsp);
  return 0;
}

FTSENT *
fts_read (FTS *ftsp)
{
  FTSENT *ptr;
  FTSENT *temp;
  char *t;
  int instr;
  int saved_errno;

  if (ftsp->fts_cur == NULL || (ftsp->fts_options & FTS_STOP))
    return NULL;

  ptr = ftsp->fts_cur;
  instr = ptr->fts_instr;
  ptr->fts_instr = FTS_NOINSTR;

  if (instr == FTS_AGAIN)
    {
      ptr->fts_info = __fts_stat (ftsp, ptr, 0);
      return ptr;
    }

  if (instr == FTS_FOLLOW
      && (ptr->fts_info == FTS_SL || ptr->fts_info == FTS_SLNONE))
    {
      ptr->fts_info = __fts_stat (ftsp, ptr, 1);
      if (ptr->fts_info == FTS_D && !(ftsp->fts_options & FTS_NOCHDIR))
	{
	  ptr->fts_symfd = open (".", O_RDONLY, 0);
	  if (ptr->fts_symfd == -1)
	    {
	      ptr->fts_errno = errno;
	      ptr->fts_info = FTS_ERR;
	    }
	  else
	    ptr->fts_flags |= FTS_SYMFOLLOW;
	}
      return ptr;
    }

  if (ptr->fts_info == FTS_D)
    {
      if (instr == FTS_SKIP
	  || ((ftsp->fts_options & FTS_XDEV) && ptr->fts_dev != ftsp->fts_dev))
	{
	  if (ptr->fts_flags & FTS_SYMFOLLOW)
	    close (ptr->fts_symfd);
	  if (ftsp->fts_child != NULL)
	    {
	      __fts_lfree (ftsp->fts_child);
	      ftsp->fts_child = NULL;
	    }
	  ptr->fts_info = FTS_DP;
	  return ptr;
	}

      if (ftsp->fts_child != NULL && (ftsp->fts_options & FTS_NAMEONLY))
	{
	  ftsp->fts_options &= ~FTS_NAMEONLY;
	  __fts_lfree (ftsp->fts_child);
	  ftsp->fts_child = NULL;
	}

      if (ftsp->fts_child != NULL)
	{
	  if (__fts_chdir (ftsp, ptr, -1, ptr->fts_accpath) == -1)
	    {
	      ptr->fts_errno = errno;
	      ptr->fts_flags |= FTS_DONTCHDIR;
	      for (ptr = ftsp->fts_child; ptr != NULL; ptr = ptr->fts_link)
		ptr->fts_accpath = ptr->fts_parent->fts_accpath;
	    }
	}
      else
	{
	  ftsp->fts_child = __fts_build (ftsp, BREAD);
	  if (ftsp->fts_child == NULL)
	    {
	      if (ftsp->fts_options & FTS_STOP)
		return NULL;
	      return ptr;
	    }
	}

      ptr = ftsp->fts_child;
      ftsp->fts_child = NULL;
      ftsp->fts_cur = ptr;
      goto name;
    }

 next:
  temp = ptr;
  ptr = ptr->fts_link;
  if (ptr != NULL)
    {
      ftsp->fts_cur = ptr;
      free (temp);

      if (ptr->fts_level == FTS_ROOTLEVEL)
	{
	  if (FCHDIR (ftsp, ftsp->fts_rfd))
	    {
	      ftsp->fts_options |= FTS_STOP;
	      return NULL;
	    }
	  __fts_load (ftsp, ptr);
	  return ptr;
	}

      if (ptr->fts_instr == FTS_SKIP)
	goto next;
      if (ptr->fts_instr == FTS_FOLLOW)
	{
	  ptr->fts_info = __fts_stat (ftsp, ptr, 1);
	  if (ptr->fts_info == FTS_D && !(ftsp->fts_options & FTS_NOCHDIR))
	    {
	      ptr->fts_symfd = open (".", O_RDONLY, 0);
	      if (ptr->fts_symfd == -1)
		{
		  ptr->fts_errno = errno;
		  ptr->fts_info = FTS_ERR;
		}
	      else
		ptr->fts_flags |= FTS_SYMFOLLOW;
	    }
	  ptr->fts_instr = FTS_NOINSTR;
	}

    name:
      t = ftsp->fts_path + NAPPEND (ptr->fts_parent);
      *t++ = '/';
      memmove (t, ptr->fts_name, ptr->fts_namelen + 1);
      return ptr;
    }

  ptr = temp->fts_parent;
  ftsp->fts_cur = ptr;
  free (temp);

  if (ptr->fts_level == FTS_ROOTPARENTLEVEL)
    {
      free (ptr);
      errno = 0;
      ftsp->fts_cur = NULL;
      return NULL;
    }

  ftsp->fts_path[ftsp->fts_pathlen] = '\0';
  if (ptr->fts_level == FTS_ROOTLEVEL)
    {
      if (FCHDIR (ftsp, ftsp->fts_rfd))
	{
	  ftsp->fts_options |= FTS_STOP;
	  return NULL;
	}
    }
  else if (ptr->fts_flags & FTS_SYMFOLLOW)
    {
      if (FCHDIR (ftsp, ptr->fts_symfd))
	{
	  saved_errno = errno;
	  close (ptr->fts_symfd);
	  errno = saved_errno;
	  ftsp->fts_options |= FTS_STOP;
	  return NULL;
	}
      close (ptr->fts_symfd);
    }
  else if (!(ptr->fts_flags & FTS_DONTCHDIR)
	   && __fts_chdir (ftsp, ptr->fts_parent, -1, "..") == -1)
    {
      ftsp->fts_options |= FTS_STOP;
      return NULL;
    }
  ptr->fts_info = ptr->fts_errno != 0 ? FTS_ERR : FTS_DP;
  return ptr;
}

FTSENT *
fts_children (FTS *ftsp, int instr)
{
  FTSENT *ptr;
  int fd;
  if (instr != 0 && instr != FTS_NAMEONLY)
    {
      errno = EINVAL;
      return NULL;
    }

  ptr = ftsp->fts_cur;
  errno = 0;
  if (ftsp->fts_options & FTS_STOP)
    return NULL;

  if (ptr->fts_info == FTS_INIT)
    return ptr->fts_link;
  if (ptr->fts_info != FTS_D)
    return NULL;

  if (ftsp->fts_child != NULL)
    __fts_lfree (ftsp->fts_child);
  if (instr == FTS_NAMEONLY)
    {
      ftsp->fts_options |= FTS_NAMEONLY;
      instr = BNAMES;
    }
  else
    instr = BCHILD;

  if (ptr->fts_level != FTS_ROOTLEVEL || *ptr->fts_accpath == '/'
      || (ftsp->fts_options & FTS_NOCHDIR))
    {
      ftsp->fts_child = __fts_build (ftsp, instr);
      return ftsp->fts_child;
    }

  fd = open (".", O_RDONLY, 0);
  if (fd == -1)
    return NULL;
  ftsp->fts_child = __fts_build (ftsp, instr);
  if (fchdir (fd) == -1)
    return NULL;
  close (fd);
  return ftsp->fts_child;
}

int
fts_set (FTS *ftsp, FTSENT *ptr, int instr)
{
  if (instr != 0 && instr != FTS_AGAIN && instr != FTS_FOLLOW
      && instr != FTS_NOINSTR && instr != FTS_SKIP)
    {
      errno = EINVAL;
      return 1;
    }
  ptr->fts_instr = instr;
  return 0;
}
