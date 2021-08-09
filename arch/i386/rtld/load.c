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

#include <sys/mman.h>
#include <branch.h>
#include <errno.h>
#include <rtld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct rtld_info rtld_shlibs[MAX_SHLIBS];
struct queue_node *rtld_init_func;
struct queue_node *rtld_fini_func;

static inline void
rtld_check_offset (struct rtld_info *dlinfo)
{
  if (unlikely (dlinfo->offset == NULL))
    {
      fprintf (stderr, "ld.so: %s: load offset requested before object was "
	       "loaded\n", dlinfo->name);
      abort ();
    }
}

void
rtld_map_elf (int fd, struct rtld_info *dlinfo)
{
  Elf32_Ehdr *ehdr = malloc (sizeof (Elf32_Ehdr));
  int ret;
  if (unlikely (ehdr == NULL))
    RTLD_NO_MEMORY (dlinfo->name);
  ret = read (fd, ehdr, sizeof (Elf32_Ehdr));
  if (ret != sizeof (Elf32_Ehdr))
    {
      fprintf (stderr, "ld.so: %s: bad ELF header\n", dlinfo->name);
      abort ();
    }
  if (ehdr->e_ident[EI_MAG0] != ELFMAG0
      || ehdr->e_ident[EI_MAG1] != ELFMAG1
      || ehdr->e_ident[EI_MAG2] != ELFMAG2
      || ehdr->e_ident[EI_MAG3] != ELFMAG3)
    {
      fprintf (stderr, "ld.so: %s: bad ELF magic number\n", dlinfo->name);
      abort ();
    }
  if (ehdr->e_ident[EI_CLASS] != ELFCLASS32
      || ehdr->e_ident[EI_DATA] != ELFDATA2LSB
      || ehdr->e_type != ET_DYN
      || ehdr->e_machine != MACHTYPE)
    {
      fprintf (stderr, "ld.so: %s: bad ELF object/machine type\n",
	       dlinfo->name);
      abort ();
    }
  rtld_load_phdrs (fd, ehdr, dlinfo);
}

void
rtld_load_dynamic (int obj, unsigned long priority, int mode)
{
  Elf32_Dyn *entry;
  struct rtld_info *dlinfo = &rtld_shlibs[obj];
  uintptr_t *init_array = NULL;
  uintptr_t *fini_array = NULL;
  size_t init_arraysz = 0;
  size_t fini_arraysz = 0;
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
			  dlinfo->offset + entry->d_un.d_ptr, priority * 2);
	  break;
	case DT_INIT_ARRAY:
	  init_array = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_INIT_ARRAYSZ:
	  init_arraysz = entry->d_un.d_val / sizeof (uintptr_t);
	  break;
	case DT_FINI:
	  rtld_queue_add (&rtld_fini_func,
			  dlinfo->offset + entry->d_un.d_ptr, priority * 2);
	  break;
	case DT_FINI_ARRAY:
	  fini_array = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_FINI_ARRAYSZ:
	  fini_arraysz = entry->d_un.d_val / sizeof (uintptr_t);
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

  /* Add array of init/fini functions, if present */
  if (init_array != NULL)
    {
      size_t i;
      for (i = 0; i < init_arraysz; i++)
	{
	  if (init_array[i] != 0)
	    rtld_queue_add (&rtld_init_func, dlinfo->offset + init_array[i],
			    priority * 2 + 1);
	}
    }
  if (fini_array != NULL)
    {
      size_t i;
      for (i = 0; i < fini_arraysz; i++)
	{
	  if (fini_array[i] != 0)
	    rtld_queue_add (&rtld_fini_func, dlinfo->offset + fini_array[i],
			    priority * 2 + 1);
	}
    }

  /* Setup PLT/GOT for lazy dynamic linking */
  if (likely (dlinfo->pltgot != NULL))
    {
      dlinfo->pltgot[1] = obj;
      dlinfo->pltgot[2] = (uintptr_t) rtld_lazy_lookup_symbol;
    }

  /* If we are loading the main executable, we need to build a list of
     R_386_COPY relocations needed which will be used during relocation of
     shared objects. */
  if (obj == 0)
    {
      size_t size;
      size_t i = 0;
      size_t j = 0;

      /* Determine the number of entries */
      if (dlinfo->rel.table != NULL)
	{
	  for (size = 0; size < dlinfo->rel.size; size += dlinfo->rel.entsize)
	    {
	      Elf32_Rel *entry =
		(Elf32_Rel *) ((uintptr_t) dlinfo->rel.table + size);
	      if (ELF32_R_TYPE (entry->r_info) == R_386_COPY
		  && ELF32_R_SYM (entry->r_info) != STN_UNDEF)
		i++;
	    }
	}
      if (dlinfo->rela.table != NULL)
	{
	  for (size = 0; size < dlinfo->rela.size; size += dlinfo->rela.entsize)
	    {
	      Elf32_Rela *entry =
		(Elf32_Rela *) ((uintptr_t) dlinfo->rela.table + size);
	      if (ELF32_R_TYPE (entry->r_info) == R_386_COPY
		  && ELF32_R_SYM (entry->r_info) != STN_UNDEF)
		i++;
	    }
	}
      rtld_globals = malloc (sizeof (struct rtld_global) * i);
      if (unlikely (rtld_globals == NULL))
	RTLD_NO_MEMORY (dlinfo->name);

      /* Add symbol names to table entries */
      if (dlinfo->rel.table != NULL)
	{
	  for (size = 0; size < dlinfo->rel.size; size += dlinfo->rel.entsize)
	    {
	      Elf32_Rel *entry =
		(Elf32_Rel *) ((uintptr_t) dlinfo->rel.table + size);
	      if (ELF32_R_TYPE (entry->r_info) == R_386_COPY
		  && ELF32_R_SYM (entry->r_info) != STN_UNDEF)
		{
		  Elf32_Sym *symbol =
		    rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
		  rtld_globals[j].name = dlinfo->strtab.table + symbol->st_name;
		  rtld_globals[j].final_addr = (void *) symbol->st_value;
		  rtld_globals[j].size = symbol->st_size;
		  j++;
		}
	    }
	}
      if (dlinfo->rela.table != NULL)
	{
	  for (size = 0; size < dlinfo->rela.size; size += dlinfo->rela.entsize)
	    {
	      Elf32_Rela *entry =
		(Elf32_Rela *) ((uintptr_t) dlinfo->rela.table + size);
	      if (ELF32_R_TYPE (entry->r_info) == R_386_COPY
		  && ELF32_R_SYM (entry->r_info) != STN_UNDEF)
		{
		  Elf32_Sym *symbol =
		    rtld_get_symbol (dlinfo, ELF32_R_SYM (entry->r_info));
		  rtld_globals[j].name = dlinfo->strtab.table + symbol->st_name;
		  rtld_globals[j].final_addr = (void *) symbol->st_value;
		  rtld_globals[j].size = symbol->st_size;
		  j++;
		}
	    }
	}
      rtld_global_count = i;
    }

  /* Load shared libraries */
  for (entry = dlinfo->dynamic; entry->d_tag != DT_NULL; entry++)
    {
      if (entry->d_tag == DT_NEEDED)
	{
	  const char *name = dlinfo->strtab.table + entry->d_un.d_val;
	  unsigned int *temp;
	  unsigned int lib;
	  if (unlikely (*name == '\0'))
	    continue;
	  lib = rtld_load_shlib (name, priority + 1, mode);
	  temp = realloc (dlinfo->deps.deps,
			  sizeof (unsigned int) * (dlinfo->deps.count + 1));
	  if (unlikely (temp == NULL))
	    RTLD_NO_MEMORY (name);
	  dlinfo->deps.deps = temp;
	  dlinfo->deps.deps[dlinfo->deps.count++] = lib;
	}
    }
}

void
rtld_load_segment (int fd, Elf32_Phdr *phdr, struct rtld_info *dlinfo)
{
  struct segment_node *segment;
  void *addr;
  int prot = 0;

  /* Map memory region to contain contents of program header*/
  if (phdr->p_offset == 0)
    {
      addr = mmap (NULL, phdr->p_memsz, PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
      dlinfo->loadbase = addr;
      dlinfo->offset = addr;
    }
  else
    {
      uintptr_t vaddr;
      rtld_check_offset (dlinfo);
      vaddr = (uintptr_t) dlinfo->offset + phdr->p_vaddr;
      addr = mmap ((void *) (vaddr & 0xfffff000), phdr->p_memsz,
		   PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    }
  if (unlikely (addr == MAP_FAILED))
    {
      fprintf (stderr, "ld.so: %s: failed to map memory: %s\n",
	       dlinfo->name, strerror (errno));
      abort ();
    }

  /* Read contents into memory */
  if (unlikely (pread (fd, dlinfo->offset + phdr->p_vaddr, phdr->p_filesz,
		       phdr->p_offset) != phdr->p_filesz))
    {
      fprintf (stderr, "ld.so: %s: failed to load segment\n", dlinfo->name);
      abort ();
    }
  memset (dlinfo->offset + phdr->p_vaddr + phdr->p_filesz, 0,
	  phdr->p_memsz - phdr->p_filesz);

  if (phdr->p_flags & PF_R)
    prot |= PROT_READ;
  if (phdr->p_flags & PF_W)
    prot |= PROT_WRITE;
  if (phdr->p_flags & PF_X)
    prot |= PROT_EXEC;
  segment = malloc (sizeof (struct segment_node));
  if (unlikely (segment == NULL))
    RTLD_NO_MEMORY (dlinfo->name);
  segment->addr = addr;
  segment->len = phdr->p_memsz;
  segment->prot = prot;
  segment->next = NULL;
  if (dlinfo->segments.tail == NULL)
    dlinfo->segments.head = segment;
  else
    dlinfo->segments.tail->next = segment;
  dlinfo->segments.tail = segment;
}

void
rtld_load_phdrs (int fd, Elf32_Ehdr *ehdr, struct rtld_info *dlinfo)
{
  Elf32_Phdr *phdr = malloc (sizeof (Elf32_Phdr));
  Elf32_Dyn *dynamic = NULL;
  int i;
  if (unlikely (phdr == NULL))
    RTLD_NO_MEMORY (dlinfo->name);

  for (i = 0; i < ehdr->e_phnum; i++)
    {
      if (pread (fd, phdr, sizeof (Elf32_Phdr),
		 ehdr->e_phoff + ehdr->e_phentsize * i) != sizeof (Elf32_Phdr))
	{
	  fprintf (stderr, "ld.so: %s: failed to read program header\n",
		   dlinfo->name);
	  abort ();
	}
      if (phdr->p_type == PT_LOAD)
	rtld_load_segment (fd, phdr, dlinfo);
      else if (phdr->p_type == PT_DYNAMIC)
	{
	  rtld_check_offset (dlinfo);
	  dynamic = dlinfo->offset + phdr->p_vaddr;
	}
    }

  if (unlikely (dynamic == NULL))
    {
      fprintf (stderr, "ld.so: %s: missing dynamic section\n", dlinfo->name);
      abort ();
    }
  dlinfo->dynamic = dynamic;
  free (phdr);
}

unsigned int
rtld_load_shlib (const char *name, unsigned long priority, int mode)
{
  int fd;
  unsigned int i;
  /* Check if a library with the same name has already been loaded */
  for (i = 1; i < MAX_SHLIBS && rtld_shlibs[i].name != NULL; i++)
    {
      if (strcmp (name, rtld_shlibs[i].name) == 0)
	return i; /* Already loaded */
    }
  if (i == MAX_SHLIBS)
    {
      fprintf (stderr, "ld.so: %s: max shared object limit reached\n", name);
      abort ();
    }

  rtld_shlibs[i].name = name;
  fd = rtld_open_shlib (name);
  if (fd == -1)
    {
      fprintf (stderr, "ld.so: %s: failed to open: %s\n", name,
	       strerror (errno));
      abort ();
    }
  rtld_shlibs[i].fd = fd;
  rtld_map_elf (fd, &rtld_shlibs[i]);
  rtld_load_dynamic (i, priority, mode);
  rtld_relocate (i, mode);
  return i;
}
