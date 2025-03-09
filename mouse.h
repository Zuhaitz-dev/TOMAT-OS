// TODO: header and review, pretty much.

#ifndef __MOUSE_H
#define __MOUSE_H

#include "port.h"
#include "interrupts.h"
#include "types.h"

class MouseDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

 public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};


#endif