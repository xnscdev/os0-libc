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

static char __libc_locale_buf[LOCALE_BUFFER_SIZE];
static struct __locale __libc_current_locale;

locale_t __libc_locale = &__libc_current_locale;
__lock_t __libc_locale_lock;

/* TODO Support setting other locales using files in /usr/share/locale */

static int
__set_lc_collate (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale->__names[LC_COLLATE] = locale;
  return 0;
}

static int
__set_lc_ctype (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale_set_ctype_posix ();
  __libc_locale->__names[LC_CTYPE] = locale;
  return 0;
}

static int
__set_lc_messages (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale->__names[LC_MESSAGES] = locale;
  return 0;
}

static int
__set_lc_monetary (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale_set_monetary_posix ();
  __libc_locale->__names[LC_MONETARY] = locale;
  return 0;
}

static int
__set_lc_numeric (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale_set_numeric_posix ();
  __libc_locale->__names[LC_NUMERIC] = locale;
  return 0;
}

static int
__set_lc_time (const char *locale)
{
  if (strcmp (locale, "C") != 0)
    return -1;

  __libc_locale->__names[LC_TIME] = locale;
  return 0;
}

static int
__set_locale (int category, const char *locale)
{
  int ret = -1;
  if (*locale == '\0' || strcmp (locale, "C") == 0
      || strcmp (locale, "POSIX") == 0)
    locale = "C";
  else /* TODO Check files in /usr/share/locale */
    return -1;

  __libc_lock (&__libc_locale_lock);
  switch (category)
    {
    case LC_COLLATE:
      ret = __set_lc_collate (locale);
      break;
    case LC_CTYPE:
      ret = __set_lc_ctype (locale);
      break;
    case LC_MESSAGES:
      ret = __set_lc_messages (locale);
      break;
    case LC_MONETARY:
      ret = __set_lc_monetary (locale);
      break;
    case LC_NUMERIC:
      ret = __set_lc_numeric (locale);
      break;
    case LC_TIME:
      ret = __set_lc_time (locale);
      break;
    }
  __libc_unlock (&__libc_locale_lock);
  return ret;
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
  const char *var = getenv ("LC_ALL");
  if (var != NULL)
    locale = var;
  else
    {
      var = getenv ("LANG");
      if (var != NULL)
	locale = var;
      else
	locale = "C";
    }
  __set_lc_all (locale);

  var = getenv ("LC_COLLATE");
  if (var != NULL)
    __set_lc_collate (var);

  var = getenv ("LC_CTYPE");
  if (var != NULL)
    __set_lc_ctype (var);

  var = getenv ("LC_MESSAGES");
  if (var != NULL)
    __set_lc_messages (var);

  var = getenv ("LC_MONETARY");
  if (var != NULL)
    __set_lc_monetary (var);

  var = getenv ("LC_NUMERIC");
  if (var != NULL)
    __set_lc_numeric (var);

  var = getenv ("LC_TIME");
  if (var != NULL)
    __set_lc_time (var);
}
