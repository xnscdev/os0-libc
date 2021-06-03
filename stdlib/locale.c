/* locale.c -- This file is part of OS/0 libc.
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

#include <limits.h>
#include <locale.h>
#include <setup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCALE_BUFFER_SIZE 1024

static const char *__libc_locales[LC_ALL];
static char __libc_locale_buf[LOCALE_BUFFER_SIZE];
static struct lconv __libc_lconv;

static char *
__set_lc_all (const char *locale)
{
  int i;
  if (locale != NULL)
    {
      if (*locale == '\0' || strcmp (locale, "C") == 0
	  || strcmp (locale, "POSIX") == 0)
	locale = "C";
      else
	return NULL; /* Only supported locale is C */
      for (i = 0; i < LC_ALL; i++)
	__libc_locales[i] = locale;
      goto end;
    }

  /* Return a string describing all of the locales. If all locale categories
     have equivalent locales, return that locale, otherwise return a composite
     locale consisting of all of the categories' values separated by `;' */
  locale = __libc_locales[0];
  for (i = 1; i < LC_ALL; i++)
    {
      if (strcmp (__libc_locales[i], locale) != 0)
	{
	  snprintf (__libc_locale_buf, LOCALE_BUFFER_SIZE,
		    "LC_COLLATE=%s;LC_CTYPE=%s;LC_MESSAGES=%s;LC_MONETARY=%s;"
		    "LC_NUMERIC=%s;LC_TIME=%s", __libc_locales[LC_COLLATE],
		    __libc_locales[LC_CTYPE], __libc_locales[LC_MESSAGES],
		    __libc_locales[LC_MONETARY], __libc_locales[LC_NUMERIC],
		    __libc_locales[LC_TIME]);
	  return __libc_locale_buf;
	}
    }

 end:
  strncpy (__libc_locale_buf, locale, LOCALE_BUFFER_SIZE - 1);
  __libc_locale_buf[LOCALE_BUFFER_SIZE - 1] = '\0';
  return __libc_locale_buf;
}

struct lconv *
localeconv (void)
{
  __libc_lconv.decimal_point = ".";
  __libc_lconv.thousands_sep = "";
  __libc_lconv.grouping = "";
  __libc_lconv.int_curr_symbol = "";
  __libc_lconv.currency_symbol = "";
  __libc_lconv.mon_decimal_point = "";
  __libc_lconv.mon_thousands_sep = "";
  __libc_lconv.mon_grouping = "";
  __libc_lconv.positive_sign = "";
  __libc_lconv.negative_sign = "";
  __libc_lconv.int_frac_digits = CHAR_MAX;
  __libc_lconv.frac_digits = CHAR_MAX;
  __libc_lconv.p_cs_precedes = CHAR_MAX;
  __libc_lconv.p_sep_by_space = CHAR_MAX;
  __libc_lconv.n_cs_precedes = CHAR_MAX;
  __libc_lconv.n_sep_by_space = CHAR_MAX;
  __libc_lconv.p_sign_posn = CHAR_MAX;
  __libc_lconv.n_sign_posn = CHAR_MAX;
  __libc_lconv.int_p_cs_precedes = CHAR_MAX;
  __libc_lconv.int_p_sep_by_space = CHAR_MAX;
  __libc_lconv.int_n_cs_precedes = CHAR_MAX;
  __libc_lconv.int_n_sep_by_space = CHAR_MAX;
  __libc_lconv.int_p_sign_posn = CHAR_MAX;
  __libc_lconv.int_n_sign_posn = CHAR_MAX;
  return &__libc_lconv;
}

char *
setlocale (int category, const char *locale)
{
  /* TODO Implement */
  if (category < 0 || category > LC_ALL)
    return NULL;
  if (category == LC_ALL)
    return __set_lc_all (locale);
  if (*locale == '\0' || strcmp (locale, "C") == 0
      || strcmp (locale, "POSIX") == 0)
    locale = "C";
  else
    return NULL; /* Only supported locale is C */
  __libc_locales[category] = locale;
  strncpy (__libc_locale_buf, locale, LOCALE_BUFFER_SIZE - 1);
  __libc_locale_buf[LOCALE_BUFFER_SIZE - 1] = '\0';
  return __libc_locale_buf;
}

void
__libc_setup_locale (void)
{
  int i;
  const char *lang = getenv ("LANG");
  if (lang == NULL)
    lang = "C";
  for (i = 0; i < LC_ALL; i++)
    __libc_locales[i] = lang;
}
