/* langinfo.h -- This file is part of OS/0 libc.
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

#ifndef _LANGINFO_H
#define _LANGINFO_H

#include <nl_types.h>

enum
{
  CODESET,
  D_T_FMT,
  D_FMT,
  T_FMT,
  T_FMT_AMPM,
  AM_STR,
  PM_STR,
  DAY_1,
  DAY_2,
  DAY_3,
  DAY_4,
  DAY_5,
  DAY_6,
  DAY_7,
  ABDAY_1,
  ABDAY_2,
  ABDAY_3,
  ABDAY_4,
  ABDAY_5,
  ABDAY_6,
  ABDAY_7,
  MON_1,
  MON_2,
  MON_3,
  MON_4,
  MON_5,
  MON_6,
  MON_7,
  MON_8,
  MON_9,
  MON_10,
  MON_11,
  MON_12,
  ABMON_1,
  ABMON_2,
  ABMON_3,
  ABMON_4,
  ABMON_5,
  ABMON_6,
  ABMON_7,
  ABMON_8,
  ABMON_9,
  ABMON_10,
  ABMON_11,
  ABMON_12,
  ERA,
  ERA_D_FMT,
  ERA_D_T_FMT,
  ERA_T_FMT,
  ALT_DIGITS,
  RADIXCHAR,
  THOUSEP,
  YESEXPR,
  NOEXPR,
  CRNCYSTR
};

__BEGIN_DECLS

char *nl_langinfo (nl_item item);

__END_DECLS

#endif
