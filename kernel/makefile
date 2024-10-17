ASM = nasm
ASM_SRC = $(shell find . -type f -name '*.asm')
ASM_OBJS = $(patsubst %.asm, %.s.o, $(ASM_SRC))
ASM_FLAGS = -f elf64
OUTPUT = turquoise.kernel
CPP = g++
CPP_FLAGS = -I ../ -I ./ -Wall \
	-Wextra \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel
CPP_SRCS = $(shell find . -type f -name '*.cpp')
CPP_OBJS = $(patsubst %.cpp, %.cpp.o, $(CPP_SRCS))
LD = ld
LD_FLAGS = -m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	-T linker.ld
EMBED_FILE = $(shell find embed -type f ! -name '*.o')
EMBED_FILE_OBJ = $(patsubst %, %.o, $(EMBED_FILE))

$(OUTPUT): $(CPP_OBJS) $(ASM_OBJS) makefile linker.ld
	@printf "Linking \e[1;32mturquoise.kernel\e[0;37m\n"
	@$(foreach f, $(EMBED_FILE), printf "Embeding \e[1;32m$(f)\e[0;37m\n";objcopy -O elf64-x86-64 -B i386 -I binary $(f) $(f).o;)
	@$(LD) $(LD_FLAGS) $(CPP_OBJS) $(EMBED_FILE_OBJ) $(ASM_OBJS) -o $ $(OUTPUT)
	

%.cpp.o: %.cpp makefile
	@printf "\e[0;37m[CPP] $< \e[1;31m...\e[0;37m\r"
	@$(CPP) $(CPP_FLAGS) -c $< -o $@
	@printf "\e[0;37m[CPP] $< \e[1;32mOK \e[0;37m\n"

%.s.o: %.asm
	@printf "\e[0;37m[ASM] $< \e[1;31m...\e[0;37m\r"
	@$(ASM) $(ASM_FLAGS) $< -o $@
	@printf "\e[0;37m[ASM] $< \e[1;32mOK \e[0;37m\n"

test:
	@echo $(EMBED_FILE)
