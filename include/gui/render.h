#ifndef __MYOS__GUI__RENDER_H
#define __MYOS__GUI__RENDER_H

#include <drivers/vga.h>
 #include <common/graphicscontext.h>
 
namespace myos
{
    namespace gui
    {
        class Pixel
        {
         public:
            myos::common::uint8_t r;
            myos::common::uint8_t g;
            myos::common::uint8_t b;
         };
 
        class Render : public myos::drivers::VideoGraphicsArray
        {
         private:
            Pixel pixels[320][200];
 
         public:
            Render(
                common::int32_t w,
                common::int32_t h);
 
            ~Render();
 
            void display(common::GraphicsContext* gc);
 
            void PutPixel(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
        };
    }
}

#endif