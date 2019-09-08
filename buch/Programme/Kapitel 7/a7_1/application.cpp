#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_8.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_8 b( "b8_1.bmp" );

  long mx = ( x_res - b.xscale ) / 2;
  long my = ( y_res - b.yscale ) / 2;

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    b.display( mx, my, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
