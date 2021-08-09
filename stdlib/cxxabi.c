/* cxxabi.c -- This file is part of OS/0 libc.
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

#include <stddef.h>

/* Definitions for C++ ABI functions */

#define CXA_ATEXIT_MAX 128
#define CXA_CURR_FUNC  (__cxa_atexit_funcs[__cxa_atexit_nfuncs])

struct cxa_dtor
{
  void (*func) (void *);
  void *obj;
  void *dso;
};

static struct cxa_dtor __cxa_atexit_funcs[CXA_ATEXIT_MAX];
static size_t __cxa_atexit_nfuncs;

void
__cxa_pure_virtual (void)
{
}

int
__cxa_atexit (void (*func) (void *), void *obj, void *dso)
{
  if (__cxa_atexit_nfuncs >= CXA_ATEXIT_MAX)
    return -1;
  CXA_CURR_FUNC.func = func;
  CXA_CURR_FUNC.obj = obj;
  CXA_CURR_FUNC.dso = dso;
  __cxa_atexit_nfuncs++;
  return 0;
}

void
__cxa_finalize (void *func)
{
  if (func == NULL)
    {
      while (__cxa_atexit_nfuncs-- >= 0)
	{
	  if (CXA_CURR_FUNC.func != NULL)
	    CXA_CURR_FUNC.func (CXA_CURR_FUNC.obj);
	}
    }
  else
    {
      size_t num = __cxa_atexit_nfuncs;
      while (num-- >= 0)
	{
	  if (__cxa_atexit_funcs[num].func == func)
	    {
	      __cxa_atexit_funcs[num].func (__cxa_atexit_funcs[num].obj);
	      __cxa_atexit_funcs[num].func = NULL;
	    }
	}
    }
}
