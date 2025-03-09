/**
 * @file kernel.cpp
 * @brief This source file contains the main entry point for the TOMAT-OS kernel.
 *
 * This file provides the implementation of the kernelMain function, which is the main entry point
 * for the TOMAT-OS kernel. It includes a simple printf function for displaying text on the screen,
 * and a callConstructors function for calling global constructors.
*/

#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

// Helper function to scroll the screen up by one line.
static void ScrollUp()
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
void printf(char* str)
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

typedef void(*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    // The tutorial uses end_ctors instead of &end_ctors. It does not work
    // like that, but let's not forget about that in the future for now.
    for (constructor* i = &start_ctors; i != &end_ctors; i++) 
    {
        (*i)();
    }
}

// We add extern "C" so the fuction name isn't modified and doesn't cause problems 
extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/) 
{
    printf("Hello World! --- https://github.com/Zuhaitz-dev\n");    // Will not work without the printf function we made. 
                                                                    // Libraries won't work, for now, so we have to make everything.

    // Trying a longer text to test the new features.

    printf("\nLorem ipsum dolor sit amet, "
           "consectetur adipiscing elit, "
           "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
           "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
           "nisi ut aliquip ex ea commodo consequat. "
           "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
           "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);
    //InterruptManager interrupts(0x20, &gdt);

    KeyboardDriver keyboard(&interrupts);
    MouseDriver mouse(&interrupts);


    interrupts.Activate();

    while (1);

}
