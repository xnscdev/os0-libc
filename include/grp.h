/* grp.h -- This file is part of OS/0 libc.
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

#ifndef _GRP_H
#define _GRP_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stddef.h>

#define __gr_mem_limit 16

struct group
{
  char *gr_name;
  gid_t gr_gid;
  char **gr_mem;
  char *__gr_memv[__gr_mem_limit];
};

__BEGIN_DECLS

struct group *getgrgid (gid_t gid);
struct group *getgrnam (const char *name);
int getgrgid_r (gid_t gid, struct group *__restrict grp,
		char *__restrict buffer, size_t len,
		struct group **__restrict result);
int getgrnam_r (const char *__restrict name, struct group *__restrict grp,
		char *__restrict buffer, size_t len,
		struct group **__restrict result);
void setgrent (void);
struct group *getgrent (void);
void endgrent (void);

int setgroups (size_t, const gid_t *list);
int getgrouplist (const char *user, gid_t group, gid_t *groups, int *ngroups);

__END_DECLS

#endif
