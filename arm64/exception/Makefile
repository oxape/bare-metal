UNAME_S := $(shell uname -s)
ifeq ($(CROSS_COMPILE),)

CROSS_COMPILE := $(shell if aarch64-elf-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'aarch64-elf-'; \
	elif aarch64-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'aarch64-linux-gnu-'; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find a riscv64 version of GCC/binutils." 1>&2; \
	echo "*** To turn off this error, run 'gmake TOOLPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)

endif

# Base address to load the program
RAM_BASE=0x40000000
LINKER_SCRIPTS="link.ld"

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump 

SRC = src
BUILD = build

.PHONY: clean all dump debug qemu qemu-gdb

# define
CFLAGS += -DPRINTF_ALIAS_STANDARD_FUNCTION_NAMES_SOFT=1
# -O0          		: Disable optimizations
# -g           		: Generate debug info
# -ffixed-r8   		: Don't touch register r8
# -fno-builtin 		: https://stackoverflow.com/questions/54281780/what-exactly-is-fno-builtin-doing-here
# -ffreestanding 	: https://stackoverflow.com/questions/17692428/what-is-ffreestanding-option-in-gcc
# -mcmodel=medany   : https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html
CFLAGS += -g -Og
# disable generate fpu simd instructions
#CFLAGS += -mgeneral-regs-only
CFLAGS += -march=armv8-a -mcpu=cortex-a57
CFLAGS += -fno-builtin -ffreestanding
LDFLAGS += -nostdlib
# LDFLAGS += -L/Users/starot/bin/gcc-arm-none-eabi-10.3-2021.10/lib/gcc/arm-none-eabi/10.3.1/ -lgcc 

# Name of target program
PROG=main

# List of all *c sources
# simple_printf no need fpu or simd
#  src/simple_print/simple_printf.c

CSRC = \
 src/print/printf.c\
 src/uart/uart.c\
 src/main.c

#List of all *S (asm) sources
ASRC= \
 src/start.S\
 src/vector.S\
 src/asm/currentEL.S\
 src/asm/callee.S\
 src/asm/ldr_str.S\
 src/asm/cmp.S\
 src/asm/adr.S\
 src/asm/pseudo.S\
 src/asm/macro.S

TMP_OBJ += $(CSRC:.c=.o)
TMP_OBJ += $(ASRC:.S=.o)
DIR_PATH = $(dir $(TMP_OBJ))
# 使用sort函数排序(sort函数会去除重复项)
DIRS := $(sort $(DIR_PATH))

OBJ = $(patsubst %, $(BUILD)/%, $(TMP_OBJ))

$(foreach n, $(DIRS), $(shell mkdir -p "$(BUILD)/$(n)"))

all: $(BUILD)/$(PROG).bin

$(BUILD)/%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/$(PROG).elf: $(OBJ)
	echo $(OBJ)
	$(LD) -T $(LINKER_SCRIPTS) $^ $(LDFLAGS) -o $@

$(BUILD)/$(PROG).bin: $(BUILD)/$(PROG).elf
	$(OBJCOPY) -v -O binary $^ $@
	$(OBJDUMP) --visualize-jumps=off -S $(BUILD)/$(PROG).elf > $(PROG).asm
	$(OBJDUMP) -t $(BUILD)/$(PROG).elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(PROG).sym

clean:
	rm -rf build/*
	rm -f *.asm
	rm -f *.sym
	rm -f *.a
	rm -f *.o 
	rm -f *.elf
	rm -f *.v
	rm -f *.bin
	rm -f *.md5
	rm -f *.hex
	rm -f *.dmp
	rm -f .gdbinit


# Show the disassembly
disassemble: $(BUILD)/$(PROG).elf
	$(OBJDUMP) -S $(BUILD)/$(PROG).elf > $(PROG).asm
	$(OBJDUMP) -t $(BUILD)/$(PROG).elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(PROG).sym

# Connect to the remote gdb and start debugging
debug: $(BUILD)/$(PROG).bin
	rm .gdbscript ; \
	echo 'target remote 10.7.9.19:4000' >> .gdbscript ; \
	echo 'restore $(BUILD)/$(PROG).bin binary $(RAM_BASE)' >> .gdbscript ; \
	echo 'jump *$(RAM_BASE)' >> .gdbscript ; \
	$(GDB) -x .gdbscript $(BUILD)/$(PROG).elf ;

# run in qemu

QEMU = qemu-system-aarch64

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)
ifndef CPUS
CPUS := 1
endif

QEMUOPTS = -M virt -cpu cortex-a57 -m 8G -nographic -smp $(CPUS)
# enable highmem
QEMUOPTS += -machine highmem=on
# enable EL2
QEMUOPTS += -machine virtualization=on
# enable EL3
# QEMUOPTS += -machine secure=on

qemu: all
	$(QEMU) $(QEMUOPTS) -kernel $(BUILD)/$(PROG).elf

.gdbinit: .gdbinit.tmpl
	sed -e "s#:1234#:$(GDBPORT)#" -e "s#xxx.elf#$(BUILD)/$(PROG).elf#" < $^ > $@

qemu-gdb: all .gdbinit
	@echo "*** Now run 'gdb' in another window." 1>&2
	$(QEMU) $(QEMUOPTS) -kernel $(BUILD)/$(PROG).elf -S $(QEMUGDB)


.PHONY: obj
obj:
	@echo $(OBJ)

tmp_obj:
	@echo $(TMP_OBJ)

dir_path:
	@echo $(words $(DIR_PATH))

dirs:
	@echo $(DIRS)