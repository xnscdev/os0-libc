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
  void *dl_loadbase;        /* Address of ELF header */
  Elf32_Dyn *dl_dynamic;    /* Address of ELF .dynamic section */
  void *dl_pltgot;          /* Address of PLT/GOT */
  Elf32_Word *dl_hash;      /* Symbol hash table */
  ELFStringTable dl_strtab; /* Symbol string table */
  ELFSymbolTable dl_symtab; /* Dynamic symbol table */
  ELFRelaTable dl_rela;     /* Relocations with explicit addends */
  ELFRelTable dl_rel;       /* Relocation table */
  void *dl_init;            /* Address of initialization function */
  void *dl_fini;            /* Address of termination function */
  PLTRelTable dl_pltrel;    /* Relocation table for PLT */
} DynamicLinkInfo;

struct _PriorityQueueNode
{
  void *q_data;
  unsigned long q_priority;
  struct _PriorityQueueNode *q_next;
};

typedef struct _PriorityQueueNode PriorityQueueNode;

#define mmap(addr, len, prot, flags, fd, offset)		\
  __rtld_syscall (SYS_mmap, addr, len, prot, flags, fd, offset)
#define munmap(addr, len) __rtld_syscall (SYS_munmap, addr, len)

__BEGIN_DECLS

int __rtld_load_dynamic (DynamicLinkInfo *dlinfo) __hidden;

void *__rtld_memcpy (void *__restrict dest, const void *__restrict src,
		     size_t len) __hidden;
void *__rtld_memset (void *buffer, int c, size_t len) __hidden;

void *__rtld_malloc (size_t len) __hidden;

void __rtld_print (const char *msg) __hidden;
void __rtld_print_ptr (void *ptr) __hidden;

void __rtld_queue_push (PriorityQueueNode **head, void *data,
			int priority) __hidden;
void *__rtld_queue_pop (PriorityQueueNode **head) __hidden;

long __rtld_syscall (long num, ...) __hidden;

__END_DECLS

#endif
