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
#include <string.h>

static inline Elf32_Sym *
rtld_get_symbol (struct rtld_info *dlinfo, Elf32_Word index)
{
  return (Elf32_Sym *) ((uintptr_t) dlinfo->symtab.table +
			index * dlinfo->symtab.entsize);
}

unsigned long
rtld_symbol_hash (const char *name)
{
  const unsigned char *str = (const unsigned char *) name;
  unsigned long h = 0;
  unsigned long g;
  while (*str != '\0')
    {
      h = (h << 4) + *str++;
      g = h & 0xf0000000;
      if (g != 0)
	h ^= g >> 24;
      h &= ~g;
    }
  return h;
}

void *
rtld_lookup_symbol (const char *name, struct rtld_info *dlinfo, int local)
{
  Elf32_Word nbucket = dlinfo->hash[0];
  Elf32_Word nchain = dlinfo->hash[1];
  Elf32_Word *bucket = &dlinfo->hash[2];
  Elf32_Word *chain = &bucket[nbucket];
  unsigned long hash = rtld_symbol_hash (name);
  Elf32_Word y = bucket[hash % nbucket];
  size_t i;

  while (y != STN_UNDEF)
    {
      Elf32_Sym *symbol = rtld_get_symbol (dlinfo, y);
      if (strcmp (dlinfo->strtab.table + symbol->st_name, name) == 0)
	{
	  if (symbol->st_shndx == STN_UNDEF
	      || (!local && ELF32_ST_BIND (symbol->st_info) == STB_LOCAL))
	    goto depsearch;
	  return dlinfo->offset + symbol->st_value;
	}
      y = chain[y % nchain];
    }

 depsearch:
  /* Not found in shared object hash table, try searching for the symbol in
     its dependencies */
  for (i = 0; i < dlinfo->deps.count; i++)
    {
      struct rtld_info *info = &rtld_shlibs[dlinfo->deps.deps[i]];
      void *addr = rtld_lookup_symbol (name, info, 0);
      if (addr != NULL)
        return addr;
    }
  return NULL; /* Could not find symbol */
}

void
rtld_perform_rel (Elf32_Rel *entry, struct rtld_info *dlinfo,
		  Elf32_Sword addend)
{
  Elf32_Sym *symbol = NULL;
  void *symaddr;

  /* Lookup the symbol if there is one specified */
  if (ELF32_R_SYM (entry->r_info) != STN_UNDEF)
    {
      const char *name;
      symbol = rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
      name = dlinfo->strtab.table + symbol->st_name;
      symaddr = rtld_lookup_symbol (name, dlinfo, 1);
      if (unlikely (symaddr == NULL))
	{
	  if (ELF32_ST_BIND (symbol->st_info) == STB_WEAK)
	    return; /* Weak symbols are optionally defined */
	  fprintf (stderr, "ld.so: %s: couldn't resolve undefined symbol: %s\n",
		   dlinfo->name, name);
	  abort ();
	}
    }

  switch (ELF32_R_TYPE (entry->r_info))
    {
#define REL_OFFSET ((Elf32_Addr *) (dlinfo->offset + entry->r_offset))
    case R_386_COPY:
    case R_386_GLOB_DAT:
    case R_386_JMP_SLOT:
      addend = 0;
    case R_386_32:
      *REL_OFFSET = (uintptr_t) symaddr + addend;
      break;
    case R_386_PC32:
      *REL_OFFSET = (uintptr_t) symaddr - (uintptr_t) dlinfo->offset -
	entry->r_offset - 4 + addend;
      break;
    case R_386_RELATIVE:
      *REL_OFFSET += (uintptr_t) dlinfo->offset + addend;
      break;
    case R_386_GOTOFF:
      *REL_OFFSET = (uintptr_t) symaddr - (uintptr_t) dlinfo->pltgot + addend;
      break;
    case R_386_GOTPC:
      *REL_OFFSET = (uintptr_t) dlinfo->pltgot - entry->r_offset - 4 + addend;
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
