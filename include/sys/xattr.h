/* xattr.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_XATTR_H
#define _SYS_XATTR_H

#include <bits/xattr.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <stddef.h>

__BEGIN_DECLS

int setxattr (const char *path, const char *name, const void *value, size_t len,
	      int flags);
int lsetxattr (const char *path, const char *name, const void *value,
	       size_t len, int flags);
int fsetxattr (int fd, const char *name, const void *value, size_t len,
	       int flags);
ssize_t getxattr (const char *path, const char *name, void *value, size_t len);
ssize_t lgetxattr (const char *path, const char *name, void *value, size_t len);
ssize_t fgetxattr (int fd, const char *name, void *value, size_t len);
ssize_t listxattr (const char *path, char *buffer, size_t len);
ssize_t llistxattr (const char *path, char *buffer, size_t len);
ssize_t flistxattr (int fd, char *buffer, size_t len);
int removexattr (const char *path, const char *name);
int lremovexattr (const char *path, const char *name);
int fremovexattr (int fd, const char *name);

__END_DECLS

#endif
