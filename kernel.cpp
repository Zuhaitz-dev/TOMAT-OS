
void printf(char* str)
{
    // Each letter is separated in two bytes
    //
    //          0               1               2
    //  |       |       |       |       |
    //  | Color | Char  |  Col  |  Char |   ...
    //  |       |       |       |       |
    //
    // For now we are not touching the colors, so it's 0xFF00
    // Being the first four bits the color of the chars
    // And the last four bits the background color

    unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int i{0}; str[i] != '\0'; i++) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

// We add extern "C" so the fuction name isn't modified and doesn't cause problems 
extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) 
{
    printf("Hello World! --- https://github.com/Zuhaitz-dev");      // Will not work without the printf function we made. 
                                                                    // Libraries won't work, for now, so we have to make everything.

    while(1);

}

