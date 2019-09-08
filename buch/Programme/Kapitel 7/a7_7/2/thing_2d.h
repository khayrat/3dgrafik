#ifndef THING_2D_H
#define THING_2D_H

#include "st_bitmap.h"

class thing_2d
{
  private:
    long lx, ly;
    
    st_bitmap shape;

  public:
    svertex spos;
    
    void update_pos( vector v ) {  spos.sx += v.x;  spos.sy += v.y;  }
    void display( pixel_32 *sbuffer );

    void load( st_bitmap *b, long x, long y );

    thing_2d( void ) : lx( 0 ), ly( 0 ), spos( svertex( 0, 0 ) ) { }
    thing_2d( st_bitmap *b, long x, long y ) {  load( b, x, y );  }
};

void thing_2d::display( pixel_32 *sbuffer )
{	
  long bx = long( spos.sx - lx );
  long by = long( spos.sy - ly );
  
  long ex = bx + shape.xscale - 1;
  long ey = by + shape.yscale - 1;

  long px = 0;  
  long py = 0;
  if( bx < 0 ) {  px = -bx;  bx = 0;  }
  if( by < 0 ) {  py = -by;  by = 0;  }

  if( ex >= x_res ) ex = x_res - 1;
  if( ey >= y_res ) ey = y_res - 1;
  
  short x_length = (ex - bx + 1);  
  short y_length = (ey - by + 1);
  long picture_offset = py * shape.xscale + px;
  long screen_offset = by * x_res + bx;  

  if( bx < x_res && by < y_res && ex > 0 && ey > 0 )
    shape.draw_partial( picture_offset, x_length, y_length, screen_offset, sbuffer );
}

void thing_2d::load( st_bitmap *b, long x, long y )
{
  lx = x;
  ly = y;
  
  shape.load( b );
  
  spos.sx = spos.sy = 0.0;
}

#endif