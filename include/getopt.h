/* getopt.h -- This file is part of OS/0 libc.
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

#ifndef _GETOPT_H
#define _GETOPT_H

#include <sys/cdefs.h>

#define no_argument       0
#define required_argument 1
#define optional_argument 2

struct option
{
  const char *name;
  int has_arg;
  int *flag;
  int val;
};

__BEGIN_DECLS

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

int getopt (int argc, char *const *argv, const char *shortopts);
int getopt_long (int argc, char *const *argv, const char *shortopts,
		 const struct option *longopts, int *index);
int getopt_long_only (int argc, char *const *argv, const char *shortopts,
		      const struct option *longopts, int *index);

__END_DECLS

#endif
