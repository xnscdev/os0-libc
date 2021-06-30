/* brk.S -- This file is part of OS/0 libc.
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

#include <sys/errno.h>
#include <sys/syscall.h>

	.section .text
	.global brk
	.type brk, @function
brk:
	push	%ebx
	mov	$SYS_brk, %eax
	mov	8(%esp), %ebx
	int	$0x80
	cmp	%eax, %ebx
	pop	%ebx

	xor	%eax, %eax
	jne	1f
	ret

1:
	dec	%eax
	push	%eax
	pushl	$ENOMEM
	call	__libc_set_errno
	add	$4, %esp
	pop	%eax
	ret

	.size brk, . - brk

	.global sbrk
	.type sbrk, @function
sbrk:
	push	%ebx

	mov	8(%esp), %ecx
	/* If increment is negative, error out */
	test	%ecx, %ecx
	jns	2f

1:
	pushl	$ENOMEM
	call	__libc_set_errno
	add	$4, %esp
	mov	$-1, %eax
	pop	%ebx
	ret

2:
	/* Determine current program break */
	mov	$SYS_brk, %eax
	xor	%ebx, %ebx
	int	$0x80
	push	%eax

	lea	(%eax,%ecx,1), %ebx
	mov	$SYS_brk, %eax
	int	$0x80
	cmp	%eax, %ebx
	pop	%eax
	jne	1b

	pop	%ebx
	ret

	.size sbrk, . - sbrk
