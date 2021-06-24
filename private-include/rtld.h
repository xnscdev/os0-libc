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

#define MAX_SHLIBS 64

/* Macros for determining valid ELF header machine type */
#if   defined ARCH_I386
#define MACHTYPE EM_386
#elif defined ARCH_X86_64
#define MACHTYPE EM_X86_64
#else
#error "No ELF machine type supported for architecture"
#endif

/* Modified versions of the structures defined in kernel <sys/rtld.h> */

typedef struct
{
  char *st_table;
  size_t st_len;
} ELFStringTable;

typedef struct
{
  Elf32_Sym *sym_table;
  size_t sym_entsize;
} ELFSymbolTable;

typedef struct
{
  Elf32_Rela *ra_table;
  size_t ra_size;
  size_t ra_entsize;
} ELFRelaTable;

typedef struct
{
  Elf32_Rel *r_table;
  size_t r_size;
  size_t r_entsize;
} ELFRelTable;

typedef struct
{
  Elf32_Word pt_type;
  void *pt_table;
  size_t pt_size;
} PLTRelTable;

typedef struct
{
  const char *dl_name;      /* Name of object */
  void *dl_loadbase;        /* Address of ELF header */
  Elf32_Dyn *dl_dynamic;    /* Address of ELF .dynamic section */
  void *dl_pltgot;          /* Address of PLT/GOT */
  Elf32_Word *dl_hash;      /* Symbol hash table */
  ELFStringTable dl_strtab; /* Symbol string table */
  ELFSymbolTable dl_symtab; /* Dynamic symbol table */
  ELFRelaTable dl_rela;     /* Relocations with explicit addends */
  ELFRelTable dl_rel;       /* Relocation table */
  PLTRelTable dl_pltrel;    /* Relocation table for PLT */
} DynamicLinkInfo;

/* Node for init/fini function queue */

struct _PriorityQueueNode
{
  void *q_data;
  unsigned long q_priority;
  struct _PriorityQueueNode *q_next;
};

typedef struct _PriorityQueueNode PriorityQueueNode;

__BEGIN_DECLS

extern DynamicLinkInfo rtld_shlibs[MAX_SHLIBS];
extern PriorityQueueNode *rtld_init_func;

void rtld_load_dynamic (DynamicLinkInfo *dlinfo, unsigned long priority);
void rtld_load_shlib (const char *name, unsigned long priority);

/* Utility functions */

void rtld_queue_add (PriorityQueueNode **head, void *data, int priority);
void *rtld_queue_poll (PriorityQueueNode **head);

__END_DECLS

#endif
