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
     // For now, we are not modifying the color attributes, so we use 0xFF00.
     // The first four bits represent the foreground color of the characters,
     // and the last four bits represent the background color.
 
     static uint16_t* VideoMemory = (uint16_t*)0xb8000;
 
     static uint8_t x{0}, y{0};
 
     for (int i{0}; str[i] != '\0'; i++)
     {
         switch (str[i])
         {
             case '\n':
                 x = 0;
                 y++;
                 break;
             default:
                 VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                 x++;
                 break;
         }
 
         // New line
         if (x >= 80)
         {
             x = 0;
             y++;
         }
 
         // Scroll
         if (y >= 25)
         {
             for (y = 0; y < 25; y++)
             {
                 for (x = 0; x < 80; x++)
                 {
                     VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                 }
             }
             x = 0;
             y = 0;
         }
     }
 
     // Previous code (does not work well because if we call printf again,
     // it will overwrite the previous text):
     //
     //  uint16_t* VideoMemory = (uint16_t*)0xb8000;
     // for (int i{0}; str[i] != '\0'; i++) {
     //     VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
     // }
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
 extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) 
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
            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n");
 
     GlobalDescriptorTable gdt;
 
     while (1);
 
 }