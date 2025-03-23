#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <gui/render.h>     // Thank you, sloganking
#include <multitasking.h>

// #define GRAPHICSMODE    // Comment or define this flag as needed

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;



// Interesting wiki: https://www.lowlevel.eu/wiki/Hauptseite
// If you don't speak German, this one is great too: https://wiki.osdev.org/Expanded_Main_Page


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
        char foo[2] = {c, '\0'};
        printf(foo);
    }
};



//----------Class----------//

// TOCHECK: When booting, mouse is working mostly well, but it still acts like it's also in the center position.

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
 public:
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                | (VideoMemory[80 * y + x] & 0x00FF);
    }
    

    
    virtual void
    OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        // Erase previous cursor
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                | (VideoMemory[80 * y + x] & 0x00FF);
        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;
        // Draw new cursor
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4
                                | (VideoMemory[80 * y + x] & 0xF000) >> 4
                                | (VideoMemory[80 * y + x] & 0x00FF);
    }
};



    // For testing multitasking. 
    // It works, but it is commented to be able to use the graphics mode
    /*
    void
    TaskA()
    {
        while (true)
            printf("A");
    }



    void
    TaskB()
    {
        while(true)
            printf("B");
    }
    */


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
    printf("Hello World! --- https://github.com/Zuhaitz-dev\n");

    GlobalDescriptorTable gdt;
    
    // Uncomment to test multitasking 
    // (in text mode, as for now TaskA and TaskB aren't... great)
    
    TaskManager taskManager;

    /*
    Task task1(&gdt, TaskA);
    Task task2(&gdt, TaskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */

    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    printf("\nInitializing Hardware, Stage 1...\n");
    
    DriverManager drvManager;
    
#if defined(GRAPHICSMODE)
    // Initialize graphics mode objects.
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
    KeyboardDriver keyboard(&interrupts, &desktop);
    drvManager.AddDriver(&keyboard);
    MouseDriver mouse(&interrupts, &desktop);
    drvManager.AddDriver(&mouse);
#else
    // Use basic console input handlers.
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);
#endif

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

#if defined(GRAPHICSMODE)
    VideoGraphicsArray vga;
    Render rend(320, 200); // Hardcoded for now
#endif

    printf("\nInitializing Hardware, Stage 2...\n");
    
    drvManager.ActivateAll();
    
    printf("\nInitializing Hardware, Stage 3...\n");

#if defined(GRAPHICSMODE)
    vga.SetMode(320,200,8);
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);
    Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);
#endif

    interrupts.Activate();

#if defined(GRAPHICSMODE)
    while (1)
    {
        // Render graphics mode.
        desktop.Draw(&rend);
        rend.display(&vga);
    }
#else
    while (1)
    {
        // In text mode, we could halt or add other non-graphical tasks.
    }
#endif
}
