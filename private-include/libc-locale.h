/* libc-locale.h -- This file is part of OS/0 libc.
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

#ifndef _LIBC_LOCALE_H
#define _LIBC_LOCALE_H

#include <sys/cdefs.h>
#include <locale.h>
#include <locks.h>
#include <stddef.h>

#define LOCALE_BUFFER_SIZE 1024

#define CURRENT_LOCALE(category) (__libc_locale->__ ## category)

__BEGIN_DECLS

extern const struct __locale __libc_posix_locale __hidden;
extern locale_t __libc_locale __hidden;
extern __lock_t __libc_locale_lock __hidden;
extern mbstate_t __libc_mbstate __hidden;

/* POSIX C locale functions */

int __libc_posix_iswalnum (wint_t wc) __hidden;
int __libc_posix_iswalpha (wint_t wc) __hidden;
int __libc_posix_iswblank (wint_t wc) __hidden;
int __libc_posix_iswcntrl (wint_t wc) __hidden;
int __libc_posix_iswdigit (wint_t wc) __hidden;
int __libc_posix_iswgraph (wint_t wc) __hidden;
int __libc_posix_iswlower (wint_t wc) __hidden;
int __libc_posix_iswprint (wint_t wc) __hidden;
int __libc_posix_iswpunct (wint_t wc) __hidden;
int __libc_posix_iswspace (wint_t wc) __hidden;
int __libc_posix_iswupper (wint_t wc) __hidden;
int __libc_posix_iswxdigit (wint_t wc) __hidden;

wint_t __libc_posix_towlower (wint_t wc) __hidden;
wint_t __libc_posix_towupper (wint_t wc) __hidden;

size_t __libc_posix_mbrtowc (wchar_t *__restrict pwc,
			     const char *__restrict str, size_t len,
			     mbstate_t *__restrict ps) __hidden;
size_t __libc_posix_wcrtomb (char *__restrict str, wchar_t wc,
			     mbstate_t *__restrict ps) __hidden;

void __libc_locale_set_ctype_posix (void) __hidden;
void __libc_locale_set_monetary_posix (void) __hidden;
void __libc_locale_set_numeric_posix (void) __hidden;

__END_DECLS

#endif
