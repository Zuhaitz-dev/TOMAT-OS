/**
 * @file interrupts.h
 * @brief This header file defines the Interrupt Descriptor Table (IDT) and its associated Interrupt Manager.
 *
 * The IDT is used by the CPU to handle interrupts and exceptions. This file includes the declaration of the
 * InterruptManager class, which is responsible for setting up the IDT, managing interrupt handlers, and
 * handling interrupt requests. The IDT entries are defined by the GateDescriptor structure, which specifies
 * the handler address, code segment selector, and access rights.
*/

#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H
 
#include "types.h"
#include "port.h"
#include "gdt.h"
 
class InterruptManager;

/**
 * @class InterruptHandler
 * @brief Base class for interrupt handlers.
 * 
 * The InterruptHandler class is a base class for interrupt handlers. It does not provide any specific
 * functionality, but can be used as a base class for specific interrupt handlers.
*/
class InterruptHandler {
    protected:
        uint8_t interruptNumber; ///< The number of the interrupt that the handler is associated with.
        InterruptManager* interruptManager; ///< Pointer to the InterruptManager instance.
    
        /**
        * @brief Constructor for the InterruptHandler class.
        *
        * Initializes the interrupt number and the associated InterruptManager instance.
        *
        * @param interruptNumber The number of the interrupt that the handler is associated with.
        * @param interruptManager A pointer to the InterruptManager instance.
        */
        InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);

        ~InterruptHandler();
    
    public:
        /**
        * @brief Handles the interrupt request.
        *
        * This function is called when an interrupt occurs. It prints a message indicating that an interrupt
        * has been received and returns the stack pointer.
        *         *
        * @param esp The stack pointer at the time of the interrupt.
        * @return The stack pointer after handling the interrupt.
        */
        virtual uint32_t HandleInterrupt(uint32_t esp);
};

/**
 * @class InterruptManager
 * @brief Manages the Interrupt Descriptor Table (IDT) and handles interrupt requests.
 *
 * The InterruptManager class is responsible for initializing the IDT, setting up interrupt handlers,
 * and managing the Programmable Interrupt Controllers (PICs). It provides methods to activate interrupts
 * and handle specific interrupt requests.
*/

class InterruptManager
{
friend class InterruptHandler;

 protected:

    static InterruptManager* ActiveInterruptManager; ///< Pointer to the active InterruptManager instance.
    InterruptHandler *handlers[256]; ///< Array to hold the interrupt handlers.


    /**
     * @struct GateDescriptor
     * @brief Describes an entry in the Interrupt Descriptor Table (IDT).
     *
     * Each entry in the IDT is represented by a GateDescriptor, which contains the address of the interrupt
     * handler, the code segment selector, and access rights.
    */
    struct GateDescriptor
    {
        uint16_t handlerAddressLowBits;  ///< Lower 16 bits of the handler address.
        uint16_t gdt_codeSegmentSelector; ///< Code segment selector in the GDT.
        uint8_t reserved;                ///< Reserved field (must be 0).
        uint8_t access;                  ///< Access rights and flags (e.g., present bit, privilege level).
        uint16_t handlerAddressHighBits; ///< Upper 16 bits of the handler address.
    } __attribute__((packed));
 
    static GateDescriptor interruptDescriptorTable[256]; ///< Array to hold the IDT entries.
 
    /**
     * @brief Sets an entry in the Interrupt Descriptor Table (IDT).
     *
     * @param interruptNumber The interrupt number to set the descriptor for.
     * @param codeSegmentSelectorOffset The code segment selector offset in the GDT.
     * @param handler The function pointer to the interrupt handler.
     * @param DescriptorPrivilegeLevel The privilege level required to access the interrupt (0-3).
     * @param DescriptorType The type of the descriptor (e.g., interrupt gate, trap gate).
    */
    static void SetInterruptDescriptorTableEntry(
        uint8_t interruptNumber,
        uint16_t codeSegmentSelectorOffset,
        void (*handler)(),
        uint8_t DescriptorPrivilegeLevel,
        uint8_t DescriptorType
    );
 
    /**
     * @struct InterruptDescriptorTablePointer
     * @brief Describes the pointer to the Interrupt Descriptor Table (IDT).
     *
     * This structure is used to load the IDT into the CPU using the `lidt` instruction.
    */
    struct InterruptDescriptorTablePointer
    {
        uint16_t size; ///< Size of the IDT in bytes minus 1.
        uint32_t base; ///< Base address of the IDT.
    } __attribute__((packed));
 
    // Programmable Interrupt Controllers (PICs) for managing hardware interrupts.
    Port8BitSlow picMasterCommand; ///< Command port for the master PIC.
    Port8BitSlow picMasterData;    ///< Data port for the master PIC.
    Port8BitSlow picSlaveCommand;  ///< Command port for the slave PIC.
    Port8BitSlow picSlaveData;     ///< Data port for the slave PIC.
 
 public:
    /**
     * @brief Constructor for the InterruptManager class.
     *
     * Initializes the IDT and configures the PICs.
     *
     * @param gdt A pointer to the Global Descriptor Table (GDT) used to get the code segment selector.
    */
    InterruptManager(GlobalDescriptorTable* gdt);
 
    /**
     * @brief Destructor for the InterruptManager class.
     *
     * Currently, this destructor does not perform any specific actions.
    */
    ~InterruptManager();
 
    /**
    * @brief Activates the interrupt handling mechanism.
    *
    * Enables interrupts by setting the interrupt flag (IF) in the CPU's flag register.
    */
    void Activate();

    /**
    * @brief Deactivates the interrupt handling mechanism.
    *
    * Disables interrupts by clearing the interrupt flag (IF) in the CPU's flag register.
    */
    void Deactivate();

    /**
     * @brief Handles an interrupt request.
     *
     * This function is called when an interrupt occurs. It prints a message indicating that an interrupt
     * has been received and returns the stack pointer.
     *
     * @param interruptNumber The number of the interrupt that occurred.
     * @param esp The stack pointer at the time of the interrupt.
     * @return The stack pointer after handling the interrupt.
    */
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
 
    /**
     * @brief Default interrupt handler that ignores the interrupt request.
    */
    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);

    /**
     * @brief Default interrupt handler that ignores the interrupt request.
    */
    static void IgnoreInterruptRequest();
 
    /**
     * @brief Handles interrupt request 0x00 (e.g., timer interrupt).
    */
    static void HandleInterruptRequest0x00();
 
    /**
     * @brief Handles interrupt request 0x01 (e.g., keyboard interrupt).
    */
    static void HandleInterruptRequest0x01();

    /**
     * @brief Handles interrupt request 0x0C (e.g., mouse interrupt).
    */
    static void HandleInterruptRequest0x0C();
 };
 
 #endif