/* regex.c -- This file is part of OS/0 libc.
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

#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#define RE_MEMCHECK(x) do			\
    {						\
      if ((x) == NULL)				\
        return REG_ESPACE;			\
    }						\
  while (0)

#define RE_RETCHECK(x) do			\
    {						\
      int __r = (x);				\
      if (__r != 0)				\
	return __r;				\
    }						\
  while (0)

static void __re_free_regex (struct __re_regex *re);

static void
__re_free_node (struct __re_node *node)
{
  switch (node->type)
    {
    case __RE_CHARS:
      free (node->chars);
      break;
    case __RE_SUBEXPR:
      __re_free_regex (node->subexpr);
      free (node->subexpr);
      break;
    default:
      break;
    }
}

static void
__re_free_regex (struct __re_regex *re)
{
  size_t i;
  for (i = 0; i < re->count; i++)
    __re_free_node (&re->nodes[i]);
  free (re->nodes);
}

static int
__re_new_node (struct __re_regex *re)
{
  struct __re_node *nodes =
    realloc (re->nodes, sizeof (struct __re_node) * ++re->count);
  RE_MEMCHECK (nodes);
  re->nodes = nodes;
  return 0;
}

static int
__re_compile_bracket (struct __re_regex *re, const char **str)
{
  struct __re_node *n;
  const char *ptr = *str + 1;
  struct __re_chars *chars;
  size_t alloc = 0;
  int linking = 0;

  RE_RETCHECK (__re_new_node (re));
  n = &re->nodes[re->count - 1];
  n->type = __RE_CHARS;
  n->nitems = 0;
  n->exclude = 0;
  n->chars = NULL;

  /* Check for negated class */
  if (*ptr == '^')
    {
      n->exclude = 1;
      ptr++;
    }

  /* ] as the first token in a bracket expression is taken literally */
  if (*ptr == ']')
    {
      alloc = 4;
      chars = malloc (sizeof (struct __re_chars) * alloc);
      RE_MEMCHECK (chars);
      n->chars = chars;
      n->chars[0].min = ']';
      n->chars[0].max = ']';
      n->nitems++;
      ptr++;
    }

  while (*ptr != ']' && *ptr != '\0')
    {
      /* Hyphen used as range operator */
      if (*ptr == '-' && n->nitems > 0 && !linking)
	{
	  linking = 1;
	  goto next;
	}

      /* Allocate more space for character entries if needed */
      if (n->nitems == alloc)
	{
	  alloc += 4;
	  chars = realloc (n->chars, sizeof (struct __re_chars) * alloc);
	  RE_MEMCHECK (chars);
	  n->chars = chars;
	}

      /* POSIX bracket expression */
      if (*ptr == '[')
	{
	  switch (*++ptr)
	    {
	    case ':':
	      {
		const char *p = ++ptr;
		char *s;
		size_t len;

		/* If we expected a normal entry, add the - entry first */
		if (linking)
		  {
		    n->chars[n->nitems].wctype = 0;
		    n->chars[n->nitems].min = '-';
		    n->chars[n->nitems].max = '-';
		    if (++n->nitems == alloc)
		      {
			alloc += 4;
			chars = realloc (n->chars, sizeof (struct __re_chars) *
					 alloc);
			RE_MEMCHECK (chars);
			n->chars = chars;
		      }
		  }

		/* Search for the end of the character class name */
		while (isalpha (*p))
		  p++;
		if (p[0] != ':' || p[1] != ']')
		  break; /* Doesn't match form [:class:], just parse normally */

		/* Determine the character class */
		len = p - ptr;
		s = malloc (len + 1);
		strncpy (s, ptr, len);
		s[len] = '\0';
		n->chars[n->nitems].wctype = wctype (s);
		free (s);
		if (n->chars[n->nitems].wctype == 0)
		  return REG_ECTYPE; /* Invalid character class */

		ptr = p + 1;
		n->nitems++;
		goto next;
	      }
	    case '=':
	    case '.':
	      /* Equivalence classes and collating elements are not supported */
	      return REG_ENOSYS;
	    }
	  ptr--;
	}

      /* Add the character normally; change the maximum range of the previous
	 entry if a range operator was seen, otherwise add a new entry */
      if (linking)
	{
	  n->chars[n->nitems - 1].max = *ptr;
	  if (n->chars[n->nitems - 1].min > n->chars[n->nitems - 1].max)
	    return REG_ERANGE;
	  linking = 0;
	}
      else
	{
	  n->chars[n->nitems].wctype = 0;
	  n->chars[n->nitems].min = *ptr;
	  n->chars[n->nitems].max = *ptr;
	  n->nitems++;
	}

    next:
      ptr++;
    }
  if (*ptr == '\0')
    return REG_EBRACK;

  /* If - was the last character in the bracket expression, add it to the
     list of characters accepted */
  if (linking)
    {
      if (n->nitems == alloc)
	{
	  chars = realloc (n->chars, sizeof (struct __re_chars) * ++n->nitems);
	  RE_MEMCHECK (chars);
	  n->chars = chars;
	}
      n->chars[n->nitems - 1].min = '-';
      n->chars[n->nitems - 1].max = '-';
    }

  n->min_rep = 1;
  n->max_rep = 1;
  n->can_rep = 1;
  *str = ptr;
  return 0;
}

static int
__re_compile_fixed (struct __re_regex *re, __re_node_type type)
{
  struct __re_node *n = &re->nodes[re->count - 1];
  int rep = type == __RE_ANY;
  RE_RETCHECK (__re_new_node (re));
  n->type = type;
  n->min_rep = 1;
  n->max_rep = 1;
  n->can_rep = rep;
  return 0;
}

static int
__re_compile_literal (struct __re_regex *re, char c)
{
  struct __re_node *n;
  RE_RETCHECK (__re_new_node (re));
  n = &re->nodes[re->count - 1];
  n->type = __RE_CHARS;
  n->nitems = 1;
  n->exclude = 0;
  n->chars = malloc (sizeof (struct __re_chars));
  RE_MEMCHECK (n->chars);
  n->chars[0].min = c;
  n->chars[0].max = c;
  n->min_rep = 1;
  n->max_rep = 1;
  n->can_rep = 1;
  return 0;
}

static int
__re_compile_expr (struct __re_regex *__restrict re,
		   const char *__restrict pattern)
{
  const char *ptr = pattern;
  int escaped = 0;
  while (*ptr != '\0')
    {
      switch (*ptr)
	{
	case '\\':
	  if (escaped)
	    RE_RETCHECK (__re_compile_literal (re, *ptr));
	  else
	    {
	      escaped = 1;
	      ptr++;
	      continue;
	    }
	  break;
	case '[':
	  RE_RETCHECK (__re_compile_bracket (re, &ptr));
	  break;
	case '.':
	  RE_RETCHECK (__re_compile_fixed (re, __RE_ANY));
	  break;
	case '^':
	  RE_RETCHECK (ptr == pattern ? __re_compile_fixed (re, __RE_START) :
		       __re_compile_literal (re, *ptr));
	  break;
	case '$':
	  RE_RETCHECK (ptr[1] == '\0' ? __re_compile_fixed (re, __RE_END) :
		       __re_compile_literal (re, *ptr));
	  break;
	case '*':
	  if (ptr == pattern || (*pattern == '^' && ptr != pattern + 1))
	    RE_RETCHECK (__re_compile_literal (re, *ptr));
	  else
	    {
	      struct __re_node *n = &re->nodes[re->count - 1];
	      if (!n->can_rep)
		return REG_BADRPT;
	      n->max_rep = (size_t) -1;
	      n->can_rep = 0;
	    }
	  break;
	default:
	  RE_RETCHECK (__re_compile_literal (re, *ptr));
	}
      ptr++;
      escaped = 0;
    }
  if (escaped)
    return REG_EESCAPE;
  return 0;
}

int
regcomp (regex_t *__restrict preg, const char *__restrict pattern, int cflags)
{
  struct __re_regex *re = &preg->__re_regex;
  int ret;
  memset (preg, 0, sizeof (regex_t));
  ret = __re_compile_expr (re, pattern);
  if (ret != 0)
    {
      __re_free_regex (re);
      return ret;
    }
  preg->__re_cflags = cflags;
  return 0;
}

int
regexec (const regex_t *__restrict preg, const char *__restrict str,
	 size_t nmatch, regmatch_t pmatch[__restrict], int eflags)
{
  return REG_NOMATCH;
}

size_t
regerror (int errcode, const regex_t *__restrict preg, char *__restrict errbuf,
	  size_t errbuf_size)
{
  return 0;
}

void
regfree (regex_t *preg)
{
  __re_free_regex (&preg->__re_regex);
}
