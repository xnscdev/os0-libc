/* fgetws.c -- This file is part of OS/0 libc.
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

#include <stdio.h>
#include <unistd.h>

wchar_t *
fgetws (wchar_t *__restrict ws, int size, FILE *__restrict stream)
{
  wchar_t *ret;
  flockfile (stream);
  ret = fgetws_unlocked (ws, size, stream);
  funlockfile (stream);
  return ret;
}

wchar_t *
fgetws_unlocked (wchar_t *__restrict ws, int size, FILE *__restrict stream)
{
  int i;
  for (i = 0; i < size - 1; i++)
    {
      wchar_t c = fgetwc_unlocked (stream);
      if (feof (stream) || ferror (stream))
	break;
      ws[i] = c;
      if (c == '\n')
	{
	  i++;
	  break;
	}
    }
  ws[i] = L'\0';
  return ws;
}
