#include <gui/render.h>

using namespace myos::common;
using namespace myos::gui;
 
Render::Render(
    int32_t w,
    int32_t h)
{
// Pixel[w][h] pixels;
}



Render::~Render()
{

}



// Displays pre-rendered frame to screen
void
Render::display(GraphicsContext* gc)
{
    for(int ix = 0; ix < 320; ix++)
        for(int iy = 0; iy < 200; iy++)
            gc->PutPixel(ix,iy, pixels[ix][iy].r, pixels[ix][iy].g, pixels[ix][iy].b);
}



void
Render::PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b){
    pixels[x][y].r = r;
    pixels[x][y].g = g;
    pixels[x][y].b = b;
}