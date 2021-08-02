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
#include <dlfcn.h>
#include <rtld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rtld_global *rtld_globals;
size_t rtld_global_count;

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
rtld_lookup_symbol (const char *name, int obj, int local)
{
  struct rtld_info *dlinfo = &rtld_shlibs[obj];
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
      int lib = dlinfo->deps.deps[i];
      void *addr = rtld_lookup_symbol (name, lib, 0);
      if (addr != NULL)
        return addr;
    }
  return NULL; /* Could not find symbol */
}

void
rtld_perform_rel (Elf32_Rel *entry, int obj, Elf32_Sword addend, int mode)
{
#define REL_OFFSET ((Elf32_Addr *) (dlinfo->offset + entry->r_offset))
  struct rtld_info *dlinfo = &rtld_shlibs[obj];
  Elf32_Sym *symbol = NULL;
  const char *name = NULL;
  void *symaddr;
  size_t i;

  if ((mode & RTLD_LAZY) && ELF32_R_TYPE (entry->r_info) == R_386_JMP_SLOT)
    {
      *REL_OFFSET += (uintptr_t) dlinfo->offset;
      return;
    }

  /* Lookup the symbol if there is one specified */
  if (ELF32_R_SYM (entry->r_info) != STN_UNDEF)
    {
      symbol = rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
      name = dlinfo->strtab.table + symbol->st_name;
      symaddr = rtld_lookup_symbol (name, obj, 1);
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
    case R_386_32:
      *REL_OFFSET = (uintptr_t) symaddr + addend;
      break;
    case R_386_PC32:
      *REL_OFFSET = (uintptr_t) symaddr - (uintptr_t) dlinfo->offset -
	entry->r_offset - 4 + addend;
      break;
    case R_386_GLOB_DAT:
      for (i = 0; i < rtld_global_count; i++)
	{
	  if (strcmp (rtld_globals[i].name, name) == 0)
	    {
	      rtld_globals[i].start_addr = symaddr;
	      *REL_OFFSET = (uintptr_t) rtld_globals[i].final_addr;
	      break;
	    }
	}
      if (i == rtld_global_count)
	{
	  /* The symbol is not needed by the executable, link directly to
	     address in shared object */
	  *REL_OFFSET = (uintptr_t) symaddr;
	}
      break;
    case R_386_JMP_SLOT:
      *REL_OFFSET = (uintptr_t) symaddr;
      break;
    case R_386_COPY:
      for (i = 0; i < rtld_global_count; i++)
	{
	  if (strcmp (rtld_globals[i].name, name) == 0)
	    {
	      memcpy (rtld_globals[i].final_addr, rtld_globals[i].start_addr,
		      rtld_globals[i].size);
	      break;
	    }
	}
      break;
    case R_386_RELATIVE:
      *REL_OFFSET += (uintptr_t) dlinfo->offset + addend;
      break;
#undef REL_OFFSET
    }
}

void
rtld_relocate (int obj, int mode)
{
  struct rtld_info *dlinfo = &rtld_shlibs[obj];
  size_t size;
  if (dlinfo->rel.table != NULL)
    {
      for (size = 0; size < dlinfo->rel.size; size += dlinfo->rel.entsize)
	{
	  Elf32_Rel *entry =
	    (Elf32_Rel *) ((uintptr_t) dlinfo->rel.table + size);
	  rtld_perform_rel (entry, obj, 0, mode);
	}
    }
  if (dlinfo->rela.table != NULL)
    {
      for (size = 0; size < dlinfo->rela.size; size += dlinfo->rela.entsize)
	{
	  Elf32_Rela *entry =
	    (Elf32_Rela *) ((uintptr_t) dlinfo->rela.table + size);
	  rtld_perform_rel ((Elf32_Rel *) entry, obj, entry->r_addend, mode);
	}
    }

  if (dlinfo->pltrel.type == DT_REL)
    {
      for (size = 0; size < dlinfo->pltrel.size / sizeof (Elf32_Rel); size++)
	{
	  Elf32_Rel *entry = (Elf32_Rel *) dlinfo->pltrel.table + size;
	  rtld_perform_rel (entry, obj, 0, mode);
	}
    }
  else
    {
      for (size = 0; size < dlinfo->pltrel.size / sizeof (Elf32_Rela); size++)
	{
	  Elf32_Rela *entry = (Elf32_Rela *) dlinfo->pltrel.table + size;
	  rtld_perform_rel ((Elf32_Rel *) entry, obj, entry->r_addend, mode);
	}
    }
}

void *
rtld_lazy_get_got_offset (unsigned long symbol, int obj)
{
  return rtld_shlibs[obj].pltgot + 3 + (symbol >> 3);
}

const char *
rtld_lazy_get_symbol_name (void *got_addr, int obj)
{
  struct rtld_info *dlinfo = &rtld_shlibs[obj];
  int rela = dlinfo->pltrel.type == DT_RELA;
  size_t first = 0;
  size_t last = dlinfo->pltrel.size /
    (rela ? sizeof (Elf32_Rela) : sizeof (Elf32_Rel)) - 1;
  while (first <= last)
    {
      size_t mid = (first + last) / 2;
      Elf32_Rel *entry;
      void *addr;
      if (rela)
	entry = (Elf32_Rel *) ((Elf32_Rela *) dlinfo->pltrel.table + mid);
      else
	entry = (Elf32_Rel *) dlinfo->pltrel.table + mid;
      addr = dlinfo->offset + entry->r_offset;
      if (addr == got_addr)
	{
	  Elf32_Sym *symbol =
	    rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
	  return dlinfo->strtab.table + symbol->st_name;
	}
      else if (addr > got_addr)
	last = mid - 1;
      else
	first = mid + 1;
    }
  return NULL;
}

void
rtld_lazy_bind_fail (const char *name, int obj)
{
  fprintf (stderr, "ld.so: %s: failed to bind symbol: %s\n",
	   rtld_shlibs[obj].name, name);
  abort ();
}
