/* pwd.h -- This file is part of OS/0 libc.
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

#ifndef _PWD_H
#define _PWD_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stddef.h>

struct passwd
{
  char *pw_name;
  char *pw_passwd;
  uid_t pw_uid;
  gid_t pw_gid;
  char *pw_gecos;
  char *pw_dir;
  char *pw_shell;
};

__BEGIN_DECLS

struct passwd *getpwnam (const char *name);
struct passwd *getpwuid (uid_t uid);
int getpwnam_r (const char *__restrict name, struct passwd *__restrict pwd,
		char *__restrict buffer, size_t len,
		struct passwd **__restrict result);
int getpwuid_r (uid_t uid, struct passwd *__restrict pwd,
		char *__restrict buffer, size_t len,
		struct passwd **__restrict result);

__END_DECLS

#endif
