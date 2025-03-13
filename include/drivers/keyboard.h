#ifndef __TOMATOS__DRIVERS__KEYBOARD_H
#define __TOMATOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace TomatOS
{
    namespace drivers
    {
    
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };
        
        class KeyboardDriver : public TomatOS::hardwarecommunication::InterruptHandler, public Driver
        {
            TomatOS::hardwarecommunication::Port8Bit dataport;
            TomatOS::hardwarecommunication::Port8Bit commandport;
            
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(TomatOS::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual TomatOS::common::uint32_t HandleInterrupt(TomatOS::common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    
#endif