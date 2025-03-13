#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;



// Helper function to scroll the screen up by one line.
static void
ScrollUp()
{
    uint16_t* VideoMemory = (uint16_t*)0xB8000;
    const int width = 80;
    const int height = 25;
    // Move each row up.
    for (int row = 1; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            VideoMemory[(row - 1) * width + col] = VideoMemory[row * width + col];
        }
    }
    // Clear the last row.
    for (int col = 0; col < width; col++)
    {
        VideoMemory[(height - 1) * width + col] = (0x0F << 8) | ' ';
    }
}



// An improved printf function.
void
printf(char* str)
{
    // Each character is represented by two bytes in video memory:
    // 
    //          0               1               2
    //  |       |       |       |       |
    //  | Color | Char  | Color | Char  |   ...
    //  |       |       |       |       |
    //
    // The higher byte represents the color attributes (foreground and background),
    // and the lower byte represents the ASCII value of the character.

    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;
    const int width = 80;
    const int height = 25;
    // Choose a color attribute: foreground white (0x0F) on black background.
    uint8_t color = 0x0F;

    for (int i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];
        if (c == '\n')
        {
            x = 0;
            y++;
        }
        else if (c == '\b')
        {
            // Handle backspace: move back one column, clear the character.
            if (x > 0)
                x--;
            else if (y > 0)
            {
                y--;
                x = width - 1;
            }
            VideoMemory[y * width + x] = (color << 8) | ' ';
        }
        else
        {
            VideoMemory[y * width + x] = (color << 8) | c;
            x++;
        }

        // Auto-wrap to next line if at the end.
        if (x >= width)
        {
            x = 0;
            y++;
        }

        // Scroll if reached the end of the screen.
        if (y >= height)
        {
            ScrollUp();
            y = height - 1;
        }
    }
}



void
printfHex(uint8_t key)
{
    char foo[3] = { 0 };  // extra byte for '\0'
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    foo[2] = '\0';
    printf(foo);
}



//----------Class----------//



class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};



//----------Class----------//



class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    
    
    virtual void
    OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};





typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void
callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void
kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World! --- https://github.com/Zuhaitz-dev\n");    // Will not work without the printf function we made. 
                                                                    // Libraries won't work, for now, so we have to make everything.
                                                                    
    GlobalDescriptorTable gdt;
    // InterruptManager interrupts(&gdt);
    InterruptManager interrupts(0x20, &gdt);

    printf("\nInitializing Hardware, Stage 1...\n");
     
    DriverManager drvManager;
    
        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDriver(&keyboard);

        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        drvManager.AddDriver(&mouse);
      
    printf("\nInitializing Hardware, Stage 2...\n");
        drvManager.ActivateAll();
         
    printf("\nInitializing Hardware, Stage 3...\n");
    interrupts.Activate();

    while (1);
}
