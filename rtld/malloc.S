/* malloc.S -- This file is part of OS/0 libc.
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

#include <sys/syscall.h>

/* Stupid malloc implementation for the dynamic linker, there is no
   corresponding free function because it is not necessary; the program break
   cannot be lowered */

	.section .text
	.global __rtld_malloc
	.hidden __rtld_malloc
	.type __rtld_malloc, @function
__rtld_malloc:
	push	%ebx
	mov	$SYS_brk, %eax
	xor	%ebx, %ebx
	int	$0x80

	mov	8(%esp), %ecx
	lea	(%eax,%ecx,1), %ebx
	mov	$SYS_brk, %eax
	int	$0x80
	pop	%ebx
	ret

	.size __rtld_malloc, . - __rtld_malloc
