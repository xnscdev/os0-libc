/* unlink.c -- This file is part of OS/0 libc.
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

#include <sys/syscall.h>
#include <unistd.h>

int
setuid (uid_t uid)
{
  return syscall (SYS_setuid, uid);
}

uid_t
getuid (void)
{
  return syscall (SYS_getuid);
}

int
setgid (gid_t gid)
{
  return syscall (SYS_setgid, gid);
}

gid_t
getgid (void)
{
  return syscall (SYS_getgid);
}

int
seteuid (uid_t euid)
{
  return setreuid (-1, euid);
}

uid_t
geteuid (void)
{
  uid_t euid;
  getresuid (NULL, &euid, NULL);
  return euid;
}

int
setegid (gid_t egid)
{
  return setregid (-1, egid);
}

gid_t
getegid (void)
{
  gid_t egid;
  getresgid (NULL, &egid, NULL);
  return egid;
}

int
setreuid (uid_t ruid, uid_t euid)
{
  return syscall (SYS_setreuid, ruid, euid);
}

int
setregid (gid_t rgid, gid_t egid)
{
  return syscall (SYS_setregid, rgid, egid);
}

int
setresuid (uid_t ruid, uid_t euid, uid_t suid)
{
  return syscall (SYS_setresuid, ruid, euid, suid);
}

int
getresuid (uid_t *ruid, uid_t *euid, uid_t *suid)
{
  return syscall (SYS_getresuid, ruid, euid, suid);
}

int
setresgid (gid_t rgid, gid_t egid, gid_t sgid)
{
  return syscall (SYS_setresgid, rgid, egid, sgid);
}

int
getresgid (gid_t *rgid, gid_t *egid, gid_t *sgid)
{
  return syscall (SYS_getresgid, rgid, egid, sgid);
}
