/* queue.c -- This file is part of OS/0 libc.
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

/* Don't abort if we can't allocate memory */

#undef  RTLD_NO_MEMORY
#define RTLD_NO_MEMORY do			\
    {						\
      strcpy (err, "couldn't allocate memory"); \
      return -1;				\
    }						\
  while (0)

/* Functions called by userspace libdl API. These functions set an error
   message and return a failure status instead of aborting */

#define rtld_dlopen_check_offset(dlinfo) do				\
    {									\
      if (unlikely (dlinfo->offset == NULL))				\
	{								\
	  strcpy (err, "load offset requested before object was loaded"); \
	  return -1;							\
	}								\
    }									\
  while (0)

static int rtld_dlopen_shlib (const char *name, char *err, int mode);

static int
rtld_dlopen_load_dynamic (struct rtld_info *dlinfo, char *err, int mode)
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
	  dlinfo->init = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	case DT_FINI:
	  dlinfo->fini = dlinfo->offset + entry->d_un.d_ptr;
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
	      strcpy (err, "bad value for PLT relocation type");
	      return -1;
	    }
	  break;
	case DT_JMPREL:
	  dlinfo->pltrel.table = dlinfo->offset + entry->d_un.d_ptr;
	  break;
	}
    }

  if (dlinfo->strtab.table == NULL)
    {
      strcpy (err, "missing dynamic string table");
      return -1;
    }
  else if (dlinfo->symtab.table == NULL)
    {
      strcpy (err, "missing dynamic symbol table");
      return -1;
    }
  else if (dlinfo->hash == NULL)
    {
      strcpy (err, "missing symbol hash table");
      return -1;
    }

  /* Load shared libraries */
  for (entry = dlinfo->dynamic; entry->d_tag != DT_NULL; entry++)
    {
      if (entry->d_tag == DT_NEEDED)
	{
	  const char *name = dlinfo->strtab.table + entry->d_un.d_val;
	  unsigned int *temp;
	  int obj;
	  if (unlikely (*name == '\0'))
	    continue;
	  obj = rtld_dlopen_shlib (name, err, mode);
	  if (unlikely (obj == -1))
	    return -1;
	  temp = realloc (dlinfo->deps.deps,
			  sizeof (unsigned int) * ++dlinfo->deps.count);
	  if (unlikely (temp == NULL))
	    RTLD_NO_MEMORY;
	  dlinfo->deps.deps = temp;
	  dlinfo->deps.deps[dlinfo->deps.count - 1] = obj;
	}
    }
  return 0;
}

static int
rtld_dlopen_load_segment (int fd, Elf32_Phdr *phdr, struct rtld_info *dlinfo,
			  char *err)
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
      rtld_dlopen_check_offset (dlinfo);
      vaddr = (uintptr_t) dlinfo->offset + phdr->p_vaddr;
      addr = mmap ((void *) (vaddr & 0xfffff000), phdr->p_memsz,
		   PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
    }
  if (unlikely (addr == MAP_FAILED))
    {
      snprintf (err, __DL_ERR_BUFSIZ, "failed to map memory: %s",
		strerror (errno));
      return -1;
    }

  /* Read contents into memory */
  if (unlikely (pread (fd, dlinfo->offset + phdr->p_vaddr, phdr->p_filesz,
		       phdr->p_offset) != phdr->p_filesz))
    {
      munmap (addr, phdr->p_memsz);
      strcpy (err, "failed to load segment");
      return -1;
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
    {
      munmap (addr, phdr->p_memsz);
      RTLD_NO_MEMORY;
    }
  segment->addr = addr;
  segment->len = phdr->p_memsz;
  segment->prot = prot;
  segment->next = NULL;
  if (dlinfo->segments.tail == NULL)
    dlinfo->segments.head = segment;
  else
    dlinfo->segments.tail->next = segment;
  dlinfo->segments.tail = segment;
  return 0;
}

static int
rtld_dlopen_load_phdrs (int fd, Elf32_Ehdr *ehdr, struct rtld_info *dlinfo,
			char *err)
{
  Elf32_Phdr *phdr = malloc (sizeof (Elf32_Phdr));
  Elf32_Dyn *dynamic = NULL;
  struct segment_node *segment;
  struct segment_node *temp;
  int i;
  if (unlikely (phdr == NULL))
    RTLD_NO_MEMORY;

  for (i = 0; i < ehdr->e_phnum; i++)
    {
      if (pread (fd, phdr, sizeof (Elf32_Phdr),
		 ehdr->e_phoff + ehdr->e_phentsize * i) != sizeof (Elf32_Phdr))
	{
	  strcpy (err, "failed to read program header");
	  goto err;
	}
      if (phdr->p_type == PT_LOAD)
	{
	  if (rtld_dlopen_load_segment (fd, phdr, dlinfo, err) == -1)
	    goto err;
	}
      else if (phdr->p_type == PT_DYNAMIC)
	{
	  rtld_dlopen_check_offset (dlinfo);
	  dynamic = dlinfo->offset + phdr->p_vaddr;
	}
    }

  if (unlikely (dynamic == NULL))
    {
      strcpy (err, "missing dynamic section");
      goto err;
    }
  dlinfo->dynamic = dynamic;
  free (phdr);
  return 0;

 err:
  for (segment = dlinfo->segments.head; segment != NULL; segment = temp)
    {
      munmap (segment->addr, segment->len);
      temp = segment->next;
      free (segment);
    }
  free (phdr);
  return -1;
}

static int
rtld_dlopen_map_elf (int fd, struct rtld_info *dlinfo, char *err)
{
  Elf32_Ehdr *ehdr = malloc (sizeof (Elf32_Ehdr));
  int ret;
  if (unlikely (ehdr == NULL))
    RTLD_NO_MEMORY;
  ret = read (fd, ehdr, sizeof (Elf32_Ehdr));
  if (ret != sizeof (Elf32_Ehdr))
    {
      strcpy (err, "bad ELF header");
      return -1;
    }
  if (ehdr->e_ident[EI_MAG0] != ELFMAG0
      || ehdr->e_ident[EI_MAG1] != ELFMAG1
      || ehdr->e_ident[EI_MAG2] != ELFMAG2
      || ehdr->e_ident[EI_MAG3] != ELFMAG3)
    {
      strcpy (err, "bad ELF magic number");
      return -1;
    }
  if (ehdr->e_ident[EI_CLASS] != ELFCLASS32
      || ehdr->e_ident[EI_DATA] != ELFDATA2LSB
      || ehdr->e_type != ET_DYN
      || ehdr->e_machine != MACHTYPE)
    {
      strcpy (err, "bad ELF object/machine type");
      return -1;
    }
  return rtld_dlopen_load_phdrs (fd, ehdr, dlinfo, err);
}

static int
rtld_dlopen_shlib (const char *name, char *err, int mode)
{
  int fd = -1;
  unsigned int i;

  /* Passing NULL to dlopen will return the handle of the executable */
  if (name == NULL)
    return 0;

  /* Check if a library with the same name has already been loaded */
  for (i = 0; i < MAX_SHLIBS && rtld_shlibs[i].name != NULL; i++)
    {
      if (strcmp (name, rtld_shlibs[i].name) == 0)
	return i;
    }
  if (i == MAX_SHLIBS)
    {
      strcpy (err, "max shared object limit reached");
      return -1;
    }

  rtld_shlibs[i].name = name;
  rtld_shlibs[i].dlopened = 1;
  fd = rtld_open_shlib (name);
  if (fd == -1)
    {
      snprintf (err, __DL_ERR_BUFSIZ, "failed to open: %s", strerror (errno));
      goto err;
    }
  rtld_shlibs[i].fd = fd;
  if (rtld_dlopen_map_elf (fd, &rtld_shlibs[i], err) == -1)
    goto err;
  if (rtld_dlopen_load_dynamic (&rtld_shlibs[i], err, mode) == -1)
    goto err;
  rtld_relocate (i, mode);
  if (rtld_shlibs[i].init != NULL)
    rtld_shlibs[i].init ();
  return i;

 err:
  close (rtld_shlibs[i].fd);
  memset (&rtld_shlibs[i], 0, sizeof (struct rtld_info));
  return -1;
}

static int
rtld_closelib (unsigned int obj, char *err)
{
  struct segment_node *segment;
  struct segment_node *temp;
  int i;
  if (!rtld_shlibs[obj].dlopened)
    {
      strcpy (err, "object is in use by executable");
      return -1;
    }
  for (segment = rtld_shlibs[obj].segments.head; segment != NULL;
       segment = temp)
    {
      munmap (segment->addr, segment->len);
      temp = segment->next;
      free (segment);
    }
  if (rtld_shlibs[obj].fini != NULL)
    rtld_shlibs[obj].fini ();
  for (i = 0; i < rtld_shlibs[obj].deps.count; i++)
    {
      if (rtld_closelib (rtld_shlibs[obj].deps.deps[i], err) == -1)
	return -1;
    }
  free (rtld_shlibs[obj].deps.deps);
  memset (&rtld_shlibs[obj], 0, sizeof (struct rtld_info));
  return 0;
}

void *
rtld_dlopen (const char *name, char *err, int mode)
{
  int obj;
  if (unlikely (*name == '\0'))
    {
      strcpy (err, "bad object name");
      return NULL;
    }
  obj = rtld_dlopen_shlib (name, err, mode);
  if (unlikely (obj == -1))
    return NULL;
  return rtld_shlibs[obj].loadbase;
}

void *
rtld_dlsym (void *handle, const char *name, char *err)
{
  int i;
  for (i = 0; i < MAX_SHLIBS; i++)
    {
      if (rtld_shlibs[i].loadbase == handle)
	{
	  void *addr = rtld_lookup_symbol (name, i, 1);
	  if (unlikely (addr == NULL))
	    {
	      strcpy (err, "symbol not found");
	      return NULL;
	    }
	  return addr;
	}
    }
  strcpy (err, "bad handle");
  return NULL;
}

int
rtld_dlclose (void *handle, char *err)
{
  int i;
  if (rtld_shlibs->loadbase == handle)
    return 0; /* Closing a global handle */
  for (i = 1; i < MAX_SHLIBS; i++)
    {
      if (rtld_shlibs[i].loadbase == handle)
	return rtld_closelib (i, err);
    }
  strcpy (err, "bad handle");
  return -1;
}
