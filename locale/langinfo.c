/* langinfo.c -- This file is part of OS/0 libc.
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

#include <langinfo.h>
#include <libc-locale.h>
#include <string.h>

#define SET_STRING(s) strcpy (__libc_langinfo_buf, (s))

static char __libc_langinfo_buf[64];

char *
nl_langinfo (nl_item item)
{
  switch (item)
    {
    case CODESET:
      return SET_STRING (CURRENT_LOCALE (LC_CTYPE).codeset);
    case D_T_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).dt_fmt);
    case D_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).d_fmt);
    case T_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).t_fmt);
    case T_FMT_AMPM:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).t_fmt_ampm);
    case AM_STR:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).am);
    case PM_STR:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).pm);
    case ERA:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).era);
    case ERA_D_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).era_d_fmt);
    case ERA_D_T_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).era_dt_fmt);
    case ERA_T_FMT:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).era_t_fmt);
    case ALT_DIGITS:
      return SET_STRING (CURRENT_LOCALE (LC_TIME).altdigits);
    case RADIXCHAR:
      return SET_STRING (__libc_locale->__lconv.decimal_point);
    case THOUSEP:
      return SET_STRING (__libc_locale->__lconv.thousands_sep);
    case YESEXPR:
      return SET_STRING (CURRENT_LOCALE (LC_MESSAGES).yesexpr);
    case NOEXPR:
      return SET_STRING (CURRENT_LOCALE (LC_MESSAGES).noexpr);
    case CRNCYSTR:
      return SET_STRING (CURRENT_LOCALE (LC_MONETARY).currsym);
    default:
      if (item >= DAY_1 && item <= DAY_7)
	return SET_STRING (CURRENT_LOCALE (LC_TIME).days[item - DAY_1]);
      else if (item >= ABDAY_1 && item <= ABDAY_7)
	return SET_STRING (CURRENT_LOCALE (LC_TIME).abdays[item - ABDAY_1]);
      else if (item >= MON_1 && item <= MON_12)
	return SET_STRING (CURRENT_LOCALE (LC_TIME).months[item - MON_1]);
      else if (item >= ABMON_1 && item <= ABMON_12)
	return SET_STRING (CURRENT_LOCALE (LC_TIME).abmonths[item - ABMON_1]);
      return SET_STRING ("");
    }
}
