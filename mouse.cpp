// TODO: header and review, pretty much.

#include "mouse.h"

MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager),
dataport(0x60),
commandport(0x64)
{
    // Possible states: 0 (no buttons pressed), 1 (left button), 2 (right button), 4 (middle button)
    offset = 0;
    buttons = 0;

    uint16_t* VideoMemory = (uint16_t*)0xB8000;
    // (40, 12) is the center of the screen
    VideoMemory[80* 12 + 40] = (VideoMemory[80 * 12 + 40] & 0xF000) >> 4
    | (VideoMemory[80 * 12 + 40] & 0x0F00) << 4
    | (VideoMemory[80 * 12 + 40] & 0x00FF);

    commandport.Write(0xAB); // activate interrupts
    commandport.Write(0x20); // get current state
    uint8_t status = dataport.Read() | 2;
    commandport.Write(0x60); // set state
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}



MouseDriver::~MouseDriver()
{

}



void
printf(char* str);



uint32_t
MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    if(!(status & 0x20))
        return esp;

    static int8_t x = 40, y = 12; 


    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xB8000;

        VideoMemory[80* y + x] = (VideoMemory[80 * y + x] & 0xF000) >> 4
                                | (VideoMemory[80 * y + x] & 0x0F00) << 4
                                | (VideoMemory[80 * y + x] & 0x00FF);

        x += buffer[1];

        // Check if the mouse is out of bounds (X-axis).
        if (x < 0) x = 0;
        if (x >= 80) x = 79;

        y -= buffer[2];

        // Check if the mouse is out of bounds (Y-axis).
        if (y < 0) y = 0;
        if (y >= 25) y = 24;

        VideoMemory[80* y + x] = (VideoMemory[80 * y + x] & 0xF000) >> 4
                                | (VideoMemory[80 * y + x] & 0x0F00) << 4
                                | (VideoMemory[80 * y + x] & 0x00FF);


        for (uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i)))
            {
                // We revert the color of the mouse.
                VideoMemory[80* 25 + 2 + i] = (VideoMemory[80 * 25 + 2 + i] & 0xF000) >> 4
                                            | (VideoMemory[80 * 25 + 2 + i] & 0x0F00) << 4
                                            | (VideoMemory[80 * 25 + 2 + i] & 0x00FF);
            }
        }
    }

    return esp;
}