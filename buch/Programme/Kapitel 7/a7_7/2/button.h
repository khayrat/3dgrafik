#ifndef BUTTON_H
#define BUTTON_H

#include "simple_types.h"
#include "bitmap_32.h"
#include "input_controller.h"

class button
{
  private:
    bitmap_32 up, down, selected;

    long sx, sy;

    uchar cursor_above( svertex pos );

  public:
    void load( long bx, long by, char *u, char *d, char *s );

    void display( pixel_32 *sbuffer, uchar *running );

    button( long bx, long by, char *u, char *d, char *s ) {  load( bx, by, u, d, s );  }
};

uchar button::cursor_above( svertex pos )
{
  return pos.sx >= sx && pos.sx <= (sx + up.xscale) &&
         pos.sy >= sy && pos.sy <= (sy + up.yscale);
}

void button::display( pixel_32 *sbuffer, uchar *running )
{
  if( cursor_above( input.mouse_act_pos ) == 1 )
  {
    switch( input.mouse_left_state )
    {
      case  0 : selected.display( sx, sy, sbuffer );
                return;
      case -1 : if( cursor_above( input.mouse_down_pos ) ) {  down.display( sx, sy, sbuffer );  return;  }
                break;
      case  1 : if( cursor_above( input.mouse_down_pos ) ) {  down.display( sx, sy, sbuffer );  *running = 0;  }
                return;
    }
  }
   
  up.display( sx, sy, sbuffer );
}

void button::load( long bx, long by, char *u, char *d, char *s )
{
  sx = bx;  sy = by;

  pixel_32 tr = pixel_32( 0, 255, 0 );
  up.load( u, tr );  down.load( d, tr );  selected.load( s, tr );

  if( up.xscale != down.xscale || down.xscale != selected.xscale )
    exit_error( "button::load(): Bitmaps müssen über dieselbe Breite verfügen.\n" );

  if( up.yscale != down.yscale || down.yscale != selected.yscale )
    exit_error( "button::load(): Bitmaps müssen über dieselbe Höhe verfügen.\n" );
}

#endif