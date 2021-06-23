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

#include <rtld.h>

int
__rtld_load_dynamic (DynamicLinkInfo *dlinfo)
{
  Elf32_Dyn *entry;
  for (entry = dlinfo->dl_dynamic; entry->d_tag != DT_NULL; entry++)
    {
      switch (entry->d_tag)
	{
	case DT_NEEDED:
	  return -1; /* TODO Implement shared library lookup */
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
	  dlinfo->dl_init = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	case DT_FINI:
	  dlinfo->dl_fini = dlinfo->dl_loadbase + entry->d_un.d_ptr;
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
	      return -1;
	    }
	  break;
	case DT_JMPREL:
	  dlinfo->dl_pltrel.pt_table = dlinfo->dl_loadbase + entry->d_un.d_ptr;
	  break;
	}
    }
  return 0;
}
