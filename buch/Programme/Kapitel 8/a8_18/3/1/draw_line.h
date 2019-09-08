#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include "screen_interface.h"
#include "simple_types.h"

void draw_line( svertex begin, svertex end, pixel_32 c, pixel_32 *screen )
{
  long delta_x, delta_y, e, xstep, ystep, length;
  long offset = begin.sy * x_res + begin.sx;

  delta_x = end.sx - begin.sx;  delta_y = end.sy - begin.sy;
  xstep = 1;  ystep = x_res;

  if( delta_x < 0 ) {  delta_x = -delta_x;  xstep = -xstep;  }
  if( delta_y < 0 ) {  delta_y = -delta_y;  ystep = -ystep;  }

  if( delta_y > delta_x )
  {
    long t = delta_x;  delta_x = delta_y;  delta_y = t;
    t = xstep;  xstep = ystep;  ystep = t;
  }

  length = delta_x+1;  e = 0;
  double act_z = begin.sz;
  double zstep = (end.sz - begin.sz) / length;

  while( length-- > 0 )
  {
    if( act_z > zbuffer[ offset ] )
    {
      screen[ offset ] = c;
      zbuffer[ offset ] = act_z;
    }

    offset += xstep;
    act_z += zstep;

    e += delta_y;
    if( e >= delta_x )
    {
      e -= delta_x;  offset += ystep;
    }
  }
}

#endif
