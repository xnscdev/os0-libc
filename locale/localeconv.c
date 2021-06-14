/* localeconv.c -- This file is part of OS/0 libc.
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

static struct lconv __libc_lconv;

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
