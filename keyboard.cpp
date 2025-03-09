// TODO: header and review, pretty much.

#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1)
    {
        dataport.Read();
    }
    commandport.Write(0xAE); // activate interrupts
    commandport.Write(0x20); // get current state
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // set state
    dataport.Write(status);

    dataport.Write(0xF4);
}



KeyboardDriver::~KeyboardDriver()
{

}



void
printf(char* str);



uint32_t
KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    // Track shift state
    static bool shift = false;
    uint8_t key = dataport.Read();


    if(key == 0x0E)
    {
        // Read the next byte for the extended code.
        uint8_t extended = dataport.Read();
        // Ignore extended keys release events.
        if(extended & 0x80)
            return esp;
        // Delete key
        if(extended == 0x0E)
        {
            // Simulate deletion by moving back, erasing the character, and moving back.
            printf("\b \b");
            return esp;
        }
        return esp;
    }


    // If key is a release event (bit 7 set) and it's not a shift release, ignore it.
    if(key & 0x80)
    {
        if(key == 0xAA || key == 0xB6)
        {
            shift = false;
        }
        return esp;
    }


        switch (key)
        {
            // Shift key press/release
            case 0x2A: // Left Shift pressed
            case 0x36: // Right Shift pressed
                shift = true;
                break;
            case 0xAA: // Left Shift released
            case 0xB6: // Right Shift released
                shift = false;
                break;

            // Letters
            case 0x1E:
                shift ? printf("A") : printf("a");
                break;
            case 0x30:
                shift ? printf("B") : printf("b");
                break;
            case 0x2E:
                shift ? printf("C") : printf("c");
                break;
            case 0x20:
                shift ? printf("D") : printf("d");
                break;
            case 0x12:
                shift ? printf("E") : printf("e");
                break;
            case 0x21:
                shift ? printf("F") : printf("f");
                break;
            case 0x22:
                shift ? printf("G") : printf("g");
                break;
            case 0x23:
                shift ? printf("H") : printf("h");
                break;
            case 0x17:
                shift ? printf("I") : printf("i");
                break;
            case 0x24:
                shift ? printf("J") : printf("j");
                break;
            case 0x25:
                shift ? printf("K") : printf("k");
                break;
            case 0x26:
                shift ? printf("L") : printf("l");
                break;
            case 0x32:
                shift ? printf("M") : printf("m");
                break;
            case 0x31:
                shift ? printf("N") : printf("n");
                break;
            case 0x27:
                shift ? printf("Ñ") : printf("ñ");
                break;
            case 0x18:
                shift ? printf("O") : printf("o");
                break;
            case 0x19:
                shift ? printf("P") : printf("p");
                break;
            case 0x10:
                shift ? printf("Q") : printf("q");
                break;
            case 0x13:
                shift ? printf("R") : printf("r");
                break;
            case 0x1F:
                shift ? printf("S") : printf("s");
                break;
            case 0x14:
                shift ? printf("T") : printf("t");
                break;
            case 0x16:
                shift ? printf("U") : printf("u");
                break;
            case 0x2F:
                shift ? printf("V") : printf("v");
                break;
            case 0x11:
                shift ? printf("W") : printf("w");
                break;
            case 0x2D:
                shift ? printf("X") : printf("x");
                break;
            case 0x15:
                shift ? printf("Y") : printf("y");
                break;
            case 0x2C:
                shift ? printf("Z") : printf("z");
                break;

            case 0x2B:
                shift ? printf("Ç") : printf("ç");
                break;

            // Space key
            case 0x39:
                printf(" ");
                break;

            // Enter key
            case 0x1C:
                printf("\n");
                break;

            // Punctuation

            case 0x0C: // '
                shift ? printf("?") : printf("'");
                break;

            case 0x0D:  // ¡
                shift ? printf("¿") : printf("¡");
                break;

            case 0x1B:  // +
                shift ? printf("+") : printf("*");
                break;
                

            case 0x33:  // ,
                shift ? printf(";") : printf(",");
                break;

            case 0x34:  // .
                shift ? printf(":") : printf(".");
                break;

            case 0x35:  // -
                shift ? printf("_") : printf("-");
                break;

            // Numbers and symbols
            case 0x0B: // 0
                shift ? printf("=") : printf("0");
                break;
            case 0x02: // 1
                shift ? printf("!") : printf("1");
                break;
            case 0x03: // 2
                shift ? printf("\"") : printf("2");
                break;
            case 0x04: // 3
                shift ? printf("·") : printf("3");
                break;
            case 0x05: // 4
                shift ? printf("$") : printf("4");
                break;
            case 0x06: // 5
                shift ? printf("%") : printf("5");
                break;
            case 0x07: // 6
                shift ? printf("&") : printf("6");
                break;
            case 0x08: // 7
                shift ? printf("/") : printf("7");
                break;
            case 0x09: // 8
                shift ? printf("(") : printf("8");
                break;
            case 0x0A: // 9
                shift ? printf(")") : printf("9");
                break;
           
            default:
                {
                    // For keys not specifically handled, print the key code in hex.
                    char foo[20] = "KEYBOARD 0x00";
                    char* hex = "0123456789ABCDEF";
                    foo[11] = hex[(key >> 4) & 0x0F];
                    foo[12] = hex[key & 0x0F];
                    printf(foo);
                }
                break;
    }
    return esp;
}