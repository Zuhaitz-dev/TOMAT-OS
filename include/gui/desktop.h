#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

// > NOTE | Not recommended to hard code the mouse. Imagine a Tablet scenario, for example, the cursor won't work the same.
//        | Might consider this in the future. But now, fuck it.

namespace myos
{
    namespace gui
    {
        class Desktop : public CompositeWidget, public myos::drivers::MouseEventHandler
        {
         protected:
            myos::common::uint32_t MouseX;
            myos::common::uint32_t MouseY;
        
         public:
            Desktop(
                myos::common::int32_t w,
                myos::common::int32_t h,
                myos::common::uint8_t r,
                myos::common::uint8_t g,
                myos::common::uint8_t b);
            
            ~Desktop();

            void Draw(myos::common::GraphicsContext* gc);
            void OnMouseDown(myos::common::uint8_t button);
            void OnMouseUp(myos::common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif