#============================================================================
# bandwidth, a benchmark to estimate memory transfer bandwidth.
# Copyright (C) 2005-2017 by Zack T Smith.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# The author may be reached at veritas@comcast.net.
#============================================================================

# CFLAGS= -O6 
CFLAGS= -g -Wno-conditional-type-mismatch
CC=gcc -Wall
SRC=main.c 
OBJ=main.o
LIB= 
AS=nasm
ARM_AS=as
ARM_CC=gcc
OOC_SRC= Object.c Image.c SimpleGraphing.c 
OOC_OBJ= Object.o Image.o SimpleGraphing.o

# Note: Mac 32-bit no longer supported by Mac OS.

message:
	@echo ""
	@echo "To compile for x86 Linux:             make bandwidth32"
	@echo "To compile for x86_64 Linux:          make bandwidth64"
	@echo "To compile for x86_64 Mac OS/X:       make bandwidth-mac64"
	@echo "To compile for Raspberry pi 3 32-bit: make bandwidth-rpi32"
	@echo "To compile for generic ARM Linux:     make bandwidth-arm32"
	@echo "To compile for x86 Win32/Cygwin:      make bandwidth-win32"
	@echo "Note! For the Mac you will need to install the latest NASM; Apple's is insufficient."
	@echo ""

bandwidth-rpi32:  main.c routines-arm-rpi3-32bit.asm OOC32
	${CC} ${CFLAGS} -c ${OOC_SRC}
	ar rvs OOC32.a ${OOC_OBJ}
	${ARM_AS} -march=armv7 -mfpu=neon routines-arm-rpi3-32bit.asm -mcpu=xscale -o routines-arm-rpi3-32bit.o
	${ARM_CC} -DRASPBIAN32 routines-arm-rpi3-32bit.o ${SRC} ${CFLAGS} OOC32.a -lm -o bandwidth-rpi32

bandwidth-arm32:  main.c routines-arm-32bit.asm OOC32
	${CC} -m32 ${CFLAGS} -c ${OOC_SRC}
	ar rvs OOC32.a ${OOC_OBJ}
	${ARM_AS} routines-arm-generic-32bit.asm -mcpu=xscale -o routines-arm-generic-32bit.o
	${ARM_CC} ${CFLAGS} routines-arm-generic-32bit.o ${SRC} ${CFLAGS} OOCP32.a -lm -o bandwidth-arm32

bandwidth64:	main.c routines-x86-64bit.asm OOC64
	${AS} -f elf64 routines-x86-64bit.asm -o routines-x86-64bit.o
	${CC} ${CFLAGS} -m64 ${SRC} routines-x86-64bit.o OOC64.a -lm -o bandwidth64 

bandwidth32:	main.c routines-x86-32bit.asm OOC32
	${AS} -f elf routines-x86-32bit.asm -o routines-x86-32bit.o
	${CC} ${CFLAGS} -m32 routines-x86-32bit.o ${SRC} OOC32.a -lm -o bandwidth32 

bandwidth-mac64:	main.c routines-x86-64bit.asm OOC64
	${AS} -f macho64 routines-x86-64bit.asm -o routines-x86-64bit.o
	${CC} -m64 ${CFLAGS} ${SRC} routines-x86-64bit.o OOC64.a -lm -o bandwidth-mac64

bandwidth-mac32:	main.c routines-x86-32bit.asm OOC32
	${AS} -f macho32 routines-x86-32bit.asm -o routines-x86-32bit.o
	${CC} -m32 ${CFLAGS} ${SRC} routines-x86-32bit.o OOC32.a -lm -o bandwidth-mac32

bandwidth-win32:	main.c routines-x86-32bit.asm OOC32
	${AS} -f macho32 routines-x86-32bit.asm -o routines-x86-32bit.o
	${CC} ${CFLAGS} -D__WIN32__ -DWINVER=0x0600 -m32 -lm ${SRC} routines-x86-32bit.o OOC32.a -o bandwidth-win32

OOC64:
	${CC} -m64 ${CFLAGS} -c ${OOC_SRC}
	ar rvs OOC64.a ${OOC_OBJ}

OOC32:
	${CC} ${CFLAGS} -c ${OOC_SRC}
	ar rvs OOC32.a ${OOC_OBJ}

clean:
	rm -f main.o bandwidth bandwidth32 bandwidth64 routines-x86-32bit.o routines-x86-64bit.o 
	rm -f bandwidth-win32.exe bandwidth.bmp bandwidth-mac32 bandwidth-mac64 bandwidth-rpi32 bandwidth-arm32 routines-arm-rpi3-32bit.o routines-x86-64bit.o
	rm -rf bandwidth-mac64.dSYM bandwidth-mac32.dSYM
	rm -f ${OOC_OBJ} OOC32.a OOC64.a

