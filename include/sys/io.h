/* io.h -- This file is part of OS/0 libc.
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

#ifndef _SYS_IO_H
#define _SYS_IO_H

#include <sys/cdefs.h>
#include <stdint.h>

__attribute__ ((always_inline)) static __inline unsigned char
inb (unsigned short port)
{
  unsigned char value;
  __asm__ volatile ("inb %w1, %0" : "=a" (value) : "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline unsigned char
inb_p (unsigned short port)
{
  unsigned char value;
  __asm__ volatile ("inb %w1, %0\noutb %%al, $0x80" : "=a" (value) :
		    "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline unsigned short
inw (unsigned short port)
{
  unsigned short value;
  __asm__ volatile ("inw %w1, %0" : "=a" (value) : "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline unsigned short
inw_p (unsigned short port)
{
  unsigned short value;
  __asm__ volatile ("inw %w1, %0\noutb %%al, $0x80" : "=a" (value) :
		    "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline unsigned int
inl (unsigned short port)
{
  unsigned int value;
  __asm__ volatile ("inl %w1, %0" : "=a" (value) : "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline unsigned int
inl_p (unsigned short port)
{
  unsigned int value;
  __asm__ volatile ("inl %w1, %0\noutb %%al, $0x80" : "=a" (value) :
		    "Nd" (port));
  return value;
}

__attribute__ ((always_inline)) static __inline void
outb (unsigned char value, unsigned short port)
{
  __asm__ volatile ("outb %b0, %w1" :: "a" (value), "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
outb_p (unsigned char value, unsigned short port)
{
  __asm__ volatile ("outb %b0, %w1\noutb %%al, $0x80" :: "a" (value),
		    "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
outw (unsigned short value, unsigned short port)
{
  __asm__ volatile ("outw %w0, %w1" :: "a" (value), "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
outw_p (unsigned short value, unsigned short port)
{
  __asm__ volatile ("outw %w0, %w1\noutb %%al, $0x80" :: "a" (value),
		    "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
outl (unsigned int value, unsigned short port)
{
  __asm__ volatile ("outl %0, %w1" :: "a" (value), "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
outl_p (unsigned int value, unsigned short port)
{
  __asm__ volatile ("outl %0, %w1\noutb %%al, $0x80" :: "a" (value),
		    "Nd" (port));
}

__attribute__ ((always_inline)) static __inline void
insb (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; insb" : "=D" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

__attribute__ ((always_inline)) static __inline void
insw (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; insw" : "=D" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

__attribute__ ((always_inline)) static __inline void
insl (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; insl" : "=D" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

__attribute__ ((always_inline)) static __inline void
outsb (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; outsb" : "=S" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

__attribute__ ((always_inline)) static __inline void
outsw (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; outsw" : "=S" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

__attribute__ ((always_inline)) static __inline void
outsl (unsigned short port, void *addr, unsigned int count)
{
  __asm__ volatile ("cld; rep; outsl" : "=S" (addr), "=c" (count) : "d" (port),
		    "0" (addr), "1" (count));
}

#endif
