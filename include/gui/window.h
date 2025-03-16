#ifndef __MYOS__GUI__WINDOW_H
#define __MYOS__GUI__WINDOW_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace myos
{
    namespace gui
    {
        class Window : public CompositeWidget
        {
         protected:
            bool dragging;
        
         public:
            Window(
                Widget* parent,
                myos::common::int32_t x,
                myos::common::int32_t y,
                myos::common::int32_t w,
                myos::common::int32_t h,
                myos::common::uint8_t r,
                myos::common::uint8_t g,
                myos::common::uint8_t b);

            ~Window();

            void OnMouseDown(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);
            void OnMouseUp(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button);
            void OnMouseMove(myos::common::int32_t oldx, myos::common::int32_t oldy, myos::common::int32_t newx, myos::common::int32_t newy);            
        };
    }
}

#endif