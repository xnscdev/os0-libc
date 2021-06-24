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

DynamicLinkInfo rtld_shlibs[MAX_SHLIBS];
PriorityQueueNode *rtld_init_func;
PriorityQueueNode *rtld_fini_func;

void
rtld_load_dynamic (DynamicLinkInfo *dlinfo, unsigned long priority)
{
  Elf32_Dyn *entry;
  for (entry = dlinfo->dl_dynamic; entry->d_tag != DT_NULL; entry++)
    {
      switch (entry->d_tag)
	{
	case DT_PLTRELSZ:
	  dlinfo->dl_pltrel.pt_size = entry->d_un.d_val;
	  break;
	case DT_PLTGOT:
	  dlinfo->dl_pltgot = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_HASH:
	  dlinfo->dl_hash = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_STRTAB:
	  dlinfo->dl_strtab.st_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_SYMTAB:
	  dlinfo->dl_symtab.sym_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_RELA:
	  dlinfo->dl_rela.ra_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_RELASZ:
	  dlinfo->dl_rela.ra_size = entry->d_un.d_val;
	  break;
	case DT_RELAENT:
	  dlinfo->dl_rela.ra_entsize = entry->d_un.d_val;
	  break;
	case DT_STRSZ:
	  dlinfo->dl_strtab.st_len = entry->d_un.d_val;
	  break;
	case DT_SYMENT:
	  dlinfo->dl_symtab.sym_entsize = entry->d_un.d_val;
	  break;
	case DT_INIT:
	  rtld_queue_add (&rtld_init_func,
			    dlinfo->dl_loadbase + entry->d_un.d_ptr, ~priority);
	  break;
	case DT_FINI:
	  rtld_queue_add (&rtld_fini_func,
			    dlinfo->dl_loadbase + entry->d_un.d_ptr, priority);
	  break;
	case DT_REL:
	  dlinfo->dl_rel.r_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_RELSZ:
	  dlinfo->dl_rel.r_size = entry->d_un.d_val;
	  break;
	case DT_RELENT:
	  dlinfo->dl_rel.r_entsize = entry->d_un.d_val;
	  break;
	case DT_PLTREL:
	  switch (entry->d_un.d_val)
	    {
	    case DT_REL:
	    case DT_RELA:
	      dlinfo->dl_pltrel.pt_type = entry->d_un.d_val;
	      break;
	    default:
	      fprintf (stderr, "ld.so: bad value %lu for PLT relocation type\n",
		       entry->d_un.d_val);
	      abort ();
	    }
	  break;
	case DT_JMPREL:
	  dlinfo->dl_pltrel.pt_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	}
    }

  if (dlinfo->dl_strtab.st_table == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing dynamic string table\n",
	       dlinfo->dl_name);
      abort ();
    }
  else if (dlinfo->dl_symtab.sym_table == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing dynamic symbol table\n",
	       dlinfo->dl_name);
      abort ();
    }
  else if (dlinfo->dl_hash == NULL)
    {
      fprintf (stderr, "ld.so: %s: missing symbol hash table\n",
	       dlinfo->dl_name);
      abort ();
    }

  /* Load shared libraries */
  for (entry = dlinfo->dl_dynamic; entry->d_tag != DT_NULL; entry++)
    {
      if (entry->d_tag == DT_NEEDED)
	{
	  const char *name = dlinfo->dl_strtab.st_table + entry->d_un.d_val;
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
  for (i = 0; i < MAX_SHLIBS && rtld_shlibs[i].dl_name != NULL; i++)
    {
      if (strcmp (name, rtld_shlibs[i].dl_name) == 0)
	return; /* Already loaded */
    }
  if (i == MAX_SHLIBS)
    {
      fprintf (stderr, "ld.so: %s: max shared object limit reached\n", name);
      abort ();
    }

  rtld_shlibs[i].dl_name = name;
  rtld_load_dynamic (&rtld_shlibs[i], priority);
}
