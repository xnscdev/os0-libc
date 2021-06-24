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

DynamicLinkInfo __rtld_shlibs[MAX_SHLIBS];
PriorityQueueNode *__rtld_init_func;
PriorityQueueNode *__rtld_fini_func;

void
__rtld_load_dynamic (DynamicLinkInfo *dlinfo, unsigned long priority)
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
	  __rtld_queue_add (&__rtld_init_func,
			    dlinfo->dl_loadbase + entry->d_un.d_ptr, ~priority);
	  break;
	case DT_FINI:
	  __rtld_queue_add (&__rtld_fini_func,
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
	      __rtld_print ("Bad value for PLT relocation type\n");
	      __rtld_fail ();
	    }
	  break;
	case DT_JMPREL:
	  dlinfo->dl_pltrel.pt_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	}
    }

  if (dlinfo->dl_strtab.st_table == NULL)
    {
      __rtld_print ("Object missing string table");
      __rtld_fail ();
    }
  else if (dlinfo->dl_symtab.sym_table == NULL)
    {
      __rtld_print ("Object missing dynamic symbol table");
      __rtld_fail ();
    }
  else if (dlinfo->dl_hash == NULL)
    {
      __rtld_print ("Object missing symbol hash table");
      __rtld_fail ();
    }

  /* Load shared libraries */
  for (entry = dlinfo->dl_dynamic; entry->d_tag != DT_NULL; entry++)
    {
      if (entry->d_tag == DT_NEEDED)
	{
	  const char *name = dlinfo->dl_strtab.st_table + entry->d_un.d_val;
	  if (unlikely (*name == '\0'))
	    continue;
	  __rtld_load_shlib (name, priority + 1);
	}
    }
}

void
__rtld_load_shlib (const char *name, unsigned long priority)
{
  int i;
  /* Check if a library with the same name has already been loaded */
  for (i = 0; i < MAX_SHLIBS && __rtld_shlibs[i].dl_name != NULL; i++)
    {
      if (__rtld_strcmp (name, __rtld_shlibs[i].dl_name) == 0)
	return; /* Already loaded */
    }
  if (i == MAX_SHLIBS)
    {
      __rtld_print ("Failed to load ");
      __rtld_print (name);
      __rtld_print (": max shared object limit reached\n");
      __rtld_fail ();
    }

  __rtld_shlibs[i].dl_name = name;
  __rtld_load_dynamic (&__rtld_shlibs[i], priority);
}
