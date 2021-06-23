/* print.c -- This file is part of OS/0 libc.
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

#include <rtld.h>
#include <stdint.h>
#include <unistd.h>

#define DIGITS								\
  "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"

static char *
utoa (uintptr_t value, char *result, int base)
{
  char *ptr = result;
  char *rev = result;
  uintptr_t temp;
  char c;

  if (base < 2 || base > 36)
    {
      *result = '\0';
      return result;
    }

  do
    {
      temp = value;
      value /= base;
      *ptr++ = DIGITS[35 + (temp - value * base)];
    }
  while (value != 0);

  *ptr-- = '\0';
  while (rev < ptr)
    {
      c = *ptr;
      *ptr-- = *rev;
      *rev++ = c;
    }
  return result;
}

void
__rtld_print (const char *msg)
{
  size_t len = 0;
  while (msg[len] != '\0')
    len++;
  __rtld_syscall (SYS_write, STDERR_FILENO, msg, len);
}

void
__rtld_print_ptr (void *ptr)
{
  char buffer[16];
  __rtld_print ("0x");
  utoa ((uintptr_t) ptr, buffer, 16);
  __rtld_print (buffer);
}
