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

struct rtld_info
{
  const char *name;         /* Name of object */
  void *loadbase;           /* Address of ELF header */
  void *offset;             /* Offset to add to relocations */
  Elf32_Dyn *dynamic;       /* Address of ELF .dynamic section */
  void *pltgot;             /* Address of PLT/GOT */
  Elf32_Word *hash;         /* Symbol hash table */
  struct elf_strtab strtab; /* Symbol string table */
  struct elf_symtab symtab; /* Dynamic symbol table */
  struct elf_relatab rela;  /* Relocations with explicit addends */
  struct elf_reltab rel;    /* Relocation table */
  struct elf_pltrel pltrel; /* Relocation table for PLT */
};

/* Node for init/fini function queue */

struct queue_node
{
  void *q_data;
  unsigned long q_priority;
  struct queue_node *q_next;
};

__BEGIN_DECLS

extern struct rtld_info rtld_shlibs[MAX_SHLIBS];
extern struct queue_node *rtld_init_func;

void rtld_load_dynamic (struct rtld_info *dlinfo, unsigned long priority);
void rtld_load_shlib (const char *name, unsigned long priority);

/* Utility functions */

void rtld_queue_add (struct queue_node **head, void *data, int priority);
void *rtld_queue_poll (struct queue_node **head);

__END_DECLS

#endif
