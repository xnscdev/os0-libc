/* libdl.c -- This file is part of OS/0 libc.
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
#include <string.h>

#define RTLD_LOAD_ADDR 0x00008000

#define __DL_OPEN_FUNC_NAME  "rtld_dlopen"
#define __DL_SYM_FUNC_NAME   "rtld_dlsym"
#define __DL_CLOSE_FUNC_NAME "rtld_dlclose"

static char __dl_err_str[__DL_ERR_BUFSIZ];
static char *__dl_strtab;
static uintptr_t __dl_symtab;
static size_t __dl_symsize;
static Elf32_Word *__dl_hash;
static void *(*__dl_open_func) (const char *, char *, int);
static void *(*__dl_sym_func) (void *, const char *, char *);
static int (*__dl_close_func) (void *, char *);

static void
__dl_init_rtld_params (void)
{
  Elf32_Ehdr *ehdr = (Elf32_Ehdr *) RTLD_LOAD_ADDR;
  Elf32_Dyn *dynamic = NULL;
  Elf32_Phdr *phdr;
  int i;
  for (i = 0; i < ehdr->e_phnum; i++)
    {
      phdr = (Elf32_Phdr *) (RTLD_LOAD_ADDR + ehdr->e_phoff +
			     ehdr->e_phentsize * i);
      if (phdr->p_type == PT_DYNAMIC)
	{
	  dynamic = (Elf32_Dyn *) (RTLD_LOAD_ADDR + phdr->p_vaddr);
	  break;
	}
    }
  for (; dynamic->d_tag != DT_NULL; dynamic++)
    {
      switch (dynamic->d_tag)
	{
	case DT_HASH:
	  __dl_hash = (Elf32_Word *) (RTLD_LOAD_ADDR + dynamic->d_un.d_ptr);
	  break;
	case DT_STRTAB:
	  __dl_strtab = (char *) (RTLD_LOAD_ADDR + dynamic->d_un.d_ptr);
	  break;
	case DT_SYMTAB:
	  __dl_symtab = RTLD_LOAD_ADDR + dynamic->d_un.d_ptr;
	  break;
	case DT_SYMENT:
	  __dl_symsize = dynamic->d_un.d_val;
	  break;
	}
    }
}

static unsigned long
__dl_symbol_hash (const char *name)
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

static void *
__dl_lookup_rtld_sym (const char *name)
{
  Elf32_Word nbucket;
  Elf32_Word nchain;
  Elf32_Word *bucket;
  Elf32_Word *chain;
  unsigned long hash;
  Elf32_Word y;
  if (unlikely (__dl_hash == NULL))
    __dl_init_rtld_params ();
  nbucket = __dl_hash[0];
  nchain = __dl_hash[1];
  bucket = &__dl_hash[2];
  chain = &bucket[nbucket];
  hash = __dl_symbol_hash (name);
  y = bucket[hash % nbucket];
  while (y != STN_UNDEF)
    {
      Elf32_Sym *symbol = (Elf32_Sym *) (__dl_symtab + y * __dl_symsize);
      if (strcmp (__dl_strtab + symbol->st_name, name) == 0)
	{
	  if (ELF32_ST_BIND (symbol->st_info) != STB_GLOBAL)
	    return NULL;
	  return (void *) (symbol->st_value + RTLD_LOAD_ADDR);
	}
      y = chain[y % nchain];
    }
  return NULL; /* Could not find symbol */
}

void *
dlopen (const char *path, int mode)
{
  if (unlikely (__dl_open_func == NULL))
    __dl_open_func = __dl_lookup_rtld_sym (__DL_OPEN_FUNC_NAME);
  if (!(mode & (RTLD_LAZY | RTLD_NOW)))
    mode |= RTLD_LAZY;
  return __dl_open_func (path, __dl_err_str, mode);
}

void *
dlsym (void *handle, const char *symbol)
{
  if (unlikely (__dl_sym_func == NULL))
    __dl_sym_func = __dl_lookup_rtld_sym (__DL_SYM_FUNC_NAME);
  return __dl_sym_func (handle, symbol, __dl_err_str);
}

char *
dlerror (void)
{
  static char buffer[__DL_ERR_BUFSIZ];
  if (*__dl_err_str == '\0')
    return NULL;
  strncpy (buffer, __dl_err_str, __DL_ERR_BUFSIZ);
  *__dl_err_str = '\0';
  return buffer;
}

int
dlclose (void *handle)
{
  if (unlikely (__dl_close_func == NULL))
    __dl_close_func = __dl_lookup_rtld_sym (__DL_CLOSE_FUNC_NAME);
  return __dl_close_func (handle, __dl_err_str);
}
