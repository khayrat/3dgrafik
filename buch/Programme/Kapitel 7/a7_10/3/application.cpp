#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"
#include "heightfield.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Enter:\t\tWechsel des Fraktals\nLeertaste:\tWechsel der Farbpalette\n\nEscape:\t\tProgramm beenden" );

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 borders3[ 3 ] = {  pixel_32( 40, 40, 255 ), pixel_32( 128, 128, 128 ), pixel_32( 255, 255, 255 )  };

  pixel_32 borders5[ 5 ] =
  {
    pixel_32(  40,  40, 255 ), pixel_32( 128, 128, 128 ),
    pixel_32( 255, 255, 255 ),
    pixel_32( 128, 128, 128 ), pixel_32(  40,  40, 255 )
  };

  uchar act_pal = 0;
  primary_color palette[ 2 ];
  palette[ 0 ].load( 256, 3, borders3 );
  palette[ 1 ].load( 256, 5, borders5 );

  long grad = 500;

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

    if( input.event_key == VK_SPACE ) if( ++act_pal > 1 ) act_pal = 0;

    clouds.display( mx, my, palette[ act_pal ].components, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
