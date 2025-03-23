
#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace myos
{
    namespace drivers
    {
    
        class MouseEventHandler
        {
         public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(common::uint8_t button);
            virtual void OnMouseUp(common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };
        
        
        class MouseDriver : public hardwarecommunication::InterruptHandler, public Driver
        {
            hardwarecommunication::Port8Bit dataport;
            hardwarecommunication::Port8Bit commandport;
            common::uint8_t buffer[3];
            common::uint8_t offset;
            common::uint8_t buttons;

            MouseEventHandler* handler;
         public:
            MouseDriver(hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual common::uint32_t HandleInterrupt(common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    
#endif
