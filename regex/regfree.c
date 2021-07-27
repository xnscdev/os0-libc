/* regfree.c -- This file is part of OS/0 libc.
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

/* Copyright 1992, 1993, 1994, 1997 Henry Spencer.  All rights reserved.
   This software is not subject to any license of the American Telephone
   and Telegraph Company or of the Regents of the University of California.

   Permission is granted to anyone to use this software for any purpose on
   any computer system, and to alter it and redistribute it, subject
   to the following restrictions:

   1. The author is not responsible for the consequences of use of this
      software, no matter how awful, even if they arise from flaws in it.

   2. The origin of this software must not be misrepresented, either by
      explicit claim or by omission.  Since few users ever read sources,
      credits must appear in the documentation.

   3. Altered versions must be plainly marked as such, and must not be
      misrepresented as being the original software.  Since few users
      ever read sources, credits must appear in the documentation.

   4. This notice may not be removed or altered. */

/* Derived from Henry Spencer's regex implementation */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "utils.h"
#include "regex2.h"

/*
 - regfree - free everything
 = extern void regfree(regex_t *);
 */
void
regfree (regex_t *preg)
{
  register struct re_guts *g;

  if (preg->re_magic != MAGIC1)	/* oops */
    return;			/* nice to complain, but hard */

  g = preg->re_g;
  if (g == NULL || g->magic != MAGIC2)	/* oops again */
    return;
  preg->re_magic = 0;		/* mark it invalid */
  g->magic = 0;			/* mark it invalid */

  if (g->strip != NULL)
    free (g->strip);
  if (g->sets != NULL)
    free (g->sets);
  if (g->setbits != NULL)
    free (g->setbits);
  if (g->must != NULL)
    free (g->must);
  free (g);
}
