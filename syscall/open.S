/* open.S -- This file is part of OS/0 libc.
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

	.section .text
	.global open
	.type open, @function
open:
	push	%ebx
	mov	8(%esp), %ebx
	mov	12(%esp), %ecx
	mov	16(%esp), %edx
	mov	$SYS_open, %eax
	int	$0x80
	pop	%ebx
	jmp	syscall_asm_ret

	.size open, . - open

	.global openat
	.type openat, @function
openat:
	push	%ebx
	push	%esi
	mov	12(%esp), %ebx
	mov	16(%esp), %ecx
	mov	20(%esp), %edx
	mov	24(%esp), %esi
	mov	$SYS_openat, %eax
	int	$0x80
	pop	%esi
	pop	%ebx
	jmp	syscall_asm_ret

	.size openat, . - openat
