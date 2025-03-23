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
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler
        {
         protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;

            // For blinking cursor
            common::uint8_t frameCounter;
        
         public:
            Desktop(
                common::int32_t w,
                common::int32_t h,
                common::uint8_t r,
                common::uint8_t g,
                common::uint8_t b);
            
            ~Desktop();

            void Draw(common::GraphicsContext* gc);
            
            void OnMouseDown(common::uint8_t button);
            void OnMouseUp(common::uint8_t button);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif