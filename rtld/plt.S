/* plt.S -- This file is part of OS/0 libc.
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

#include <bits/signal.h>

/* Lazy symbol binding handler, called from the PLT */

	.section .text
	.global rtld_lazy_lookup_symbol
	.type rtld_lazy_lookup_symbol, @function
rtld_lazy_lookup_symbol:
	push	%ebp
	mov	%esp, %ebp
	sub	$8, %esp

	mov	8(%ebp), %edx /* Symbol */
	mov	4(%ebp), %ecx /* Shared object ID */

	pushl	$1
	push	%ecx

	/* Determine address in GOT */
	push	%edx
	call	rtld_lazy_get_got_offset
	mov	%eax, -4(%ebp)

	/* Determine symbol name */
	mov	%eax, (%esp)
	call	rtld_lazy_get_symbol_name
	mov	%eax, -8(%ebp)

	/* Lookup symbol address */
	mov	%eax, (%esp)
	call	rtld_lookup_symbol
	add	$4, %esp

	test	%eax, %eax
	jnz	2f

1:
	/* The symbol couldn't be found, abort */
	pushl	-8(%ebp)
	call	rtld_lazy_bind_fail

2:
	/* Write resolved address to GOT */
	mov	-4(%ebp), %ecx
	mov	%eax, (%ecx)

	mov	%ebp, %esp
	pop	%ebp
	add	$8, %esp /* Pushed onto stack by PLT code */
	jmp	*%eax

	.size rtld_lazy_lookup_symbol, . - rtld_lazy_lookup_symbol
