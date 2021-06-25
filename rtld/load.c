/* load.c -- This file is part of OS/0 libc.
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

#include <branch.h>
#include <rtld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rtld_info rtld_shlibs[MAX_SHLIBS];
struct queue_node *rtld_init_func;
struct queue_node *rtld_fini_func;

void
rtld_load_dynamic (struct rtld_info *dlinfo, unsigned long priority)
{
  Elf32_Dyn *entry;
  for (entry = dlinfo->dynamic; entry->d_tag != DT_NULL; entry++)
    {
      switch (entry->d_tag)
	{
	case DT_PLTRELSZ:
	  dlinfo->pltrel.size = entry->d_un.d_val;
	  break;
	case DT_PLTGOT:
	  dlinfo->pltgot = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_HASH:
	  dlinfo->hash = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_STRTAB:
	  dlinfo->strtab.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_SYMTAB:
	  dlinfo->symtab.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_RELA:
	  dlinfo->rela.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_RELASZ:
	  dlinfo->rela.size = entry->d_un.d_val;
	  break;
	case DT_RELAENT:
	  dlinfo->rela.entsize = entry->d_un.d_val;
	  break;
	case DT_STRSZ:
	  dlinfo->strtab.len = entry->d_un.d_val;
	  break;
	case DT_SYMENT:
	  dlinfo->symtab.entsize = entry->d_un.d_val;
	  break;
	case DT_INIT:
	  rtld_queue_add (&rtld_init_func,
			  dlinfo->offset + entry->d_un.d_ptr, priority);
	  break;
	case DT_FINI:
	  rtld_queue_add (&rtld_fini_func,
			  dlinfo->offset + entry->d_un.d_ptr, priority);
	  break;
	case DT_REL:
	  dlinfo->rel.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_RELSZ:
	  dlinfo->rel.size = entry->d_un.d_val;
	  break;
	case DT_RELENT:
	  dlinfo->rel.entsize = entry->d_un.d_val;
	  break;
	case DT_PLTREL:
	  switch (entry->d_un.d_val)
	    {
	    case DT_REL:
	    case DT_RELA:
	      dlinfo->pltrel.type = entry->d_un.d_val;
	      break;
	    default:
	      fprintf (stderr, "ld.so: bad value %lu for PLT relocation type\n",
		       entry->d_un.d_val);
	      abort ();
	    }
	  break;
	case DT_JMPREL:
	  dlinfo->pltrel.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	}
    }

  if (dlinfo->strtab.table == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing dynamic string table\n",
	       dlinfo->name);
      abort ();
    }
  else if (dlinfo->symtab.table == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing dynamic symbol table\n",
	       dlinfo->name);
      abort ();
    }
  else if (dlinfo->hash == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing symbol hash table\n", dlinfo->name);
      abort ();
    }

  /* Load shared libraries */
  for (entry = dlinfo->dynamic; entry->d_tag != DT_NULL; entry++)
    {
      if (entry->d_tag == DT_NEEDED)
	{
	  const char *name = dlinfo->strtab.table + entry->d_un.d_val;
	  if (unlikely (*name == '\0'))
	    continue;
	  rtld_load_shlib (name, priority + 1);
	}
    }
}

void
rtld_load_shlib (const char *name, unsigned long priority)
{
  int i;
  /* Check if a library with the same name has already been loaded */
  for (i = 0; i < MAX_SHLIBS && rtld_shlibs[i].name != NULL; i++)
    {
      if (strcmp (name, rtld_shlibs[i].name) == 0)
	return; /* Already loaded */
    }
  if (i == MAX_SHLIBS)
    {
      fprintf (stderr, "ld.so: %s: max shared object limit reached\n", name);
      abort ();
    }

  rtld_shlibs[i].name = name;
  rtld_load_dynamic (&rtld_shlibs[i], priority);
}
