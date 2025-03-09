/**
 * @file interrupts.cpp
 * @brief works with the Interrupt Descriptor Table (IDT) and its associated Interrupt Manager.
 *
 * The IDT is used by the CPU to handle interrupts and exceptions. This file includes the declaration of the
 * InterruptManager class, which is responsible for setting up the IDT, managing interrupt handlers, and
 * handling interrupt requests. The IDT entries are defined by the GateDescriptor structure, which specifies
 * the handler address, code segment selector, and access rights.
*/

#include "interrupts.h"


/**
 * @brief Default interrupt handler that ignores the interrupt request.
*/
InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager)
{
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager -> handlers[interruptNumber] = this;
}


/**
 * @brief Destructor for the InterruptHandler class.
*/
InterruptHandler::~InterruptHandler()
{
    if(interruptManager -> handlers[interruptNumber] == this)
        interruptManager -> handlers[interruptNumber] = 0;
}

/**
 * @brief Handles the interrupt request.
 *
 * This function is called when an interrupt occurs. It prints a message indicating that an interrupt
 * has been received and returns the stack pointer.
 *         *
 * @param esp The stack pointer at the time of the interrupt.
 * @return The stack pointer after handling the interrupt.
*/
uint32_t 
InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}



void
printf(char* str);



// Array to hold the Interrupt Descriptor Table (IDT) entries
InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

// Pointer to the active InterruptManager instance
InterruptManager* InterruptManager::ActiveInterruptManager = 0;



/**
 * @brief Sets an entry in the Interrupt Descriptor Table (IDT).
 *
 * This function configures an IDT entry for a specific interrupt number. It sets the handler address,
 * code segment selector, and access rights for the interrupt.
 *
 * @param interruptNumber The interrupt number to set the descriptor for.
 * @param codeSegmentSelectorOffset The code segment selector offset in the GDT.
 * @param handler The function pointer to the interrupt handler.
 * @param DescriptorPrivilegeLevel The privilege level required to access the interrupt (0-3).
 * @param DescriptorType The type of the descriptor (e.g., interrupt gate, trap gate).
*/
void
InterruptManager::SetInterruptDescriptorTableEntry(
            uint8_t interruptNumber,
            uint16_t codeSegmentSelectorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;
 
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}



/**
 * @brief Constructor for the InterruptManager class.
 *
 * Initializes the Interrupt Descriptor Table (IDT) and configures the Programmable Interrupt Controllers (PICs).
 * The IDT is initialized with default handlers for all interrupts, and specific handlers are set for certain
 * interrupts (e.g., timer interrupt).
 *
 * @param gdt A pointer to the Global Descriptor Table (GDT) used to get the code segment selector.
*/
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
:   picMasterCommand(0x20),
    picSlaveCommand(0xA0),
    picMasterData(0x21),
    picSlaveData(0xA1)
{
    uint16_t codeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
 
    // Initialize all IDT entries with a default handler
    for (uint16_t i = 0; i < 256; i++)
    {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
 
    // Set specific handlers for certain interrupts
    SetInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
 
    // Configure the PICs (Programmable Interrupt Controllers)
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);
 
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);
 
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);
     
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);
 
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
 
    // Load the IDT into the CPU
    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
}



/**
 * @brief Destructor for the InterruptManager class.
 *
 * Currently, this destructor does not perform any specific actions.
*/
InterruptManager::~InterruptManager()
{

}



/**
 * @brief Activates the interrupt handling mechanism.
 *
 * Enables interrupts by setting the interrupt flag (IF) in the CPU's flag register.
 * If another InterruptManager instance is active, it is deactivated before activating this one.
*/
void 
InterruptManager::Activate()
{
    if (ActiveInterruptManager != 0)
    {
        ActiveInterruptManager->Deactivate();
    }
    ActiveInterruptManager = this;
    asm("sti");
}



/**
 * @brief Deactivates the interrupt handling mechanism.
 *
 * Disables interrupts by clearing the interrupt flag (IF) in the CPU's flag register.
*/
void 
InterruptManager::Deactivate()
{
    if (ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}



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
uint32_t 
InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if (ActiveInterruptManager != 0) 
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);   

    return esp;
}



/**
 * @brief DoHandleInterrupt
 * 
 * Default interrupt handler that ignores the interrupt request.
*/
uint32_t 
InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if (handlers[interruptNumber] != 0)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if (0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        picMasterCommand.Write(0x20);
        if (0x28 <= interruptNumber)
            picSlaveCommand.Write(0x20);
    }
    else if (interruptNumber != 0x20)
    {
        char* foo = "UNHANDLED INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";
        foo[22] = hex[(interruptNumber >> 4) & 0x0F];
        foo[23] = hex[interruptNumber & 0x0F];

        printf(foo);
    }


    if (0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        picMasterCommand.Write(0x20);
        if (0x28 <= interruptNumber)
            picSlaveCommand.Write(0x20);
    }
    
    return esp;
}