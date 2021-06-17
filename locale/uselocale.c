/* uselocale.c -- This file is part of OS/0 libc.
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
#include <stdlib.h>
#include <string.h>

locale_t
uselocale (locale_t loc)
{
  locale_t temp;
  if (loc == NULL)
    return LC_GLOBAL_LOCALE;
  temp = __libc_locale;
  __libc_locale = loc;
  return temp;
}

locale_t
__libc_get_locale (void)
{
  return __libc_locale;
}
