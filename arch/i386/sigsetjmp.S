/* sigsetjmp.S -- This file is part of OS/0 libc.
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
	.global sigsetjmp
	.type sigsetjmp, @function
sigsetjmp:
	mov	4(%esp), %eax

	/* Save registers */
	mov	%ebx, EBX_OFFSET(%eax)
	mov	%esi, ESI_OFFSET(%eax)
	mov	%edi, EDI_OFFSET(%eax)
	mov	%ebp, EBP_OFFSET(%eax)

	/* Save expected stack pointer on return */
	lea	4(%esp), %ecx
	mov	%ecx, ESP_OFFSET(%eax)

	/* Save return address */
	mov	(%esp), %ecx
	mov	%ecx, EIP_OFFSET(%eax)

	/* Save the signal mask if desired */
	jmp	__sigsetjmp_maybe_save_mask

	.size sigsetjmp, . - sigsetjmp
