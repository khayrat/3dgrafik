#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"
#include "heightfield.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Enter:\tWechsel des Fraktals\n\nEscape:\tProgramm beenden" );

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 borders[ 3 ] = {  pixel_32( 40, 40, 255 ), pixel_32( 128, 128, 128 ), pixel_32( 255, 255, 255 )  };
  primary_color palette;  palette.load( 256, 3, borders );

  long grad = 350;

  heightfield clouds( 450, 450 );
  clouds.plasma_fractal( grad );

  if( clouds.xscale >= x_res || clouds.yscale >= y_res )
    exit_error( "Plasmafraktal ist zu groß für das aktuelle Programmfenster.\n" );

  long mx = (x_res - clouds.xscale) / 2;
  long my = (y_res - clouds.yscale) / 2;

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key == VK_ESCAPE ) break;

    if( input.event_key == VK_RETURN )
      clouds.plasma_fractal( grad );

    clouds.display( mx, my, palette.components, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
