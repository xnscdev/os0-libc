/* strerror.c -- This file is part of OS/0 libc.
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

#include <errno.h>
#include <string.h>

const char *const sys_errlist[] = {
  [EPERM] = "Operation not permitted",
  [ENOENT] = "No such file or directory",
  [ESRCH] = "No such process",
  [EINTR] = "Interrupted system call",
  [EIO] = "Input/output error",
  [ENXIO] = "No such device or address",
  [E2BIG] = "Argument list too long",
  [ENOEXEC] = "Exec format error",
  [EBADF] = "Bad file descriptor",
  [ECHILD] = "No child processes",
  [EAGAIN] = "Resource temporarily unavailable",
  [ENOMEM] = "Cannot allocate memory",
  [EACCES] = "Permission denied",
  [EFAULT] = "Bad address",
  [ENOTBLK] = "Block device required",
  [EBUSY] = "Device or resource busy",
  [EEXIST] = "File exists",
  [EXDEV] = "Invalid cross-device link",
  [ENODEV] = "No such device",
  [ENOTDIR] = "Not a directory",
  [EISDIR] = "Is a directory",
  [EINVAL] = "Invalid argument",
  [ENFILE] = "Too many open files",
  [EMFILE] = "Too many open files in system",
  [ENOTTY] = "Inappropriate ioctl for device",
  [ETXTBSY] = "Text file busy",
  [EFBIG] = "File too large",
  [ENOSPC] = "No space left on device",
  [ESPIPE] = "Illegal seek",
  [EROFS] = "Read-only file system",
  [EMLINK] = "Too many links",
  [EPIPE] = "Broken pipe",
  [EDOM] = "Numerical argument out of domain",
  [ERANGE] = "Numerical result out of range",
  [EDEADLK] = "Resource deadlock avoided",
  [ENAMETOOLONG] = "File name too long",
  [ENOLCK] = "No locks available",
  [ENOSYS] = "Function not implemented",
  [ENOTEMPTY] = "Directory not empty",
  [ELOOP] = "Too many levels of symbolic links",
  [ENOSTR] = "Device not a stream",
  [ENODATA] = "No data available",
  [ETIME] = "Timer expired",
  [ENOSR] = "Out of streams resources",
  [ENONET] = "Machine is not on the network",
  [ENOPKG] = "Package not installed",
  [EREMOTE] = "Remote I/O error",
  [ENOLINK] = "Link has been severed",
  [EADV] = "Advertise error",
  [ESRMNT] = "Srmount error",
  [ECOMM] = "Communication error on send",
  [EPROTO] = "Protocol error",
  [EMULTIHOP] = "Multihop attempted",
  [EOVERFLOW] = "Value too large for defined data type",
  [ENOTUNIQ] = "Name not unique on the network",
  [EBADFD] = "File descriptor in bad state",
  [EREMCHG] = "Remote address changed",
  [ELIBACC] = "Can not access a needed shared library", /* sic */
  [ELIBBAD] = "Accessing a corrupt shared library",
  [ELIBMAX] = "Attempting to link in too many shared libraries",
  [ELIBEXEC] = "Cannot exec a shared library directly",
  [EILSEQ] = "Invalid or incomplete multibyte or wide character",
  [ERESTART] = "Interrupted system call should be restarted",
  [ESTRPIPE] = "Streams pipe error",
  [EUSERS] = "Too many users",
  [ENOTSOCK] = "Socket operation on non-socket",
  [EDESTADDRREQ] = "Destination address required",
  [EMSGSIZE] = "Message too long",
  [EPROTOTYPE] = "Protocol wrong type for socket",
  [ENOPROTOOPT] = "Protocol not available",
  [EPROTONOSUPPORT] = "Protocol not supported",
  [ESOCKTNOSUPPORT] = "Socket type not supported",
  [EOPNOTSUPP] = "Operation not supported",
  [EPFNOSUPPORT] = "Protocol family not supported",
  [EAFNOSUPPORT] = "Address family not supported by protocol",
  [EADDRINUSE] = "Address already in use",
  [EADDRNOTAVAIL] = "Cannot assign requested address",
  [ENETDOWN] = "Network is down",
  [ENETUNREACH] = "Network is unreachable",
  [ENETRESET] = "Network dropped connection on reset",
  [ECONNABORTED] = "Software caused connection abort",
  [ECONNRESET] = "Connection reset by peer",
  [ENOBUFS] = "No buffer space available",
  [EISCONN] = "Transport endpoint is already connected",
  [ENOTCONN] = "Transport endpoint is not connected",
  [ESHUTDOWN] = "Cannot send after transport endpoint shutdown",
  [ETOOMANYREFS] = "Too many references: cannot splice",
  [ETIMEDOUT] = "Connection timed out",
  [ECONNREFUSED] = "Connection refused",
  [EHOSTDOWN] = "Host is down",
  [EHOSTUNREACH] = "No route to host",
  [EALREADY] = "Operation already in progress",
  [EINPROGRESS] = "Operation now in progress",
  [ESTALE] = "Stale file handle",
  [EUCLEAN] = "Structure needs cleaning",
  [EREMOTEIO] = "Remote I/O error",
  [EDQUOT] = "Disk quota exceeded"
};

const int sys_nerr = __NR_errno;

char *
strerror (int err)
{
  if (err < 0 || err >= sys_nerr)
    return NULL;
  return (char *) sys_errlist[err];
}

char *
strerror_r (int err, char *buffer, size_t len)
{
  if (err < 0 || err > sys_nerr)
    return NULL;
  strncpy (buffer, sys_errlist[err], len - 1);
  buffer[len - 1] = '\0';
  return buffer;
}
