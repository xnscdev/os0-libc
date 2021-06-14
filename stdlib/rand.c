/* setjmp.c -- This file is part of OS/0 libc.
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

#include <stdlib.h>

/* Linear congruential generator PRNG */

#define LCG_MULT 1103515245
#define LCG_INC  12345

static unsigned int __libc_seed;

int
rand (void)
{
  return rand_r (&__libc_seed);
}

int
rand_r (unsigned int *seed)
{
  unsigned int next = *seed;
  int ret;

  next *= LCG_MULT;
  next += LCG_INC;
  ret = (unsigned int) (next / 65536) % 2048;

  next *= LCG_MULT;
  next += LCG_INC;
  ret ^= (unsigned int) (next / 65536) % 1024;

  next *= LCG_MULT;
  next += LCG_INC;
  ret ^= (unsigned int) (next / 65536) % 1024;

  *seed = next;
  return ret;
}

void
srand (unsigned int seed)
{
  __libc_seed = seed;
}
