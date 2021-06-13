/* getopt.c -- This file is part of OS/0 libc.
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

/* Getopt for GNU.
   Copyright (C) 1987-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library and is also part of gnulib.
   Patches to this file should be submitted to both projects.
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum __libc_order
{
  REQUIRE_ORDER,
  PERMUTE,
  RETURN_IN_ORDER
};

static enum __libc_order order;
static int first_nonopt;
static int last_nonopt;
static char *next_char;

char *optarg;
int optind;
int opterr = 1;
int optopt = '?';

static void
exchange (char **argv)
{
  int bottom = first_nonopt;
  int mid = last_nonopt;
  int top = optind;
  char *temp;

  while (top > mid && mid > bottom)
    {
      if (top - mid > mid - bottom)
	{
	  int len = mid - bottom;
	  int i;
	  for (i = 0; i < len; i++)
	    {
	      temp = argv[bottom + i];
	      argv[bottom + i] = argv[top - mid + bottom + i];
	      argv[top - mid + bottom + i] = temp;
	    }
	  top -= len;
	}
      else
	{
	  int len = top - mid;
	  int i;
	  for (i = 0; i < len; i++)
	    {
	      temp = argv[bottom + i];
	      argv[bottom + i] = argv[mid + i];
	      argv[mid + i] = temp;
	    }
	  bottom += len;
	}
    }

  first_nonopt += optind - last_nonopt;
  last_nonopt = optind;
}

static int
process_long_option (int argc, char **argv, const char *shortopts,
		     const struct option *longopts, int *index, int long_only,
		     int print_err, const char *prefix)
{
  char *end;
  size_t namelen;
  const struct option *ptr;
  const struct option *found = NULL;
  int nopts;
  int opt_index;

  for (end = next_char; *end != '\0' && *end != '='; end++)
    ;
  namelen = end - next_char;

  for (ptr = longopts, nopts = 0; ptr->name != NULL; ptr++, nopts++)
    {
      if (strncmp (ptr->name, next_char, namelen) == 0
	  && namelen == strlen (ptr->name))
	{
	  found = ptr;
	  opt_index = nopts;
	  break;
	}
    }

  if (found == NULL)
    {
      unsigned char *ambig_set = NULL;
      int ambig_alloc = 0;
      int ambig_fallback = 0;
      int index_found = -1;

      for (ptr = longopts, opt_index = 0; ptr->name != NULL; ptr++, opt_index++)
	{
	  if (strncmp (ptr->name, next_char, namelen) == 0)
	    {
	      if (found == NULL)
		{
		  found = ptr;
		  index_found = opt_index;
		}
	      else if (long_only || found->has_arg != ptr->has_arg
		       || found->flag != ptr->flag || found->val != ptr->val)
		{
		  if (!ambig_fallback)
		    {
		      if (!print_err)
			ambig_fallback = 1;
		      else if (ambig_set == NULL)
			{
			  ambig_set = malloc (nopts);
			  if (ambig_set == NULL)
			    ambig_fallback = 1;
			  else
			    ambig_alloc = 1;
			  if (ambig_set != NULL)
			    {
			      memset (ambig_set, 0, nopts);
			      ambig_set[index_found] = 1;
			    }
			}
		      if (ambig_set != NULL)
			ambig_set[opt_index] = 1;
		    }
		}
	    }
	}

      if (ambig_set != NULL || ambig_fallback)
	{
	  if (print_err)
	    {
	      if (ambig_fallback)
		fprintf (stderr, "%s: option '%s%s' is ambiguous\n", argv[0],
			 prefix, next_char);
	      else
		{
		  fprintf (stderr, "%s: option '%s%s' is ambiguous; "
			   "possibilities:", argv[0], prefix, next_char);
		  for (opt_index = 0; opt_index < nopts; opt_index++)
		    {
		      if (ambig_set[opt_index])
			fprintf (stderr, " '%s%s'", prefix,
				 longopts[opt_index].name);
		    }
		  fputc ('\n', stderr);
		}
	    }
	  if (ambig_alloc)
	    free (ambig_set);
	  next_char += strlen (next_char);
	  optind++;
	  optopt = 0;
	  return '?';
	}
      opt_index = index_found;
    }

  if (found == NULL)
    {
      if (!long_only || argv[optind][1] == '-'
	  || strchr (shortopts, *next_char) == NULL)
	{
	  if (print_err)
	    fprintf (stderr, "%s: unrecognized option '%s%s'\n", argv[0],
		     prefix, next_char);
	  next_char = NULL;
	  optind++;
	  optopt = 0;
	  return '?';
	}
      return -1;
    }

  optind++;
  next_char = NULL;
  if (*end != '\0')
    {
      if (found->has_arg)
	optarg = end + 1;
      else
	{
	  if (print_err)
	    fprintf (stderr, "%s: option '%s%s' doesn't allow an argument\n",
		     argv[0], prefix, found->name);
	  optopt = found->val;
	  return '?';
	}
    }
  else if (found->has_arg == required_argument)
    {
      if (optind < argc)
	optarg = argv[optind++];
      else
	{
	  if (print_err)
	    fprintf (stderr, "%s: option '%s%s' requires an argument\n",
		     argv[0], prefix, found->name);
	  optopt = found->val;
	  return shortopts[0]  == ':' ? ':' : '?';
	}
    }

  if (index != NULL)
    *index = opt_index;
  if (found->flag != NULL)
    {
      *found->flag = found->val;
      return 0;
    }
  return found->val;
}

static const char *
__libc_getopt_init (int argc, char **argv, const char *shortopts)
{
  if (optind == 0)
    optind = 1;
  first_nonopt = optind;
  last_nonopt = optind;
  if (*shortopts == '-')
    {
      order = RETURN_IN_ORDER;
      shortopts++;
    }
  else if (*shortopts == '+')
    {
      order = REQUIRE_ORDER;
      shortopts++;
    }
  else if (getenv ("POSIXLY_CORRECT") != NULL)
    order = REQUIRE_ORDER;
  else
    order = PERMUTE;
  return shortopts;
}

static int
__libc_getopt (int argc, char **argv, const char *shortopts,
	       const struct option *longopts, int *index, int long_only)
{
  int print_err = opterr;
  char c;
  const char *temp;
  if (argc < 1)
    return -1;
  optarg = NULL;

  if (optind == 0)
    shortopts = __libc_getopt_init (argc, argv, shortopts);
  else if (*shortopts == '-' || *shortopts == '+')
    shortopts++;
  if (*shortopts == ':')
    print_err = 0;

  if (next_char == NULL || *next_char == '\0')
    {
      if (last_nonopt > optind)
	last_nonopt = optind;
      if (first_nonopt > optind)
	first_nonopt = optind;

      if (order == PERMUTE)
	{
	  if (first_nonopt != last_nonopt && last_nonopt != optind)
	    exchange (argv);
	  else if (last_nonopt != optind)
	    first_nonopt = optind;

	  while (optind < argc
		 && (argv[optind][0] != '-' || argv[optind][1] == '\0'))
	    optind++;
	  last_nonopt = optind;
	}

      if (optind != argc && strcmp (argv[optind], "--") == 0)
	{
	  optind++;
	  if (first_nonopt != last_nonopt && last_nonopt != optind)
	    exchange (argv);
	  else if (first_nonopt == last_nonopt)
	    first_nonopt = optind;
	  last_nonopt = argc;
	  optind = argc;
	}

      if (optind == argc)
	{
	  if (first_nonopt != last_nonopt)
	    optind = first_nonopt;
	  return -1;
	}

      if (argv[optind][0] != '-' || argv[optind][1] == '\0')
	{
	  if (order == REQUIRE_ORDER)
	    return -1;
	  optarg = argv[optind++];
	  return 1;
	}

      if (longopts != NULL)
	{
	  if (argv[optind][1] == '-')
	    {
	      next_char = argv[optind] + 2;
	      return process_long_option (argc, argv, shortopts, longopts,
					  index, long_only, print_err, "--");
	    }
	  if (long_only && (argv[optind][2] != '\0'
			    && strchr (shortopts, argv[optind][1]) == NULL))
	    {
	      int code;
	      next_char = argv[optind] + 1;
	      code = process_long_option (argc, argv, shortopts, longopts,
					  index, long_only, print_err, "-");
	      if (code != -1)
		return code;
	    }
	}
      next_char = argv[optind] + 1;
    }

  c = *next_char++;
  temp = strchr (shortopts, c);
  if (*next_char++ == '\0')
    optind++;

  if (temp == NULL || c == ':' || c == ';')
    {
      if (print_err)
	fprintf (stderr, "%s: invalid option -- '%c'\n", argv[0], c);
      optopt = c;
      return '?';
    }

  if (temp[0] == 'W' && temp[1] == ';' && longopts != NULL)
    {
      if (*next_char != '\0')
	optarg = next_char;
      else if (optind == argc)
	{
	  if (print_err)
	    fprintf (stderr, "%s: option requires an argument -- '%c'\n",
		     argv[0], c);
	  optopt = c;
	  return shortopts[0] == ':' ? ':' : '?';
	}
      else
	optarg = argv[optind];

      next_char = optarg;
      optarg = NULL;
      return process_long_option (argc, argv, shortopts, longopts, index, 0,
				  print_err, "-W ");
    }
  if (temp[1] == ':')
    {
      if (temp[2] == ':')
	{
	  if (*next_char != '\0')
	    {
	      optarg = next_char;
	      optind++;
	    }
	  else
	    optarg = NULL;
	  next_char = NULL;
	}
      else
	{
	  if (*next_char != '\0')
	    {
	      optarg = next_char;
	      optind++;
	    }
	  else if (optind == argc)
	    {
	      if (print_err)
		fprintf (stderr, "%s: option requires an argument -- '%c'\n",
			 argv[0], c);
	      optopt = c;
	      return shortopts[0] == ':' ? ':' : '?';
	    }
	  else
	    optarg = argv[optind++];
	  next_char = NULL;
	}
    }
  return c;
}

int
getopt (int argc, char *const *argv, const char *shortopts)
{
  return __libc_getopt (argc, (char **) argv, shortopts, NULL, 0, 0);
}

int
getopt_long (int argc, char *const *argv, const char *shortopts,
	     const struct option *longopts, int *index)
{
  return __libc_getopt (argc, (char **) argv, shortopts, longopts, index, 0);
}

int
getopt_long_only (int argc, char *const *argv, const char *shortopts,
		  const struct option *longopts, int *index)
{
  return __libc_getopt (argc, (char **) argv, shortopts, longopts, index, 1);
}
