# TOMAT-OS

## Overview

TOMAT-OS is a simple 32-bit operating system designed for educational purposes. It demonstrates the basic concepts of operating system development, including bootloading, memory management, and hardware communication.

## Project Structure

The project is organized into several key files and directories:

- `loader.s`: Assembly file containing the bootloader code.
- `kernel.cpp`: Main entry point for the TOMAT-OS kernel.
- `gdt.h` and `gdt.cpp`: Header and source files for the Global Descriptor Table (GDT).
- `port.h` and `port.cpp`: Header and source files for interacting with I/O ports.
- `types.h`: Header file defining fixed-width integer types.
- `linker.ld`: Linker script defining the memory layout for the kernel.
- `Makefile`: Build script for compiling and linking the kernel.

## Files

### `loader.s`

```plaintext
/**
 * @file loader.s
 * @brief This assembly file contains the bootloader code for TOMAT-OS.
 *
 * This file provides the implementation of the bootloader, which sets up the initial
 * stack and calls the kernel's main function. It also includes the Multiboot header
 * required for booting with a Multiboot-compliant bootloader.
 */
 ```

The `loader.s` file contains the bootloader code written in assembly language. It sets up the initial stack and calls the kernel's main function. It also includes the Multiboot header required for booting with a Multiboot-compliant bootloader.

### `kernel.cpp`

 ```c
 /**
 * @file kernel.cpp
 * @brief This source file contains the main entry point for the TOMAT-OS kernel.
 *
 * This file provides the implementation of the kernelMain function, which is the main entry point
 * for the TOMAT-OS kernel. It includes a simple printf function for displaying text on the screen,
 * and a callConstructors function for calling global constructors.
 */
 ```

The `kernel.cpp` file contains the main entry point for the TOMAT-OS kernel. It includes the kernelMain function, a simple printf function for displaying text on the screen, and a callConstructors function for calling global constructors.

### `gdt.h`

 ```c
/**
 * @file gdt.h
 * @brief This header file defines the Global Descriptor Table (GDT) and its associated Segment Descriptors.
 *
 * The GDT is used by the CPU to define the characteristics of the various memory segments used in the system,
 * such as base address, size, and access privileges. This file includes the declaration of the GlobalDescriptorTable
 * class and its nested SegmentDescriptor class, which are used to manage and access the GDT entries.
 */
 ```

The `gdt.h` file defines the Global Descriptor Table (GDT) and its associated Segment Descriptors. The GDT is used by the CPU to define the characteristics of the various memory segments used in the system, such as base address, size, and access privileges.

### `gdt.cpp`

 ```c
/**
 * @file gdt.cpp
 * @brief This source file implements the Global Descriptor Table (GDT) and its associated Segment Descriptors.
 *
 * This file provides the implementation of the GlobalDescriptorTable class and its nested SegmentDescriptor class.
 * It includes the constructor and destructor for the GlobalDescriptorTable, as well as methods for accessing
 * the base and limit of the Segment Descriptors.
 */
 ```

The `gdt.cpp` file implements the Global Descriptor Table (GDT) and its associated Segment Descriptors. It includes the constructor and destructor for the GlobalDescriptorTable class, as well as methods for accessing the base and limit of the Segment Descriptors.

### `port.h`

 ```c
/**
 * @file port.h
 * @brief This header file defines classes for interacting with I/O ports.
 *
 * This file provides the declaration of the Port class and its derived classes
 * for 8-bit, 16-bit, and 32-bit I/O port operations. These classes provide methods
 * for reading from and writing to I/O ports, which are essential for hardware communication.
 */
 ```

The `port.h` file defines classes for interacting with I/O ports. It provides the declaration of the Port class and its derived classes for 8-bit, 16-bit, and 32-bit I/O port operations. These classes provide methods for reading from and writing to I/O ports, which are essential for hardware communication.

### `port.cpp`

 ```c
/**
 * @file port.cpp
 * @brief This source file implements classes for interacting with I/O ports.
 *
 * This file provides the implementation of the Port class and its derived classes
 * for 8-bit, 16-bit, and 32-bit I/O port operations. These classes provide methods
 * for reading from and writing to I/O ports, which are essential for hardware communication.
 */
 ```

The `port.cpp` file implements classes for interacting with I/O ports. It provides the implementation of the Port class and its derived classes for 8-bit, 16-bit, and 32-bit I/O port operations. These classes provide methods for reading from and writing to I/O ports, which are essential for hardware communication.

### `types.h`

 ```c
/**
 * @file types.h
 * @brief This header file defines fixed-width integer types.
 *
 * This file provides typedefs for integer types with specific widths.
 * These typedefs ensure that the integers have the same size on any platform,
 * which is crucial for portability and consistency in systems programming.
 */
 ```

The `types.h` file defines fixed-width integer types. It provides typedefs for integer types with specific widths, ensuring that the integers have the same size on any platform, which is crucial for portability and consistency in systems programming.

### `linker.ld`

 ```plaintext
/**
 * @file linker.ld
 * @brief This linker script defines the memory layout for the TOMAT-OS kernel.
 *
 * This file provides the linker script for the TOMAT-OS kernel, specifying the entry point,
 * output format, and memory sections. It ensures that the kernel is correctly linked and
 * loaded into memory, with sections for text, data, and bss.
 */
 ```

The `linker.ld` file defines the memory layout for the TOMAT-OS kernel. It specifies the entry point, output format, and memory sections, ensuring that the kernel is correctly linked and loaded into memory.

## Building and Running

To build and run TOMAT-OS, you will need a cross-compiler for i386 and QEMU for emulation. Follow these steps:

1. Build the kernel:

```bash
make
```

2. Run the kernel in QEMU:

```bash
make run
```

3. Create a bootable ISO image:

```bash
make mykernel.iso
```

4. Run the ISO image in QEMU:

```bash
make run-iso
```

5. Clean build files:

```bash
make clean
```

6. If you desire to run it on VirtualBox, use:

```bash
make run-iso-vbox
```