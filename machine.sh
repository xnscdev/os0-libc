#!/bin/sh

# Determine the architecture-specific directory name and other information
# based on the CPU name given by meson

ARCH=
case "$1" in
    x86 )    ARCH=i386 ;;
    x86_64 ) ARCH=x86_64 ;;
    *)       exit 1 ;;
esac

BITS=
case "$1" in
    arm|csky|m68k|mips|ppc|riscv32|sparc|x86 )
	BITS=32
	;;
    aarch64|alpha|ia64|mips64|ppc64|riscv64|sparc64|x86_64 )
	BITS=64
	;;
esac

echo "$ARCH $BITS"
