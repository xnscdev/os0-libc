/* posix.c -- This file is part of OS/0 libc.
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

#include <ctype.h>
#include <libc-locale.h>
#include <limits.h>
#include <string.h>

const struct __locale __libc_posix_locale = {
  .__lconv = {
    ".", "", "", "", "", "", "", "", "", "",
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX
  },
  .__LC_CTYPE = {
    {
      (int (*) (wint_t)) isalnum,
      (int (*) (wint_t)) isalpha,
      (int (*) (wint_t)) isblank,
      (int (*) (wint_t)) iscntrl,
      (int (*) (wint_t)) isdigit,
      (int (*) (wint_t)) isgraph,
      (int (*) (wint_t)) islower,
      (int (*) (wint_t)) isprint,
      (int (*) (wint_t)) ispunct,
      (int (*) (wint_t)) isspace,
      (int (*) (wint_t)) isupper,
      (int (*) (wint_t)) isxdigit,
    },
    {"tolower", "toupper"},
    {(wint_t (*) (wint_t)) tolower, (wint_t (*) (wint_t)) toupper},
    __libc_posix_mbrtowc,
    __libc_posix_wcrtomb
  },
  .__names = {"C", "C", "C", "C", "C", "C"}
};

size_t
__libc_posix_mbrtowc (wchar_t *__restrict pwc, const char *__restrict str,
		      size_t len, mbstate_t *__restrict ps)
{
  if (pwc != NULL)
    *pwc = *str;
  memset (ps, 0, sizeof (mbstate_t));
  return *str != '\0';
}

size_t
__libc_posix_wcrtomb (char *__restrict str, wchar_t wc,
		      mbstate_t *__restrict ps)
{
  if (str != NULL)
    *str = (char) wc;
  memset (ps, 0, sizeof (mbstate_t));
  return 1;
}

void
__libc_locale_set_ctype_posix (void)
{
  memcpy (&__libc_locale->__LC_CTYPE, &__libc_posix_locale.__LC_CTYPE,
	  sizeof (struct __locale_ctype_data));
}

void
__libc_locale_set_monetary_posix (void)
{
  struct lconv *l = &__libc_locale->__lconv;
  l->int_curr_symbol = "";
  l->currency_symbol = "";
  l->mon_decimal_point = "";
  l->mon_thousands_sep = "";
  l->mon_grouping = "";
  l->positive_sign = "";
  l->negative_sign = "";
  l->int_frac_digits = CHAR_MAX;
  l->frac_digits = CHAR_MAX;
  l->p_cs_precedes = CHAR_MAX;
  l->p_sep_by_space = CHAR_MAX;
  l->n_cs_precedes = CHAR_MAX;
  l->n_sep_by_space = CHAR_MAX;
  l->p_sign_posn = CHAR_MAX;
  l->n_sign_posn = CHAR_MAX;
  l->int_p_cs_precedes = CHAR_MAX;
  l->int_p_sep_by_space = CHAR_MAX;
  l->int_n_cs_precedes = CHAR_MAX;
  l->int_n_sep_by_space = CHAR_MAX;
  l->int_p_sign_posn = CHAR_MAX;
  l->int_n_sign_posn = CHAR_MAX;
}

void
__libc_locale_set_numeric_posix (void)
{
  struct lconv *l = &__libc_locale->__lconv;
  l->decimal_point = ".";
  l->thousands_sep = "";
  l->grouping = "";
}
