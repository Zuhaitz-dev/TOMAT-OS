# We are building a 32-bit OS
GPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fno-stack-protector -Wno-multichar
ASPARAMS = --32
LDPARAMS = -melf_i386

# GRAPHICSFLAG is empty by default (non-graphics build).
GRAPHICSFLAG = 

# Object files
objects =	obj/loader.o \
        	obj/gdt.o \
			obj/memorymanagement.o \
        	obj/drivers/driver.o \
        	obj/hardwarecommunication/port.o \
        	obj/hardwarecommunication/interrupts.o \
        	obj/hardwarecommunication/interruptstubs.o \
			obj/multitasking.o \
        	obj/hardwarecommunication/pci.o \
			obj/drivers/keyboard.o \
        	obj/drivers/mouse.o \
        	obj/drivers/vga.o \
			obj/gui/widget.o \
			obj/gui/window.o \
			obj/gui/desktop.o \
			obj/gui/render.o \
        	obj/kernel.o 

# Compile C++ files with additional flags (GRAPHICSFLAG)
obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPARAMS) $(GRAPHICSFLAG) -o $@ -c $<

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
	echo 'set timeout=0'               > iso/boot/grub/grub.cfg
	echo 'set default=0'              >> iso/boot/grub/grub.cfg
	echo 'menuentry "TOMAT-OS" {'      >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot'                   >> iso/boot/grub/grub.cfg
	echo '}'                        >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

# Install to /boot (requires sudo)
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

# Extra target for non-graphics mode (text mode)
run-text: GRAPHICSFLAG =
run-text: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "TOMAT-OS" &

# Extra target for graphics mode
run-gfx: GRAPHICSFLAG = -DGRAPHICSMODE
run-gfx: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "TOMAT-OS" &

# TOCHECK: Keyboard drivers aren't working correctly in this mode.
# QEMU target for non-graphics (text) mode
run-qemu-text: GRAPHICSFLAG =
run-qemu-text: mykernel.iso
	qemu-system-i386 -boot d -cdrom mykernel.iso -m 512 -vga std

# QEMU target for graphics mode
run-qemu-gfx: GRAPHICSFLAG = -DGRAPHICSMODE
run-qemu-gfx: mykernel.iso
	qemu-system-i386 -boot d -cdrom mykernel.iso -m 512 -vga std

# Clean build files and directories
clean:
	rm -rf obj mykernel.bin mykernel.iso

# Phony targets
.PHONY: clean install run run-text run-gfx

