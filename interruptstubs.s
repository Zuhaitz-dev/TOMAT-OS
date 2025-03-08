/**
 * @file interruptstubs.s
 * @brief This file contains the assembly-level interrupt stubs for handling exceptions and hardware interrupts.
 *
 * The stubs are responsible for saving the CPU state, calling the appropriate interrupt handler in the
 * InterruptManager class, and restoring the CPU state after the interrupt is handled. This file also defines
 * macros for generating exception and interrupt handlers dynamically.
 */

.set IRQ_BASE, 0x20  #< Base offset for hardware interrupts (IRQs) in the IDT.

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj  #< External reference to the C++ interrupt handler.

.global _ZN16InterruptManager22IgnoreInterruptRequestEv  #< Global symbol for the default interrupt handler.

/**
 * @macro HandleException
 * @brief Generates an exception handler stub for a specific exception number.
 *
 * This macro defines a function that handles a specific exception by saving the exception number and
 * jumping to the common interrupt handling code.
 *
 * @param num The exception number.
 */
.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptnumber)   #< Store the exception number in the `interruptnumber` variable.
    jmp int_bottom                  #< Jump to the common interrupt handling code.
.endm

/**
 * @macro HandleInterruptRequest
 * @brief Generates an interrupt handler stub for a specific IRQ number.
 *
 * This macro defines a function that handles a specific hardware interrupt by saving the IRQ number
 * (adjusted by IRQ_BASE) and jumping to the common interrupt handling code.
 *
 * @param num The IRQ number.
 */
.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)    #< Store the IRQ number in the `interruptnumber` variable.
    jmp int_bottom                              #< Jump to the common interrupt handling code.
.endm

// Define handlers for specific interrupts
HandleInterruptRequest 0x00  #< Handler for IRQ 0x00 (e.g., timer interrupt).
HandleInterruptRequest 0x01  #< Handler for IRQ 0x01 (e.g., keyboard interrupt).

/**
 * @brief Common interrupt handling code.
 *
 * This section saves the CPU state, calls the C++ interrupt handler, and restores the CPU state.
 */
int_bottom:

    # Save all general-purpose registers.
    pusha

    # Save segment registers.
    pushl       %ds
    pushl       %es
    pushl       %fs
    pushl       %gs

    # Pass the stack pointer and interrupt number to the C++ handler.
    pushl       %esp
    push        (interruptnumber)
    call        _ZN16InterruptManager15handleInterruptEhj

    # Restore the stack pointer returned by the handler.
    movl        %eax, %esp

    # Restore segment registers.
    popl        %gs
    popl        %fs
    popl        %es
    popl        %ds

    # Restore all general-purpose registers.
    popa

/**
 * @brief Default interrupt handler that ignores the interrupt request.
 *
 * This function simply returns from the interrupt without performing any action.
 */
_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret  #< Return from interrupt.

.section .data
    interruptnumber:    .byte 0  #< Variable to store the interrupt or exception number.