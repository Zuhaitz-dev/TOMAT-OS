#include <drivers/vga.h>

using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::common;



// +--------------+
// |  Some stuff  |
// +--------------+-----------------------------------------------------------------------+
// |                                                                                      |
// | - Some code comes from: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c  |
// |                                                                                      |
// | - Extra: https://wiki.osdev.org/VGA_Hardware                                         |
// |                                                                                      |
// +--------------------------------------------------------------------------------------+
// | - Two types of monitors:                                                             |
// |                                                                                      |
// |    · CRT (Cathode Ray Tube):                                                         |
// |    Uses electron beams that scan across a phosphorescent screen to create an image.  |
// |                                                                                      |
// |    · TFT (Thin-Film Transistor, a type of LCD):                                      |
// |    Uses liquid crystals and a matrix of tiny transistors to control pixels.          |
// +--------------------------------------------------------------------------------------+
// | - WARNING:                                                                           |
// |                                                                                      |
// | Directly modifying VGA registers can lead to unpredictable results, including        |
// | graphical glitches, black screens, or (very unlikely) hardware damage. Worst case    |
// | scenario: your monitor explodes (okay, probably not, but don't say I didn't warn     |
// | you)                                                                                 |
// |                                                                                      |
// +--------------------------------------------------------------------------------------+



VideoGraphicArrays::VideoGraphicArrays() :
    miscPort(0x3C2),
    crtcIndexPort(0x3D4),
    crtcDataPort(0x3D5),
    sequencerIndexPort(0x3C4),
    sequencerDataPort(0x3C5),
    graphicsControllerIndexPort(0x3CE),
    graphicsControllerDataPort(0x3CF),
    attributeControllerIndexPort(0x3C0),
    attributeControllerReadPort(0x3C1),
    attributeControllerWritePort(0x3C0),
    attributeControllerResetPort(0x3DA)
{

}



VideoGraphicArrays::~VideoGraphicArrays()
{

}



void
VideoGraphicArrays::WriteRegisters(uint8_t* registers)
{
    // Misc
    miscPort.Write(*(registers++));

    // Sequencer
    for (uint8_t i = 0; i < 5; i++)
    {
        sequencerIndexPort.Write(i);
        sequencerDataPort.Write(*(registers++));
    }

    // CRT Controller
    crtcIndexPort.Write(0x03);
    crtcDataPort.Write(crtcDataPort.Read() | 0x80);
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for (uint8_t i = 0; i < 25; i++)
    {
        crtcIndexPort.Write(i);
        crtcDataPort.Write(*(registers++));
    }

    // Graphics Controller
    for (uint8_t i = 0; i < 9; i++)
    {
        graphicsControllerIndexPort.Write(i);
        graphicsControllerDataPort.Write(*(registers++));
    }

    // Attribute Controller
    for (uint8_t i = 0; i < 21; i++)
    {
        attributeControllerResetPort.Read();
        attributeControllerIndexPort.Write(i);
        attributeControllerWritePort.Write(*(registers++));
    }

    attributeControllerResetPort.Read();
    attributeControllerIndexPort.Write(0x20);
}



bool
VideoGraphicArrays::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    return width == 320 && height == 200 && colordepth == 8;
}



bool
VideoGraphicArrays::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    if (!SupportsMode(width, height, colordepth))
        return false;

    // From: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
    unsigned char g_320x200x256[] =
    {
        /* MISC */
        0x63,
        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };

    WriteRegisters(g_320x200x256);
    
    return true;
}



uint8_t*
VideoGraphicArrays::GetFrameBufferSegment()
{
    graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = ((graphicsControllerDataPort.Read() >> 2) & 0x03);

    switch (segmentNumber)
    {
        case 0:
            return (uint8_t*)0x00000;
        case 1:
            return (uint8_t*)0xA0000;
        case 2:
            return (uint8_t*)0x0B0000;
        case 3:
            return (uint8_t*)0xB8000;
        default:
            break;
    }
}



void
VideoGraphicArrays::PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex)
{
    uint8_t* pixelAddress = GetFrameBufferSegment() + 320 * y + x;
    *pixelAddress = colorIndex;
}



uint8_t
VideoGraphicArrays::GetColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
    if (r == 0x00 && g == 0x00 && b == 0xA8)
        return 0x01;
}



// Coordinates: XY; Colors: RGB.
void
VideoGraphicArrays::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    PutPixel(x, y, GetColorIndex(r, g, b));
}