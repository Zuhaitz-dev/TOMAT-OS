#include <drivers/keyboard.h>

using namespace TomatOS::common;
using namespace TomatOS::drivers;
using namespace TomatOS::hardwarecommunication;



void
printf(char*);



KeyboardEventHandler::KeyboardEventHandler()
{

}



void
KeyboardEventHandler::OnKeyDown(char c)
{
    printf("Pressed: ");
    char tmp[2] = { c, '\0' };
    printf(tmp);
    printf("\n");
}



void
KeyboardEventHandler::OnKeyUp(char)
{

}



KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler)
: InterruptHandler(manager, 0x21),
dataport(0x60),
commandport(0x64)
{
    this->handler = handler;
}



KeyboardDriver::~KeyboardDriver()
{

}



void
printf(char*);

void
printfHex(uint8_t);



void
KeyboardDriver::Activate()
{
    while (commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xAE); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // command 0x60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);
}



uint32_t
KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    // printf("KB IRQ triggered\n");

    // Track shift state
    static bool shift = false;
    uint8_t key = dataport.Read();

    // Filter out ACK (0xFA)
    if(key == 0xFA)
        return esp;
        
    // Debug: print raw scan code as hex.
    // printf("Scan code: 0x");
    // printfHex(key);
    // printf("\n");
    

    if(handler == 0)
    {
        return esp;
    }

    // if != 0xE0 -> == 0x0E. And vice versa.
    if(key == 0x0E)
    {
        // Read the next byte for the extended code.
        uint8_t extended = dataport.Read();
        // Ignore extended key releases.
        if(extended & 0x80)
            return esp;
        // Delete key
        if(extended == 0x0E)
        {
            // Simulate deletion with backspace.
            handler->OnKeyDown('\b');
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

    // IMPORTANT: We have to check now, individually, the characters that take more than one byte.
    // What happens with these is that the color byte is also modified.
    // Surely, if we work with chars that require more than two bytes, other non-existent characters
    // will be printed on the screen. We have to fix this.
    
    // Also, for those who clone this repo, do the necessary changes here. Maybe in the future
    // I will consider a way to choose the keyboard layout, but for now, this is what we have.

    switch (key)
    {
        // case 0x01:
        //     // Kernel shutdown command.
        //     KernelShutdown();
        //     break;
        
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
            handler->OnKeyDown(shift ? 'A' : 'a');
            break;
        case 0x30:
            handler->OnKeyDown(shift ? 'B' : 'b');
            break;
        case 0x2E:
            handler->OnKeyDown(shift ? 'C' : 'c');
            break;
        case 0x20:
            handler->OnKeyDown(shift ? 'D' : 'd');
            break;
        case 0x12:
            handler->OnKeyDown(shift ? 'E' : 'e');
            break;
        case 0x21:
            handler->OnKeyDown(shift ? 'F' : 'f');
            break;
        case 0x22:
            handler->OnKeyDown(shift ? 'G' : 'g');
            break;
        case 0x23:
            handler->OnKeyDown(shift ? 'H' : 'h');
            break;
        case 0x17:
            handler->OnKeyDown(shift ? 'I' : 'i');
            break;
        case 0x24:
            handler->OnKeyDown(shift ? 'J' : 'j');
            break;
        case 0x25:
            handler->OnKeyDown(shift ? 'K' : 'k');
            break;
        case 0x26:
            handler->OnKeyDown(shift ? 'L' : 'l');
            break;
        case 0x32:
            handler->OnKeyDown(shift ? 'M' : 'm');
            break;
        case 0x31:
            handler->OnKeyDown(shift ? 'N' : 'n');
            break;
        case 0x27:
            handler->OnKeyDown(shift ? 'Ñ' : 'ñ');
            break;
        case 0x18:
            handler->OnKeyDown(shift ? 'O' : 'o');
            break;
        case 0x19:
            handler->OnKeyDown(shift ? 'P' : 'p');
            break;
        case 0x10:
            handler->OnKeyDown(shift ? 'Q' : 'q');
            break;
        case 0x13:
            handler->OnKeyDown(shift ? 'R' : 'r');
            break;
        case 0x1F:
            handler->OnKeyDown(shift ? 'S' : 's');
            break;
        case 0x14:
            handler->OnKeyDown(shift ? 'T' : 't');
            break;
        case 0x16:
            handler->OnKeyDown(shift ? 'U' : 'u');
            break;
        case 0x2F:
            handler->OnKeyDown(shift ? 'V' : 'v');
            break;
        case 0x11:
            handler->OnKeyDown(shift ? 'W' : 'w');
            break;
        case 0x2D:
            handler->OnKeyDown(shift ? 'X' : 'x');
            break;
        case 0x15:
            handler->OnKeyDown(shift ? 'Y' : 'y');
            break;
        case 0x2C:
            handler->OnKeyDown(shift ? 'Z' : 'z');
            break;

        case 0x2B:
            handler->OnKeyDown(shift ? 'Ç' : 'ç');
            break;

        // Space key
        case 0x39:
            handler->OnKeyDown(' ');
            break;

        // Enter key
        case 0x1C:
            handler->OnKeyDown('\n');
            break;

        // Punctuation
        case 0x0C: // '
            handler->OnKeyDown(shift ? '?' : '\'');
            break;
        case 0x0D: // ¡
            handler->OnKeyDown(shift ? '¿' : '¡');
            break;
        case 0x1B: // + or *
            handler->OnKeyDown(shift ? '+' : '*');
            break;
        case 0x33: // ,
            handler->OnKeyDown(shift ? ';' : ',');
            break;
        case 0x34: // .
            handler->OnKeyDown(shift ? ':' : '.');
            break;
        case 0x35: // -
            handler->OnKeyDown(shift ? '_' : '-');
            break;

        // Numbers and symbols
        case 0x0B:
            handler->OnKeyDown(shift ? '=' : '0');
            break;
        case 0x02:
            handler->OnKeyDown(shift ? '!' : '1');
            break;
        case 0x03:
            handler->OnKeyDown(shift ? '\"' : '2');
            break;
        case 0x04:
            handler->OnKeyDown(shift ? '·' : '3');
            break;
        case 0x05:
            handler->OnKeyDown(shift ? '$' : '4');
            break;
        case 0x06:
            handler->OnKeyDown(shift ? '%' : '5');
            break;
        case 0x07:
            handler->OnKeyDown(shift ? '&' : '6');
            break;
        case 0x08:
            handler->OnKeyDown(shift ? '/' : '7');
            break;
        case 0x09:
            handler->OnKeyDown(shift ? '(' : '8');
            break;
        case 0x0A:
            handler->OnKeyDown(shift ? ')' : '9');
            break;

        case 0x56:
            handler->OnKeyDown(shift ? '>' : '<');
            break;

        default:
            // For unknown keys, print the key code.
            printf("KEYBOARD 0x");
            printfHex(key);

            // Let's do it like this later:
            // handler->OnKeyDown('?');
            break;
    }

    return esp;
}
