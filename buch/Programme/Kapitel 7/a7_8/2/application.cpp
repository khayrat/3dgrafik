#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_8.h"
#include "primary_color.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Leertaste:\tÄnderung der Farbpalette\n\nEscape:\t\tEnde des Programms\n ");

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 c1[ 6 ] = 
  {
    pixel_32(   0,   0, 0 ), pixel_32(   0,   0, 128 ), pixel_32( 255,   0,   0 ),
    pixel_32( 255, 255, 0 ), pixel_32( 255, 255, 255 ), pixel_32( 255, 255, 255 )
  };

  pixel_32 c2[ 5 ] = 
  {
    pixel_32(   0,   0,   0 ), pixel_32(   0,   0, 200 ), pixel_32( 0, 200, 200 ),
    pixel_32( 255, 255, 255 ), pixel_32( 255, 255, 255 )
  };

  const long component_count = 512;

  primary_color palette[ 2 ];
  palette[ 0 ].load( component_count, 6, c1 );
  palette[ 1 ].load( component_count, 5, c2 );

  bitmap_8 fire( 540, 380 );

  long mx = (x_res - fire.xscale) / 2;
  long my = (y_res - fire.yscale) / 2;

  uchar act_color = 0;

  while( 1 )
  {
    if( input.check() == 1 ) return 1;
    if( input.event_key == VK_ESCAPE ) return 1;
    if( input.event_key == VK_SPACE ) {  ++act_color;  if( act_color > 1 ) act_color = 0;  }

    fire.update( component_count );

    fire.display( mx, my, palette[ act_color ].components, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
