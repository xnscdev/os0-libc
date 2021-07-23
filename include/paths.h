/* paths.h -- This file is part of OS/0 libc.
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

#ifndef _PATHS_H
#define _PATHS_H

#define _PATH_DEFPATH "/usr/bin:/bin"
#define _PATH_STDPATH "/usr/bin:/bin:/usr/sbin:/sbin"

#define _PATH_BSHELL  "/bin/sh"
#define _PATH_CONSOLE "/dev/console"
#define _PATH_CSHELL  "/bin/csh"
#define _PATH_DEVNULL "/dev/null"
#define _PATH_LASTLOG "/var/log/lastlog"
#define _PATH_MAILDIR "/var/mail"
#define _PATH_MAN     "/usr/share/man"
#define _PATH_MNTTAB  "/etc/fstab"
#define _PATH_MOUNTED "/etc/mtab"
#define _PATH_NOLOGIN "/etc/nologin"
#define _PATH_SHELLS  "/etc/shells"
#define _PATH_TTY     "/dev/tty"
#define _PATH_UTMP    "/var/run/utmp"
#define _PATH_VI      "/usr/bin/vi"
#define _PATH_WTMP    "/var/log/wtmp"

#define _PATH_DEV    "/dev/"
#define _PATH_TMP    "/tmp/"
#define _PATH_VARDB  "/var/db/"
#define _PATH_VARRUN "/var/run/"
#define _PATH_VARTMP "/var/tmp/"

#endif
