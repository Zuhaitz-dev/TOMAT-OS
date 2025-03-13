#ifndef __TOMATOS__DRIVERS__MOUSE_H
#define __TOMATOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace TomatOS
{
    namespace drivers
    {
    
        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(TomatOS::common::uint8_t button);
            virtual void OnMouseUp(TomatOS::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };
        
        
        class MouseDriver : public TomatOS::hardwarecommunication::InterruptHandler, public Driver
        {
            TomatOS::hardwarecommunication::Port8Bit dataport;
            TomatOS::hardwarecommunication::Port8Bit commandport;
            TomatOS::common::uint8_t buffer[3];
            TomatOS::common::uint8_t offset;
            TomatOS::common::uint8_t buttons;

            MouseEventHandler* handler;
        public:
            MouseDriver(TomatOS::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual TomatOS::common::uint32_t HandleInterrupt(TomatOS::common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    
#endif
