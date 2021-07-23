/* mntent.h -- This file is part of OS/0 libc.
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

#ifndef _MNTENT_H
#define _MNTENT_H

#include <sys/cdefs.h>
#include <paths.h>

#ifndef __FILE_defined
typedef struct __FILE FILE;
#define __FILE_defined
#endif

#define MNTTAB       _PATH_MNTTAB
#define MOUNTED      _PATH_MOUNTED

#define MNTTYPE_IGNORE "ignore"
#define MNTTYPE_NFS    "nfs"
#define MNTTYPE_SWAP   "swap"

#define MNTOPT_DEFAULTS "defaults"
#define MNTOPT_RO       "ro"
#define MNTOPT_RW       "rw"
#define MNTOPT_SUID     "suid"
#define MNTOPT_NOSUID   "nosuid"
#define MNTOPT_NOAUTO   "noauto"

struct mntent
{
  char *mnt_fsname;
  char *mnt_dir;
  char *mnt_type;
  char *mnt_opts;
  int mnt_freq;
  int mnt_passno;
};

__BEGIN_DECLS

FILE *setmntent (const char *name, const char *type);
struct mntent *getmntent (FILE *stream);
struct mntent *getmntent_r (FILE *__restrict stream,
			    struct mntent *__restrict mnt,
			    char *__restrict buffer, int bufsize);
int addmntent (FILE *__restrict stream, const struct mntent *__restrict mnt);
int endmntent (FILE *stream);
char *hasmntopt (const struct mntent *mnt, const char *opt);

__END_DECLS

#endif
