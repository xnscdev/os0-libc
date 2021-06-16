/* memcpy.c -- This file is part of OS/0 libc.
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

#include <stdint.h>
#include <string.h>

void *
memcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  size_t i;
  if ((uintptr_t) dest % 2 == 0 && (uintptr_t) src % 2 == 0 && len % 2 == 0)
    {
      for (i = 0; i < len / 2; i++)
	((uint16_t *) dest)[i] = ((uint16_t *) src)[i];
    }
  else
    {
      for (i = 0; i < len; i++)
	((unsigned char *) dest)[i] = ((unsigned char *) src)[i];
    }
  return dest;
}

void *
mempcpy (void *__restrict dest, const void *__restrict src, size_t len)
{
  size_t i;
  if ((uintptr_t) dest % 2 == 0 && (uintptr_t) src % 2 == 0 && len % 2 == 0)
    {
      uint16_t *sdest = dest;
      const uint16_t *ssrc = src;
      for (i = 0; i < len / 2; i++)
	*sdest++ = *ssrc++;
      return sdest;
    }
  else
    {
      unsigned char *cdest = dest;
      const unsigned char *csrc = src;
      for (i = 0; i < len; i++)
        *cdest++ = *csrc++;
      return cdest;
    }
}

void *
memmove (void *dest, const void *src, size_t len)
{
  size_t i;
  if (dest < src)
    return memcpy (dest, src, len);
  if ((uintptr_t) dest % 2 == 0 && (uintptr_t) src % 2 == 0 && len % 2 == 0)
    {
      for (i = len / 2; i > 0; i--)
	((uint16_t *) dest)[i - 1] = ((uint16_t *) src)[i - 1];
    }
  else
    {
      for (i = len; i > 0; i--)
	((unsigned char *) dest)[i - 1] = ((unsigned char *) src)[i - 1];
    }
  return dest;
}
