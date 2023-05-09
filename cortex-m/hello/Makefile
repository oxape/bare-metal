ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE=arm-none-eabi-
endif

# Base address to load the program
RAM_BASE=0x00100010
LINKER_SCRIPTS="link.ld"

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump 

.PHONY: clean all dump debug qemu qemu-gdb


# -O0          : Disable optimizations
# -g           : Generate debug info
# -ffixed-r8   : Don't touch register r8
CFLAGS += -g -O0
CFLAGS += -march=armv8-a -mcpu=cortex-a7
CFLAGS += -fno-builtin -ffreestanding
LDFLAGS += -nostdlib
# LDFLAGS += -L/Users/starot/bin/gcc-arm-none-eabi-10.3-2021.10/lib/gcc/arm-none-eabi/10.3.1/ -lgcc 

# Name of target program
PROG=hello

# List of all *c sources
CSRC=test.c

#List of all *S (asm) sources
ASRC=start.S

COBJ = $(subst .c,.o,$(CSRC))
AOBJ = $(subst .S,.o,$(ASRC))

all: $(PROG).bin

$(AOBJ): %.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(COBJ): %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROG).elf: $(COBJ) $(AOBJ)
	$(LD) -T $(LINKER_SCRIPTS) $^ -o $@

$(PROG).bin: $(PROG).elf
	$(OBJCOPY) -v -O binary $^ $@

clean:
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
disassemble: $(PROG).elf
	$(OBJDUMP) -d $(PROG).elf

# Connect to the remote gdb and start debugging
debug: $(PROG).bin
	rm .gdbscript ; \
	echo 'target remote 10.7.9.19:4000' >> .gdbscript ; \
	echo 'restore $(PROG).bin binary $(RAM_BASE)' >> .gdbscript ; \
	echo 'jump *$(RAM_BASE)' >> .gdbscript ; \
	$(GDB) -x .gdbscript $(PROG).elf ;

# run in qemu

QEMU = qemu-system-arm

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)
ifndef CPUS
CPUS := 1
endif

QEMUOPTS = -M virt -cpu cortex-a7 -m 2G -nographic -smp $(CPUS)


qemu: all
	$(QEMU) $(QEMUOPTS) -kernel $(PROG).bin

.gdbinit: .gdbinit.tmpl
	sed -e "s/:1234/:$(GDBPORT)/" -e "s/xxx.elf/$(PROG).elf/" < $^ > $@

qemu-gdb: all .gdbinit
	@echo "*** Now run 'gdb' in another window." 1>&2
	$(QEMU) $(QEMUOPTS) -kernel $(PROG).bin -S $(QEMUGDB)