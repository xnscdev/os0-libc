/* fgets.c -- This file is part of OS/0 libc.
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

char *
fgets (char *__restrict str, int size, FILE *__restrict stream)
{
  char *ret;
  flockfile (stream);
  ret = fgets_unlocked (str, size, stream);
  funlockfile (stream);
  return ret;
}

char *
fgets_unlocked (char *__restrict str, int size, FILE *__restrict stream)
{
  int i;
  for (i = 0; i < size - 1; i++)
    {
      char c = fgetc_unlocked (stream);
      if (feof (stream) || ferror (stream))
	break;
      str[i] = c;
      if (c == '\n')
	{
	  i++;
	  break;
	}
    }
  str[i] = '\0';
  return str;
}

char *
gets (char *str)
{
  int i;
  for (i = 0; ; i++)
    {
      char c = getchar ();
      if (feof (stdin) || ferror (stdin) || c == '\n')
	break;
      str[i] = c;
    }
  str[i] = '\0';
  return str;
}
