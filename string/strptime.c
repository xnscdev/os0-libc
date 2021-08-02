/* strptime.c -- This file is part of OS/0 libc.
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

#include <ctype.h>
#include <libc-locale.h>
#include <string.h>
#include <time.h>

#define PARSE_NUMBER(digits) do			      \
    {						      \
      num = 0;					      \
      for (i = 0; i < (digits); i++)		      \
	{					      \
	  if (isdigit (*buffer))		      \
	    {					      \
	      num *= 10;			      \
	      num += *buffer++ - '0';		      \
	    }					      \
	  else if (i > 0)			      \
	    break;				      \
	  else					      \
	    return NULL; /* Nothing parsed */	      \
	}					      \
    }						      \
  while (0)

char *
strptime (const char *__restrict buffer, const char *__restrict fmt,
	  struct tm *__restrict tp)
{
  const char *save = NULL;
  unsigned int num;
  int found;
  int i;
  while (1)
    {
      if (*fmt == '\0')
	{
	  if (save == NULL)
	    return (char *) buffer;
	  else
	    {
	      fmt = save + 1;
	      save = NULL;
	    }
	}

      if (*fmt == '%')
	{
	  fmt++;
	  switch (*fmt)
	    {
	    case 'a':
	    case 'A':
	      found = 0;
	      for (i = 0; i < 7; i++)
		{
		  char *str = CURRENT_LOCALE (LC_TIME).days[i];
		  size_t len = strlen (str);
		  if (strncmp (buffer, str, len) == 0)
		    {
		      tp->tm_wday = i;
		      found = 1;
		      buffer += len;
		      break;
		    }
		}
	      for (i = 0; i < 7; i++)
		{
		  char *str = CURRENT_LOCALE (LC_TIME).abdays[i];
		  size_t len = strlen (str);
		  if (strncmp (buffer, str, len) == 0)
		    {
		      tp->tm_wday = i;
		      found = 1;
		      buffer += len;
		      break;
		    }
		}
	      if (!found)
		return NULL;
	      break;
	    case 'b':
	    case 'h':
	    case 'B':
	      found = 0;
	      for (i = 0; i < 12; i++)
		{
		  char *str = CURRENT_LOCALE (LC_TIME).months[i];
		  size_t len = strlen (str);
		  if (strncmp (buffer, str, len) == 0)
		    {
		      tp->tm_wday = i;
		      found = 1;
		      buffer += len;
		      break;
		    }
		}
	      for (i = 0; i < 12; i++)
		{
		  char *str = CURRENT_LOCALE (LC_TIME).abmonths[i];
		  size_t len = strlen (str);
		  if (strncmp (buffer, str, len) == 0)
		    {
		      tp->tm_wday = i;
		      found = 1;
		      buffer += len;
		      break;
		    }
		}
	      if (!found)
		return NULL;
	      break;
	    case 'c':
	      save = fmt;
	      fmt = CURRENT_LOCALE (LC_TIME).dt_fmt;
	      continue;
	    case 'C':
	      PARSE_NUMBER (2);
	      tp->tm_year = num * 100 - 1900;
	      break;
	    case 'd':
	    case 'e':
	      PARSE_NUMBER (2);
	      tp->tm_mday = num;
	      break;
	    case 'D':
	      save = fmt;
	      fmt = "%m/%d/%y";
	      continue;
	    case 'H':
	      PARSE_NUMBER (2);
	      tp->tm_hour = num;
	      break;
	    case 'I':
	      PARSE_NUMBER (2);
	      if (num == 0)
		return NULL;
	      tp->tm_hour = (num - 1) % 12;
	      break;
	    case 'j':
	      PARSE_NUMBER (3);
	      if (num == 0)
		return NULL;
	      tp->tm_yday = num;
	      break;
	    case 'm':
	      PARSE_NUMBER (2);
	      if (num == 0)
		return NULL;
	      tp->tm_mon = (num - 1) % 12;
	      break;
	    case 'M':
	      PARSE_NUMBER (2);
	      if (num >= 60)
		return NULL;
	      tp->tm_min = num;
	      break;
	    case 'n':
	    case 't':
	      while (isspace (*buffer))
		buffer++;
	      break;
	    case 'p':
	      num = strlen (CURRENT_LOCALE (LC_TIME).am);
	      if (strncmp (buffer, CURRENT_LOCALE (LC_TIME).am, num) == 0)
		buffer += num;
	      else
		{
		  num = strlen (CURRENT_LOCALE (LC_TIME).pm);
		  if (strncmp (buffer, CURRENT_LOCALE (LC_TIME).pm, num) == 0)
		    {
		      buffer += num;
		      tp->tm_hour += 12;
		    }
		  else
		    return NULL;
		}
	      break;
	    case 'r':
	      save = fmt;
	      fmt = *CURRENT_LOCALE (LC_TIME).t_fmt_ampm == '\0' ?
		CURRENT_LOCALE (LC_TIME).t_fmt :
		CURRENT_LOCALE (LC_TIME).t_fmt_ampm;
	      continue;
	    case 'R':
	      save = fmt;
	      fmt = "%H : %M";
	      continue;
	    case 'S':
	      PARSE_NUMBER (2);
	      if (num >= 60)
		return NULL;
	      tp->tm_sec = num;
	      break;
	    case 'T':
	      save = fmt;
	      fmt = "%H : %M : %S";
	      continue;
	    case 'U':
	    case 'W':
	      /* Ignore whatever was parsed, since struct tm does not record
		 the week of the year */
	      PARSE_NUMBER (2);
	      break;
	    case 'w':
	      if (*buffer < '0' || *buffer >= '6')
		return NULL;
	      tp->tm_wday = *buffer++ - '0';
	      break;
	    case 'x':
	      save = fmt;
	      fmt = CURRENT_LOCALE (LC_TIME).d_fmt;
	      continue;
	    case 'X':
	      save = fmt;
	      fmt = CURRENT_LOCALE (LC_TIME).t_fmt;
	      continue;
	    case 'y':
	      PARSE_NUMBER (2);
	      if (num >= 69)
		tp->tm_year = num + 1900;
	      else
		tp->tm_year = num + 2000;
	      break;
	    case 'Y':
	      PARSE_NUMBER (4);
	      tp->tm_year = num;
	      break;
	    case '%':
	      if (*buffer++ != '%')
		return NULL;
	      break;
	    default:
	      return NULL;
	    }
	}
      else if (isspace (*fmt))
	{
	  while (isspace (*buffer))
	    buffer++;
	}
      else if (*fmt != *buffer++)
	return NULL;
      fmt++;
    }
}
