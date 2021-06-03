/* strcasestr.c -- This file is part of OS/0 libc.
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

/* Return the offset of one string within another.
   Copyright (C) 1994-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Adapted from the GNU C Library */

#include <ctype.h>
#include <string.h>
#include <strings.h>

/* Two-Way algorithm.  */
#define RETURN_TYPE char *
#define AVAILABLE(h, h_l, j, n_l)				 \
  (((j) + (n_l) <= (h_l))					 \
   || ((h_l) += strnlen ((void *) ((h) + (h_l)), (n_l) + 512),	 \
       (j) + (n_l) <= (h_l)))
#define CHECK_EOL (1)
#define RET0_IF_0(a) if (!a) goto ret0
#define CANON_ELEMENT(c) tolower (c)
#define CMP_FUNC(p1, p2, l)					\
  strncasecmp ((const char *) (p1), (const char *) (p2), l)

#include "strstr-util.h"

/* Find the first occurrence of NEEDLE in HAYSTACK, using
   case-insensitive comparison.  This function gives unspecified
   results in multibyte locales.  */
char *
strcasestr (const char *haystack, const char *needle)
{
  size_t needle_len; /* Length of NEEDLE.  */
  size_t haystack_len; /* Known minimum length of HAYSTACK.  */
  /* Handle empty NEEDLE special case.  */
  if (needle[0] == '\0')
    return (char *) haystack;
  /* Ensure HAYSTACK length is at least as long as NEEDLE length.
     Since a match may occur early on in a huge HAYSTACK, use strnlen
     and read ahead a few cachelines for improved performance.  */
  needle_len = strlen (needle);
  haystack_len = strnlen (haystack, needle_len + 256);
  if (haystack_len < needle_len)
    return NULL;
  /* Perform the search.  Abstract memory is considered to be an array
     of 'unsigned char' values, not an array of 'char' values.  See
     ISO C 99 section 6.2.6.1.  */
  if (needle_len < LONG_NEEDLE_THRESHOLD)
    return two_way_short_needle ((const unsigned char *) haystack,
                                 haystack_len,
                                 (const unsigned char *) needle,
                                 needle_len);
  return two_way_long_needle ((const unsigned char *) haystack, haystack_len,
                              (const unsigned char *) needle,
                              needle_len);
}
