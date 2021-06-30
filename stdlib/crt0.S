/* crt0.S -- This file is part of OS/0 libc.
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

	.section .text
	.global _start
	.type _start, @function
_start:
	/* Kernel places argv in ESI and envp in EDI */
	push	%edi
	call	__libc_set_environ /* Set environ variable */
	push	%esi

	/* Calculate argc */
	xor	%eax, %eax
1:
	mov	(%esi,%eax,4), %edx
	test	%edx, %edx
	jz	2f
	inc	%eax
	jmp	1b

2:
	/* Setup argc on stack */
	push	%eax

	/* Initialize C library */
	call	_init
	pushl	$_fini
	call	atexit
	add	$4, %esp
	call	__libc_init

	/* Call C entry point and exit */
	call	main
	add	$12, %esp
	push	%eax
	call	exit

	.size _start, . - _start
