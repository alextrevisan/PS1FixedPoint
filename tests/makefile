include setup.mk

# Project target name
TARGET		= FixedPoint

#TOOLS
MKPSXISO    = tools/mkpsxiso.exe
MKPSXISO_XML= iso_build/FixedPoint.xml
BIN_FOLDER  = bin
ISO_FOLDER  = iso

#EMULATOR
EMUBIN = D:\\Games\\Emuladores\\Playstation\\ePSXe.exe
EMU_CMD = $(EMUBIN) -nogui -loadbin iso/$(TARGET).cue

#ENGINE
ENGINE_DIR = ..\\FixedPoint

# Searches for C, C++ and S (assembler) files in local directory
CFILES		= $(notdir $(wildcard *.c))
CPPFILES 	= $(notdir $(wildcard *.cpp))
AFILES		= $(notdir $(wildcard *.s))

CPPENGINE =  $(notdir $(wildcard $(ENGINE_DIR)/*.cpp))

# Determine object files
#ENGINEFILES = $(addprefix build/engine/,$(CPPENGINE:.cpp=.o))

OFILES		= $(addprefix build/,$(CFILES:.c=.o)) \
			$(addprefix build/,$(CPPFILES:.cpp=.o)) \
			$(addprefix build/,$(AFILES:.s=.o)) 


# Project specific include and library directories
# (use -I for include dirs, -L for library dirs)
INCLUDE	 	+=	-I. -Iengine -Itests -Iengine/include
LIBDIRS		+=

# Libraries to link
LIBS		= -lpsxgpu -lpsxgte -lpsxspu -lpsxetc -lpsxapi -lc

# C compiler flags
CFLAGS		= -g -O3 -fno-builtin -fdata-sections -ffunction-sections -Wno-narrowing

# C++ compiler flags
CPPFLAGS	= $(CFLAGS) -fno-exceptions \
						-fno-rtti \
						-fno-unwind-tables \
						-fno-threadsafe-statics \
						-fno-use-cxa-atexit \
						-Wno-narrowing \
						-std=c++20
						
# Assembler flags
AFLAGS		= -g -msoft-float

# Linker flags
LDFLAGS		= -g -Ttext=0x80010000 -gc-sections \
			-T $(GCC_BASE)/mipsel-none-elf/lib/ldscripts/elf32elmip.x


all: $(OFILES) $(ENGINEFILES)
	@mkdir -p $(BIN_FOLDER)
	$(LD) $(LDFLAGS) $(LIBDIRS) $(OFILES) $(ENGINEFILES) $(LIBS)  -o bin/$(TARGET)
	$(ELF2X) -q $(BIN_FOLDER)/$(TARGET) $(BIN_FOLDER)/$(TARGET).exe
	$(ELF2X) -q $(BIN_FOLDER)/$(TARGET) $(BIN_FOLDER)/$(TARGET)

iso: all
	@mkdir -p $(ISO_FOLDER)
	@rm -rf $(ISO_FOLDER)/*.cue $(ISO_FOLDER)/*.bin
	@$(MKPSXISO) $(MKPSXISO_XML)
	@mv *.cue $(ISO_FOLDER)
	@mv *.bin $(ISO_FOLDER)

run: iso
	$(EMU_CMD)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(AFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(AFLAGS) $(INCLUDE) -c $< -o $@


	
build/engine/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(AFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@





clean:
	rm -rf build $(BIN_FOLDER) $(ISO_FOLDER) *.bin *.cue