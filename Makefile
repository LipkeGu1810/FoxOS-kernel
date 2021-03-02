OBJDIR = lib
BUILDDIR = bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CPPSRC = $(call rwildcard,./,*.cpp)
ASMSRC = $(call rwildcard,./,*.asm)
CSRC = $(call rwildcard,./,*.c)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(CPPSRC))
OBJS += $(patsubst %.asm, $(OBJDIR)/%.o, $(ASMSRC))
OBJS += $(patsubst %.c, $(OBJDIR)/%.o, $(CSRC))
DIRS = $(wildcard ./)

CC = gcc
ASM = nasm
LD = ld

CPPFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -Iinclude -fno-use-cxa-atexit -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-builtin-declaration-mismatch
CFLAGS = -Wall -fno-stack-protector -Iinclude -Wno-builtin-declaration-mismatch -Wno-discarded-qualifiers
ASMFLAGS = -f elf64
LDFLAGS = -static -Bsymbolic -nostdlib -Tlink.ld

foxkrnl.elf: $(OBJS)
	@echo LD $^
	@$(LD) $(LDFLAGS) -o $(BUILDDIR)/$@ $^

$(OBJDIR)/.//interrupts/interrupts.o: interrupts/interrupts.cpp
	@echo CPP INTR $^
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -mgeneral-regs-only -c -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@echo CPP $^
	@mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) -c -o $@ $^

$(OBJDIR)/%.o: %.asm
	@echo ASM $^
	@mkdir -p $(@D)
	@$(ASM) $(ASMFLAGS) -o $@ $^
 
$(OBJDIR)/%.o: %.c
	@echo CC $^
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $^

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(OBJDIR)

clean:
	rm $(OBJS)
	rm $(BUILDDIR)/foxkrnl.elf

.PHONY: clean $(BUILDDIR)/foxkrnl.elf