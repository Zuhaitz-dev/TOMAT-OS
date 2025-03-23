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
            common::uint8_t r;
            common::uint8_t g;
            common::uint8_t b;
         };
 
        class Render : public drivers::VideoGraphicsArray
        {
         private:
            Pixel pixels[320][200];
 
         public:
            Render(
                common::int32_t w,
                common::int32_t h);
 
            ~Render();
 
            void display(common::GraphicsContext* gc);
 
            void PutPixel(
                common::int32_t x, common::int32_t y,
                common::uint8_t r, common::uint8_t g, common::uint8_t b);
        };
    }
}

#endif