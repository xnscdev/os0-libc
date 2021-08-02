/* longjmp.S -- This file is part of OS/0 libc.
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

#include <jmpbuf.h>

	.section .text
	.global longjmp
	.type longjmp, @function
longjmp:
	mov	4(%esp), %edx

	/* Get return value and make sure it is nonzero */
	mov	8(%esp), %eax
	test	%eax, %eax
	jnz	1f
	inc	%eax

1:
	mov	EBX_OFFSET(%edx), %ebx
	mov	ESI_OFFSET(%edx), %esi
	mov	EDI_OFFSET(%edx), %edi
	mov	EBP_OFFSET(%edx), %ebp
	mov	ESP_OFFSET(%edx), %esp
	jmp	*EIP_OFFSET(%edx)

	.size longjmp, . - longjmp
