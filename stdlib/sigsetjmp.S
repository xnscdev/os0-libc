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

	.section .text
	.global sigsetjmp
	.type sigsetjmp, @function
sigsetjmp:
	mov	4(%esp), %eax

	/* Save registers */
	mov	%ebx, (%eax)
	mov	%esi, 4(%eax)
	mov	%edi, 8(%eax)

	lea	4(%esp), %ecx
	mov	%ecx, 16(%eax)

	/* Save return address and frame pointer */
	mov	(%esp), %ecx
	mov	%ecx, 20(%eax)
	mov	%ebp, 12(%eax)

	/* Save the signal mask if desired and return */
	jmp	__sigsetjmp_maybe_save_mask

	.size sigsetjmp, . - sigsetjmp
