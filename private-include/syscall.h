/* syscall.h -- This file is part of OS/0 libc.
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

#ifndef _SYSCALL_H
#define _SYSCALL_H

#define SYS_exit          1
#define SYS_fork          2
#define SYS_read          3
#define SYS_write         4
#define SYS_open          5
#define SYS_close         6
#define SYS_waitpid       7
#define SYS_creat         8
#define SYS_link          9
#define SYS_unlink        10
#define SYS_execve        11
#define SYS_chdir         12
#define SYS_time          13
#define SYS_mknod         14
#define SYS_chmod         15
#define SYS_chown         16
#define SYS_lseek         19
#define SYS_getpid        20
#define SYS_mount         21
#define SYS_umount       22
#define SYS_setuid        23
#define SYS_getuid        24
#define SYS_access        33
#define SYS_sync          36
#define SYS_kill          37
#define SYS_rename        38
#define SYS_mkdir         39
#define SYS_rmdir         40
#define SYS_dup           41
#define SYS_pipe          42
#define SYS_brk           45
#define SYS_setgid        46
#define SYS_getgid        47
#define SYS_signal        48
#define SYS_ioctl         54
#define SYS_fcntl         55
#define SYS_umask         60
#define SYS_chroot        61
#define SYS_dup2          63
#define SYS_sigaction     67
#define SYS_sigsuspend    72
#define SYS_sigpending    73
#define SYS_sethostname   74
#define SYS_setrlimit     75
#define SYS_getrlimit     76
#define SYS_getrusage     77
#define SYS_gettimeofday  78
#define SYS_settimeofday  79
#define SYS_symlink       83
#define SYS_readlink      85
#define SYS_swapon        87
#define SYS_reboot        88
#define SYS_mmap          90
#define SYS_munmap        91
#define SYS_truncate      92
#define SYS_getpriority   96
#define SYS_setpriority   97
#define SYS_statvfs       99
#define SYS_ioperm        101
#define SYS_socketcall    102
#define SYS_syslog        103
#define SYS_setitimer     104
#define SYS_getitimer     105
#define SYS_stat          106
#define SYS_wait4         114
#define SYS_swapoff       115
#define SYS_sysinfo       116
#define SYS_fsync         118
#define SYS_clone         120
#define SYS_setdomainname 121
#define SYS_uname         122
#define SYS_quotactl      131
#define SYS_getpgid       132
#define SYS_fchdir        133
#define SYS_sysfs         135
#define SYS_setfsuid      138
#define SYS_setfsgid      139
#define SYS_flock         143
#define SYS_getsid        147
#define SYS_fdatasync     148
#define SYS_nanosleep     162
#define SYS_getcwd        183
#define SYS_setxattr      226
#define SYS_getxattr      229
#define SYS_listxattr     232
#define SYS_removexattr   235

#define NR_syscalls 236

#endif
