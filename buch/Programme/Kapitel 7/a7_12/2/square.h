#ifndef SQUARE_H
#define SQUARE_H

#include "simple_types.h"
#include "bitmap_32.h"

struct square
{
  long size;
  svertex pos;
  pixel_32 color;

  void display( bitmap_32 *bmp );

  square( long s, svertex p, pixel_32 c ) {  size = s;  pos = p;  color = c;  }
};

void square::display( bitmap_32 *bmp )
{
  for( long z=0 ; z<=size ; z++ )
  {
    bmp->picture[ pos.sy * bmp->xscale + (pos.sx + z) ] = color;
    bmp->picture[ (pos.sy+size) * bmp->xscale + (pos.sx + z) ] = color;
    bmp->picture[ (pos.sy + z) * bmp->xscale + pos.sx ] = color;
    bmp->picture[ (pos.sy + z) * bmp->xscale + (pos.sx + size) ] = color;
  }
}

#endif
