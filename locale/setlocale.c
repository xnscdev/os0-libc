/* setlocale.c -- This file is part of OS/0 libc.
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

#include <libc-locale.h>
#include <locks.h>
#include <setup.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static locale_t __libc_locale = &__libc_posix_locale;
static char __libc_locale_buf[LOCALE_BUFFER_SIZE];

__lock_t __libc_locale_lock;

/* TODO Support setting other locales using files in /usr/share/locale */

static int
__set_locale (int category, const char *locale)
{
  if (*locale == '\0' || strcmp (locale, "C") == 0
      || strcmp (locale, "POSIX") == 0)
    locale = "C";
  else /* TODO Check files in /usr/share/locale */
    return -1;

  __libc_lock (&__libc_locale_lock);
  if (strcmp (locale, "C") == 0)
    {
      __libc_locale->__locales[category].mbrtowc = __libc_posix_mbrtowc;
      __libc_locale->__locales[category].wcrtomb = __libc_posix_wcrtomb;
    }
  else
    ; /* TODO Read contents of locale files in /usr/share/locale */
  __libc_locale->__names[category] = locale;
  __libc_unlock (&__libc_locale_lock);
  return 0;
}

static char *
__set_lc_all (const char *locale)
{
  int i;
  if (locale != NULL)
    {
      for (i = 0; i < LC_ALL; i++)
	{
	  if (__set_locale (i, locale) == -1)
	    return NULL;
	}
      goto end;
    }

  /* Return a string describing all of the locales. If all locale categories
     have equivalent locales, return that locale, otherwise return a composite
     locale consisting of all of the categories' values separated by `;' */
  locale = __libc_locale->__names[0];
  for (i = 1; i < __LC_LAST; i++)
    {
      if (strcmp (__libc_locale->__names[i], locale) != 0)
	{
	  snprintf (__libc_locale_buf, LOCALE_BUFFER_SIZE,
		    "LC_COLLATE=%s;LC_CTYPE=%s;LC_MESSAGES=%s;LC_MONETARY=%s;"
		    "LC_NUMERIC=%s;LC_TIME=%s",
		    __libc_locale->__names[LC_COLLATE],
		    __libc_locale->__names[LC_CTYPE],
		    __libc_locale->__names[LC_MESSAGES],
		    __libc_locale->__names[LC_MONETARY],
		    __libc_locale->__names[LC_NUMERIC],
		    __libc_locale->__names[LC_TIME]);
	  return __libc_locale_buf;
	}
    }

 end:
  strncpy (__libc_locale_buf, locale, LOCALE_BUFFER_SIZE - 1);
  __libc_locale_buf[LOCALE_BUFFER_SIZE - 1] = '\0';
  return __libc_locale_buf;
}

char *
setlocale (int category, const char *locale)
{
  /* TODO Implement */
  if (category < 0 || category > LC_ALL)
    return NULL;
  if (category == LC_ALL)
    return __set_lc_all (locale);

  if (__set_locale (category, locale) == -1)
    return NULL;

  strncpy (__libc_locale_buf, locale, LOCALE_BUFFER_SIZE - 1);
  __libc_locale_buf[LOCALE_BUFFER_SIZE - 1] = '\0';
  return __libc_locale_buf;
}

void
__libc_setup_locale (void)
{
  const char *locale;
  const char *lc_all = getenv ("LC_ALL");
  if (lc_all != NULL)
    locale = lc_all;
  else
    {
      const char *lang = getenv ("LANG");
      if (lang != NULL)
	locale = lang;
      else
	locale = "C";
    }
  __set_lc_all (locale);
}
