/**
 * @file linker.ld
 * @brief This linker script defines the memory layout for the TOMAT-OS kernel.
 *
 * This file provides the linker script for the TOMAT-OS kernel, specifying the entry point,
 * output format, and memory sections. It ensures that the kernel is correctly linked and
 * loaded into memory, with sections for text, data, and bss.
 */

/* As we are working with assembly lang and C++, we need this */

ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 0x0100000;

    .text :
    {
        *(.multiboot)
        *(.text*)
        *(.rodata)
    }

    .data :
    {
        start_ctors = .;
        KEEP(*( .init_array ));
        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array* )));
        end_ctors = .;

        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.fini_array*)
        *(.comment)
    }
}