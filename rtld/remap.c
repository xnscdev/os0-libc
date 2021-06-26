/* remap.c -- This file is part of OS/0 libc.
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

void
rtld_remap_exec (void)
{
  Elf32_Ehdr *ehdr = rtld_shlibs->loadbase;
  Elf32_Phdr *phdr;
  int i;
  for (i = 0; i < ehdr->e_phnum; i++)
    {
      phdr = (Elf32_Phdr *) (rtld_shlibs->loadbase + ehdr->e_phoff +
			     i * ehdr->e_phentsize);
      if (phdr->p_type == PT_LOAD)
	{
	  void *vaddr = (void *) (((uintptr_t) rtld_shlibs->offset +
				   phdr->p_vaddr) & 0xfffff000);
	  int prot = 0;
	  if (phdr->p_flags & PF_R)
	    prot |= PROT_READ;
	  if (phdr->p_flags & PF_W)
	    prot |= PROT_WRITE;
	  if (phdr->p_flags & PF_X)
	    prot |= PROT_EXEC;
	  if (unlikely (mprotect (vaddr, phdr->p_memsz, prot) == -1))
	    {
	      fprintf (stderr, "ld.so: failed to change executable segment "
		       "protection: %s\n", strerror (errno));
	      abort ();
	    }
	}
    }
}

void
rtld_remap_memory (void)
{
  unsigned int i;
  rtld_remap_exec ();
  for (i = 1; i < MAX_SHLIBS && rtld_shlibs[i].name != NULL; i++)
    {
      struct segment_node *temp;
      for (temp = rtld_shlibs[i].segments.head; temp != NULL; temp = temp->next)
	{
	  if (unlikely (mprotect (temp->addr, temp->len, temp->prot) == -1))
	    {
	      fprintf (stderr, "ld.so: %s: failed to change segment "
		       "protection: %s\n", rtld_shlibs[i].name,
		       strerror (errno));
	      abort ();
	    }
	  close (rtld_shlibs[i].fd);
	}
    }
}
