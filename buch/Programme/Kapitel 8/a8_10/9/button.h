#ifndef BUTTON_H
#define BUTTON_H

#include "simple_types.h"
#include "bitmap_32.h"
#include "input_controller.h"

class button
{
  private:
    bitmap_32 up, down, selected;
    void (* function) ( void );

    long sx, sy;
    uchar event;

    uchar cursor_above( svertex pos );

  public:
    void load( long bx, long by, char *u, char *d, char *s, void (*f)( void ), uchar e );

    void display( void );

    button( void ) : sx( 0 ), sy( 0 ), event( 0 ), function( NULL ) { }
};

uchar button::cursor_above( svertex pos )
{
  return pos.sx >= sx && pos.sx <= (sx + up.xscale) &&
         pos.sy >= sy && pos.sy <= (sy + up.yscale);
}

void button::display( void )
{
  if( cursor_above( input.mouse_act_pos ) == 1 )
  {
    switch( input.mouse_left_state )
    {
      case  0 : selected.display( sx, sy );
                return;
      case -1 : if( cursor_above( input.mouse_down_pos ) )
                {  down.display( sx, sy );  if( event == 0 ) function();  return;  }
                break;
      case  1 : if( cursor_above( input.mouse_down_pos ) )
                {  down.display( sx, sy );  if( event == 1 ) function();  }
                return;
    }
  }
   
  up.display( sx, sy );
}

void button::load( long bx, long by, char *u, char *d, char *s, void (*f)( void ), uchar e )
{
  sx = bx;  sy = by;
  function = f;
  event = e;

  up.load( u );  down.load( d );  selected.load( s );

  if( up.xscale != down.xscale || down.xscale != selected.xscale )
    exit_error( "button::load(): Bitmaps müssen über dieselbe Breite verfügen.\n" );

  if( up.yscale != down.yscale || down.yscale != selected.yscale )
    exit_error( "button::load(): Bitmaps müssen über dieselbe Höhe verfügen.\n" );
}

#endif