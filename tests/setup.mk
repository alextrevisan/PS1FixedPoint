PREFIX = mipsel-none-elf-

GCC_VERSION	= 12.2.0

PSN00B_BASE	= ../../psn00bsdk/
LIBDIRS		= -L$(PSN00B_BASE)libpsn00b
INCLUDE	 	= -I$(PSN00B_BASE)libpsn00b/include

ELF2X		= $(PSN00B_BASE)/tools/bin/elf2x

GCC_BASE	= /c/mipsel-none-elf
GCC_BIN		= $(GCC_BASE)/bin/

CC			= $(GCC_BIN)$(PREFIX)gcc
CXX			= $(GCC_BIN)$(PREFIX)g++
AS			= $(GCC_BIN)$(PREFIX)as
AR			= $(GCC_BIN)$(PREFIX)ar
RANLIB		= $(GCC_BIN)$(PREFIX)ranlib
LD			= $(GCC_BIN)$(PREFIX)ld