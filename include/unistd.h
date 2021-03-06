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

#include <bits/getopt.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/unistd.h>

__BEGIN_DECLS

int fork (void);
int vfork (void) __attribute__ ((returns_twice));
pid_t getpid (void);
pid_t getppid (void);

int setuid (uid_t uid);
uid_t getuid (void);
int setgid (gid_t gid);
gid_t getgid (void);
int seteuid (uid_t euid);
uid_t geteuid (void);
int setegid (gid_t egid);
gid_t getegid (void);
int setreuid (uid_t ruid, uid_t euid);
int setregid (gid_t rgid, gid_t egid);
int getgroups (int size, gid_t list[]);
int setresuid (uid_t ruid, uid_t euid, uid_t suid);
int getresuid (uid_t *ruid, uid_t *euid, uid_t *suid);
int setresgid (gid_t rgid, gid_t egid, gid_t sgid);
int getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid);
int setpgid (pid_t pid, pid_t pgid);
pid_t getpgid (pid_t pid);
pid_t getpgrp (void);
pid_t setsid (void);
int setpgrp (void);

char *getcwd (char *buffer, size_t len);
char *getwd (char *buffer);
char *getlogin (void);

int execl (const char *path, const char *arg0, ...);
int execle (const char *path, const char *arg0, ...);
int execlp (const char *file, const char *arg0, ...);
int execv (const char *path, char *const *argv);
int execvp (const char *file, char *const *argv);
int execve (const char *path, char *const *argv, char *const *envp);
void _exit (int code) __attribute__ ((noreturn));

int isatty (int fd);
char *ttyname (int fd);
int ttyname_r (int fd, char *buffer, size_t len);

int chdir (const char *path);
int fchdir (int fd);

int brk (void *ptr);
void *sbrk (int inc);

ssize_t pread (int fd, void *buffer, size_t len, off_t offset);
ssize_t pread64 (int fd, void *buffer, size_t len, off64_t offset);
ssize_t read (int fd, void *buffer, size_t len);

ssize_t pwrite (int fd, const void *buffer, size_t len, off_t offset);
ssize_t pwrite64 (int fd, const void *buffer, size_t len, off64_t offset);
ssize_t write (int fd, const void *buffer, size_t len);

off_t lseek (int fd, off_t offset, int whence);
off64_t lseek64 (int fd, off64_t offset, int whence);

int close (int fd);
int pipe (int fd[2]);

int dup (int fd);
int dup2 (int fd1, int fd2);

int access (const char *path, int mode);
int faccessat (int fd, const char *path, int mode, int flags);

int link (const char *oldpath, const char *newpath);
int linkat (int oldfd, const char *oldpath, int newfd, const char *newpath,
	    int flags);

int unlink (const char *path);
int unlinkat (int fd, const char *path, int flags);
int rmdir (const char *path);

int chown (const char *path, uid_t uid, gid_t gid);
int fchown (int fd, uid_t uid, gid_t gid);
int lchown (const char *path, uid_t uid, gid_t gid);
int fchownat (int fd, const char *path, uid_t uid, gid_t gid, int flags);

int symlink (const char *oldpath, const char *newpath);
int symlinkat (const char *oldpath, int fd, const char *newpath);

ssize_t readlink (const char *__restrict path, char *__restrict buffer,
		  size_t len);
ssize_t readlinkat (int fd, const char *__restrict path,
		    char *__restrict buffer, size_t len);

int truncate (const char *path, off_t len);
int ftruncate (int fd, off_t len);
int truncate64 (const char *path, off64_t len);
int ftruncate64 (int fd, off64_t len);

int getentropy (void *buffer, size_t len);

int nice (int inc);

int fsync (int fd);
void sync (void);

int reboot (int cmd);

unsigned int sleep (unsigned int seconds);
unsigned int alarm (unsigned int seconds);
int pause (void);

long syscall (long num, ...);

long sysconf (int name);

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define pread     pread64
#define pwrite    pwrite64
#define lseek     lseek64
#define truncate  truncate64
#define ftruncate ftruncate64
#endif

__END_DECLS

#endif
