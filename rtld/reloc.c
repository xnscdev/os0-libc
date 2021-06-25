/* reloc.c -- This file is part of OS/0 libc.
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

static inline Elf32_Sym *
rtld_get_symbol (struct rtld_info *dlinfo, Elf32_Word index)
{
  return (Elf32_Sym *) ((uintptr_t) dlinfo->symtab.table +
			index * dlinfo->symtab.entsize);
}

static void
rtld_perform_rel (Elf32_Rel *entry, struct rtld_info *dlinfo,
		  Elf32_Sword addend)
{
  Elf32_Sym *symbol = rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
  switch (ELF32_R_TYPE (entry->r_info))
    {
#define REL_OFFSET ((Elf32_Addr *) (dlinfo->offset + entry->r_offset))
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
      addend = 0;
    case R_386_32:
      *REL_OFFSET = (uintptr_t) dlinfo->offset + symbol->st_value + addend;
      break;
    case R_386_PC32:
      *REL_OFFSET = symbol->st_value - entry->r_offset - 4 + addend;
      break;
    case R_386_RELATIVE:
      *REL_OFFSET += (uintptr_t) dlinfo->offset + addend;
      break;
    case R_386_GOTOFF:
      *REL_OFFSET = symbol->st_value - (uintptr_t) dlinfo->pltgot + addend;
      break;
    case R_386_GOTPC:
      *REL_OFFSET =
	(uintptr_t) dlinfo->pltgot - entry->r_offset - 4 + addend;
      break;
#undef REL_OFFSET
    }
}

void
rtld_relocate (struct rtld_info *dlinfo)
{
  size_t size;
  if (dlinfo->rel.table != NULL)
    {
      for (size = 0; size < dlinfo->rel.size; size += dlinfo->rel.entsize)
	{
	  Elf32_Rel *entry =
	    (Elf32_Rel *) ((uintptr_t) dlinfo->rel.table + size);
	  rtld_perform_rel (entry, dlinfo, 0);
	}
    }
  if (dlinfo->rela.table != NULL)
    {
      for (size = 0; size < dlinfo->rela.size; size += dlinfo->rela.entsize)
	{
	  Elf32_Rela *entry =
	    (Elf32_Rela *) ((uintptr_t) dlinfo->rela.table + size);
	  rtld_perform_rel ((Elf32_Rel *) entry, dlinfo, entry->r_addend);
	}
    }

  if (dlinfo->pltrel.type == DT_REL)
    {
      for (size = 0; size < dlinfo->pltrel.size / sizeof (Elf32_Rel); size++)
	{
	  Elf32_Rel *entry = (Elf32_Rel *) dlinfo->pltrel.table + size;
	  rtld_perform_rel (entry, dlinfo, 0);
	}
    }
  else
    {
      for (size = 0; size < dlinfo->pltrel.size / sizeof (Elf32_Rela); size++)
	{
	  Elf32_Rela *entry = (Elf32_Rela *) dlinfo->pltrel.table + size;
	  rtld_perform_rel ((Elf32_Rel *) entry, dlinfo, entry->r_addend);
	}
    }
}
