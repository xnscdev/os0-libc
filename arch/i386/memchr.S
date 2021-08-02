/*************************************************************************
 * memchr.S -- This file is part of OS/0.                                *
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
	.global memchr
	.weak memchr
	.type memchr, @function
	.global __memchr
	.type __memchr, @function
memchr:
__memchr:
	push	%edi
	mov	8(%esp), %edi
	movzb	12(%esp), %eax
	mov	16(%esp), %ecx
	repne
	scasb
	jne	1f

	lea	-1(%edi), %eax
	pop	%edi
	ret

1:
	xor	%eax, %eax
	pop	%edi
	ret

	.size memchr, . - memchr
	.size __memchr, . - __memchr

	.global memrchr
	.weak memrchr
	.type memrchr, @function
	.global __memrchr
	.type __memrchr, @function
memrchr:
__memrchr:
	push	%edi
	mov	8(%esp), %edi
	movzb	12(%esp), %eax
	mov	16(%esp), %ecx
	lea	-1(%edi,%ecx,1), %edi
	std
	repne
	scasb
	cld
	jne	1f

	lea	1(%edi), %eax
	pop	%edi
	ret

1:
	xor	%eax, %eax
	pop	%edi
	ret

	.size memrchr, . - memrchr
	.size __memrchr, . - __memrchr
