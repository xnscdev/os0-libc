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

#include <rtld.h>

void __libc_init (void);

extern char **environ;

void
rtld_main (void *base, Elf32_Dyn *dynamic, char **env)
{
  environ = env;
  __libc_init ();
  rtld_shlibs->name = "";
  rtld_shlibs->loadbase = base;
  rtld_shlibs->offset = NULL;
  rtld_shlibs->dynamic = dynamic;
  rtld_load_dynamic (rtld_shlibs, 0);
  rtld_relocate (rtld_shlibs);
  rtld_remap_memory ();
}
