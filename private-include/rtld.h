/* rtld.h -- This file is part of OS/0 libc.
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

#ifndef _RTLD_H
#define _RTLD_H

#include <config.h>

#include <bits/mman.h>
#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <elf.h>
#include <stddef.h>

#define RTLD_CACHE_FILE "/var/cache/ld.so.cache"
#define MAX_SHLIBS      64

/* Macros for determining valid ELF header machine type */
#if   defined ARCH_I386
#define MACHTYPE EM_386
#elif defined ARCH_X86_64
#define MACHTYPE EM_X86_64
#else
#error "No ELF machine type supported for architecture"
#endif

/* Node for init/fini function queue */

struct queue_node
{
  void *data;
  unsigned long priority;
  struct queue_node *next;
};

/* Node for segment linked list */

struct segment_node
{
  void *addr;
  size_t len;
  int prot;
  struct segment_node *next;
};

struct rtld_segments
{
  struct segment_node *head;
  struct segment_node *tail;
};

/* ELF wrapper structures */

struct elf_strtab
{
  char *table;
  size_t len;
};

struct elf_symtab
{
  Elf32_Sym *table;
  size_t entsize;
};

struct elf_relatab
{
  Elf32_Rela *table;
  size_t size;
  size_t entsize;
};

struct elf_reltab
{
  Elf32_Rel *table;
  size_t size;
  size_t entsize;
};

struct elf_pltrel
{
  Elf32_Word type;
  void *table;
  size_t size;
};

/* Info about needed objects */

struct obj_deps
{
  size_t count;
  unsigned int *deps;
};

/* Info about a loaded shared object */

struct rtld_info
{
  struct obj_deps deps;          /* Needed shared objects */
  const char *name;              /* Name of object */
  int fd;                        /* File descriptor of object */
  void *loadbase;                /* Address of ELF header */
  void *offset;                  /* Offset to add to relocations */
  Elf32_Dyn *dynamic;            /* Address of ELF .dynamic section */
  void *pltgot;                  /* Address of PLT/GOT */
  Elf32_Word *hash;              /* Symbol hash table */
  struct elf_strtab strtab;      /* Symbol string table */
  struct elf_symtab symtab;      /* Dynamic symbol table */
  struct elf_relatab rela;       /* Relocations with explicit addends */
  struct elf_reltab rel;         /* Relocation table */
  struct elf_pltrel pltrel;      /* Relocation table for PLT */
  struct rtld_segments segments; /* Linked list of loaded segments */
};

#ifndef __FILE_defined
typedef struct __FILE FILE;
#define __FILE_defined
#endif

#define RTLD_NO_MEMORY(name) do						\
    {									\
      fprintf (stderr, "ld.so: %s: couldn't allocate memory\n", name);	\
      abort ();								\
    }									\
  while (0)

__BEGIN_DECLS

extern struct rtld_info rtld_shlibs[MAX_SHLIBS];
extern struct queue_node *rtld_init_func;
extern struct queue_node *rtld_fini_func;

int rtld_open_shlib (const char *name);
void rtld_map_elf (int fd, struct rtld_info *dlinfo);
void rtld_load_dynamic (struct rtld_info *dlinfo, unsigned long priority);
void rtld_load_segment (int fd, Elf32_Phdr *phdr, struct rtld_info *dlinfo);
void rtld_load_phdrs (int fd, Elf32_Ehdr *ehdr, struct rtld_info *dlinfo);
unsigned int rtld_load_shlib (const char *name, unsigned long priority);

int rtld_cache_lookup (FILE *cache, const char *name);
int rtld_search_lib (const char *name);

void rtld_exec_initfini_funcs (void);

void rtld_queue_add (struct queue_node **head, void *data, int priority);
void *rtld_queue_poll (struct queue_node **head);

unsigned long rtld_symbol_hash (const char *name);
void *rtld_lookup_symbol (const char *name, struct rtld_info *dlinfo);
void rtld_perform_rel (Elf32_Rel *entry, struct rtld_info *dlinfo,
		       Elf32_Sword addend);
void rtld_relocate (struct rtld_info *dlinfo);

void rtld_remap_exec (void);
void rtld_remap_memory (void);

__END_DECLS

#endif
