#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_32.h"
#include "thing_2d.h"
#include "button.h"

const double spot_speed = 1.5;

void move_thing( thing_2d *spot );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_32 arrow( "cursor.bmp", pixel_32( 0, 0, 0 ) );  
  thing_2d cursor( &arrow, 0, 0 );

  bitmap_32 spot_shape( "spot.bmp", pixel_32( 0, 0, 0 ) );
  thing_2d spot( &spot_shape, spot_shape.xscale/2, spot_shape.yscale/2 );

  bitmap_32 bground( "bground.bmp", pixel_32( 0, 0, 0 ) );

  long mx = (x_res - bground.xscale) / 2;
  long my = (y_res - bground.yscale) / 2;

  button prog_end( mx+450, my+401, "up.bmp", "down.bmp", "selected.bmp" );
  
  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ x_res * y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitspeicher.\n" );

  uchar running = 1;

  while( running == 1 )
  {
    if( input.check() == 1 ) running = 0;
    if( input.event_key != 0 ) running = 0;

    memset( sbuffer, 255, x_res * y_res * sizeof( sbuffer[ 0 ] ) );
    bground.display( mx, my, sbuffer );

    prog_end.display( sbuffer, &running );

    move_thing( &spot );
    spot.display( sbuffer );

    cursor.spos = input.mouse_act_pos;
    cursor.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;
  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

void move_thing( thing_2d *spot )
{
  double x_step = input.mouse_act_pos.sx - spot->spos.sx;
  double y_step = input.mouse_act_pos.sy - spot->spos.sy;
  vector dir( x_step, y_step );
  
  double t;
  if( dir.length > 2.0 ) t = spot_speed / dir.length;
  else t = 1.0;
 
  dir.x *= t;  dir.y *= t;
  spot->update_pos( dir );
}