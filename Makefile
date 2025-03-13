# We are building a 32-bit OS
GPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fno-stack-protector -Wno-multichar
ASPARAMS = --32
LDPARAMS = -melf_i386
QEMU = qemu-system-i386

# Object files
objects = 	obj/loader.o \
		obj/gdt.o \
		obj/drivers/driver.o \
		obj/hardwarecommunication/port.o \
		obj/hardwarecommunication/interrupts.o \
		obj/hardwarecommunication/interruptstubs.o \
		obj/drivers/keyboard.o \
		obj/drivers/mouse.o \
		obj/kernel.o 

# Compile C++ files
obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPARAMS) -o $@ -c $<

# Compile Assembly files
obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

# Link the kernel
mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

# Create a bootable ISO image
mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' 			 > iso/boot/grub/grub.cfg
	echo 'set default=0' 			>> iso/boot/grub/grub.cfg
	echo 'menuentry "TOMAT-OS" {' 		>> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' 	>> iso/boot/grub/grub.cfg
	echo '	boot' 				>> iso/boot/grub/grub.cfg
	echo '}' 			        >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

# Install to /boot (requires sudo)
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

# Run in QEMU
run: mykernel.bin
	$(QEMU) -kernel $<

# Run the ISO image in QEMU
run-iso: mykernel.iso
	$(QEMU) -cdrom $<

# Run the ISO image in VirtualBox
run-iso-vbox: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "TOMAT-OS" &

# Clean build files and directories
clean:
	rm -rf obj mykernel.bin mykernel.iso

# Phony targets
.PHONY: clean install run

