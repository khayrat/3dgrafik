#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_32.h"

const long x_length = 150, y_length = 150;  
double x_speed = 0.3, y_speed = 0.3;

void update_pos( double *sx, double *sy );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  // dieses Programm funktioniert nur unter der Auflösung 640x480
  // eine andere Auflösung erfordert einen entsprechend großen Bildschirmhintergrund "bground.bmp"

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_32 bground( "bground.bmp" );
  
  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ x_res * y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitspeicher.\n" );

  double sx = (x_res - x_length) / 2;
  double sy = (y_res - y_length) / 2;

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    update_pos( &sx, &sy );
    bground.display_partial( long( sx ), long( sy ), x_length, y_length, long( sy ) * x_res + long( sx ), sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;
  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

void update_pos( double *sx, double *sy )
{
  *sx += x_speed;
  if( *sx + x_length >= x_res ) {  *sx = x_res - x_length;  x_speed = -x_speed;  }
  else if( *sx < 0 ) {  *sx = 0; x_speed = -x_speed;  }

  *sy += y_speed;
  if( *sy + y_length >= y_res ) {  *sy = y_res - y_length;  y_speed = -y_speed;  }
  else if( *sy < 0 ) {  *sy = 0; y_speed = -y_speed;  }
}