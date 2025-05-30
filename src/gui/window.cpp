#include <gui/window.h>

using namespace myos::common;
using namespace myos::gui;

Window::Window(
    Widget* parent,
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    dragging = false;
}



Window::~Window()
{

}



void
Window::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{    
    // Only if it's the left button
    dragging = button == 1;
    CompositeWidget::OnMouseDown(x, y, button);
}



void 
Window::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{
    dragging = false;
    CompositeWidget::OnMouseUp(x, y, button);
}



void
Window::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy)
{
    if (dragging)
    {
        this -> x += newx - oldx;
        this -> y += newy - oldy;
    }

    CompositeWidget::OnMouseMove(oldx, oldy, newx, newy);
}