# We are building a 32-bit OS
GPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386
QEMU = qemu-system-i386

objects = loader.o kernel.o

# Compile C++ files
%.o: %.cpp
	g++ $(GPPARAMS) -o $@ -c $<

# Compile Assembly files
%.o: %.s
	as $(ASPARAMS) -o $@ $<

# Link the kernel
mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

# Install to /boot (requires sudo)
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

# Run in QEMU
run: mykernel.bin
	$(QEMU) -kernel mykernel.bin

# Clean build files
clean:
	rm -f $(objects) mykernel.bin
