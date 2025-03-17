#include <gui/desktop.h>

using namespace myos::common;
using namespace myos::gui;

Desktop::Desktop(
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b)
: CompositeWidget(0, 0, 0, w, h, r, g, b),
MouseEventHandler(),
frameCounter(0)
{
    MouseX = w / 2;
    MouseY = h / 2;
}


Desktop::~Desktop()
{

}



void
Desktop::Draw(myos::common::GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);

    // The conditional is to make sure we have no overflow
    frameCounter++;
    if(frameCounter > 10000)
        frameCounter = 0;

    // Toggle the cursor color every 2 frames (choose whatever you want tbh)
    // The cursor alternates between white (0xFF) and gray (0x80)
    bool blinkOn = ((frameCounter / 2) % 2 == 0);
    uint8_t cursorColor = blinkOn ? 0xFF : 0x80;

    for (int i = 0; i < 4; i++)
    {
        gc->PutPixel(MouseX - i, MouseY, cursorColor, cursorColor, cursorColor);
        gc->PutPixel(MouseX + i, MouseY, cursorColor, cursorColor, cursorColor);
        gc->PutPixel(MouseX, MouseY - i, cursorColor, cursorColor, cursorColor);
        gc->PutPixel(MouseX, MouseY + i, cursorColor, cursorColor, cursorColor);
    }
}


void
Desktop::OnMouseDown(myos::common::uint8_t button)
{
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}



void
Desktop::OnMouseUp(myos::common::uint8_t button)
{
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
}



void
Desktop::OnMouseMove(int x, int y)
{

    // To control the sensitivity. If not it's like a F1.
    x /= 4;
    y /= 4;

    int32_t newMouseX = MouseX + x;
    if (newMouseX < 0) newMouseX = 0;
    if (newMouseX >= w) newMouseX = w - 1;

    int32_t newMouseY = MouseY + y;
    if (newMouseY < 0) newMouseY = 0;
    if (newMouseY >= h) newMouseY = h - 1;

    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);

    MouseX = newMouseX;
    MouseY = newMouseY;
}