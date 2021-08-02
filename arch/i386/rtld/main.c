/* main.c -- This file is part of OS/0 libc.
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

#include <sys/stat.h>
#include <branch.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <rtld.h>
#include <setup.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

static void
rtld_exec_init_funcs (void)
{
  while (1)
    {
      void (*func) (void) = rtld_queue_poll (&rtld_init_func);
      if (func == NULL)
	break;
      func ();
    }
}

static void
rtld_exec_fini_funcs (void)
{
  while (1)
    {
      void (*func) (void) = rtld_queue_poll (&rtld_fini_func);
      if (func == NULL)
	break;
      func ();
    }
}

void *
rtld_main (void *base, Elf32_Dyn *dynamic, char **env)
{
  int mode;

  /* Initialize libc */
  environ = env;
  __libc_setup_stdstr ();

  /* Initialize executable object info */
  rtld_shlibs->name = "(executable)";
  rtld_shlibs->fd = -1;
  rtld_shlibs->loadbase = base;
  rtld_shlibs->offset = NULL;
  rtld_shlibs->dynamic = dynamic;

  /* Load, relocate, and remap segments */
  mode = getenv ("LD_BIND_NOW") == NULL ? RTLD_LAZY : RTLD_NOW;
  rtld_load_dynamic (0, 0, mode);
  rtld_relocate (0, mode);
  rtld_remap_memory ();

  rtld_exec_init_funcs ();
  return rtld_exec_fini_funcs;
}
