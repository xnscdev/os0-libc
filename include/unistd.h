/* unistd.h -- This file is part of OS/0 libc.
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

#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <stddef.h>

__BEGIN_DECLS

int fork (void);
pid_t getpid (void);
int execv (const char *path, char *const *argv);
int execvp (const char *file, char *const *argv);
int execve (const char *path, char *const *argv, char *const *envp);
void _exit (int code) __attribute__ ((noreturn));

int isatty (int fd);

int chdir (const char *path);
int fchdir (int fd);

int brk (void *ptr);
void *sbrk (int inc);

ssize_t pread (int fd, void *buffer, size_t len, off_t offset);
ssize_t read (int fd, void *buffer, size_t len);

ssize_t pwrite (int fd, const void *buffer, size_t len, off_t offset);
ssize_t write (int fd, const void *buffer, size_t len);

off_t lseek (int fd, off_t offset, int whence);

int open (const char *path, int flags, ...);
int openat (int fd, const char *path, int flags, ...);
int creat (const char *path, mode_t mode);
int close (int fd);

int access (const char *path, int mode);
int faccessat (int fd, const char *path, int mode, int flags);

int link (const char *old, const char *new);
int linkat (int oldfd, const char *old, int newfd, const char *new, int flags);

int unlink (const char *path);
int unlinkat (int fd, const char *path, int flags);

int mknod (const char *path, mode_t mode, dev_t dev);
int mknodat (int fd, const char *path, mode_t mode, dev_t dev);

int chmod (const char *path, mode_t mode);
int fchmod (int fd, mode_t mode);
int fchmodat (int fd, const char *path, mode_t mode, int flags);

int chown (const char *path, uid_t uid, gid_t gid);
int fchown (int fd, uid_t uid, gid_t gid);
int lchown (const char *path, uid_t uid, gid_t gid);
int fchownat (int fd, const char *path, uid_t uid, gid_t gid, int flags);

int rename (const char *old, const char *new);
int renameat (int oldfd, const char *old, int newfd, const char *new);

int mkdir (const char *path, mode_t mode);
int mkdirat (int fd, const char *path, mode_t mode);
int rmdir (const char *path);

int symlink (const char *old, const char *new);
int symlinkat (const char *old, int fd, const char *new);

ssize_t readlink (const char *__restrict path, char *__restrict buffer,
		  size_t len);
ssize_t readlinkat (int fd, const char *__restrict path,
		    char *__restrict buffer, size_t len);

int truncate (const char *path, off_t len);
int ftruncate (int fd, off_t len);

__END_DECLS

#endif
