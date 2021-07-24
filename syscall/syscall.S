/* syscall.S -- This file is part of OS/0 libc.
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
	.global syscall
	.type syscall, @function
syscall:
	/* Save registers */
	push	%ebx
	push	%esi
	push	%edi
	push	%ebp

	/* Setup syscall arguments and interrupt */
	mov	20(%esp), %eax
	mov	24(%esp), %ebx
	mov	28(%esp), %ecx
	mov	32(%esp), %edx
	mov	36(%esp), %esi
	mov	40(%esp), %edi
	mov	44(%esp), %ebp
	int	$0x80

	/* Restore registers */
	pop	%ebp
	pop	%edi
	pop	%esi
	pop	%ebx
	jmp	syscall_asm_ret

	.size syscall, . - syscall

	.global syscall_asm_ret
	.hidden syscall_asm_ret
syscall_asm_ret:
	push	%eax
	pushl	24(%esp)
	call	__syscall_handle_err
	add	$8, %esp
	ret

	.size syscall_asm_ret, . - syscall_asm_ret
