/*************************************************************************
 * memcpy.S -- This file is part of OS/0.                                *
 * Copyright (C) 2021 XNSC                                               *
 *                                                                       *
 * OS/0 is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * OS/0 is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with OS/0. If not, see <https://www.gnu.org/licenses/>.         *
 *************************************************************************/

	.section .text
	.global memcpy
	.weak memcpy
	.type memcpy, @function
	.global __memcpy
	.type __memcpy, @function
memcpy:
__memcpy:
	push	%esi
	push	%edi
	mov	12(%esp), %edi
	mov	16(%esp), %esi
	mov	20(%esp), %ecx
	mov	%edi, %eax

#ifdef MEMMOVE
	cmp	%esi, %edi
	ja	.backward
	je	.done
#endif

	rep
	movsb
	pop	%edi
	pop	%esi
	ret

#ifdef MEMMOVE
.backward:
	lea	-1(%edi,%ecx,1), %edi
	lea	-1(%esi,%ecx,1), %esi
	std
	rep
	movsb
	cld

.done:
	pop	%edi
	pop	%esi
	ret
#endif

	.size memcpy, . - memcpy
	.size __memcpy, . - __memcpy

#ifndef MEMMOVE

	.global mempcpy
	.weak mempcpy
	.type mempcpy, @function
	.global __mempcpy
	.type __mempcpy, @function
mempcpy:
__mempcpy:
	push	%esi
	push	%edi
	mov	12(%esp), %edi
	mov	16(%esp), %esi
	mov	20(%esp), %ecx
	lea	(%edi,%ecx,1), %eax
	rep
	movsb
	pop	%edi
	pop	%esi
	ret

	.size mempcpy, . - mempcpy
	.size __mempcpy, . - mempcpy

#endif
