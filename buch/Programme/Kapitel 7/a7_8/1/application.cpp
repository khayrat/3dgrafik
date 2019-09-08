#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Leertaste:\tÄnderung der Farbpalette\n\nEscape:\t\tEnde des Programms\n ");

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 c1[ 6 ] = 
  {
    pixel_32( 0, 0, 0 ),     pixel_32( 0, 0, 128 ),     pixel_32( 255, 0, 0 ),
    pixel_32( 255, 255, 0 ), pixel_32( 255, 255, 255 ), pixel_32( 255, 255, 255 )
  };

  pixel_32 c2[ 5 ] = 
  {
    pixel_32( 0, 0, 0 ),        pixel_32( 0, 0, 200 ),     pixel_32( 0, 200, 200 ),
    pixel_32( 255, 255, 255 ),  pixel_32( 255, 255, 255 )
  };

  const long component_count = 256;

  primary_color palette[ 2 ];
  palette[ 0 ].load( component_count, 6, c1 );
  palette[ 1 ].load( component_count, 5, c2 );

  long *sbuffer = new long[ x_res*y_res ];
  if( sbuffer == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long x;

  long pixel_count = x_res*y_res;
  for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = 0;

  uchar act_color = 0;

  while( 1 )
  {
    if( input.check() == 1 ) return 1;
    if( input.event_key == VK_ESCAPE ) return 1;
    if( input.event_key == VK_SPACE ) {  ++act_color;  if( act_color > 1 ) act_color = 0;  }

    for( x=pixel_count - 2*x_res ; x<pixel_count ; x++ )
    {
      if( (rand() % 100) < 30 ) sbuffer[ x ] = 0;

      else sbuffer[ x ] = component_count - 1;
    }

    for( x=x_res ; x<pixel_count-x_res-1 ; x++ )
    {
      sbuffer[ x-x_res ] = (sbuffer[ x ] + sbuffer[ x-1 ] +
                            sbuffer[ x+1 ] + sbuffer[ x+x_res-1 ] +
                            sbuffer[ x+x_res ] + sbuffer[ x+x_res+1 ]) / 6;

      if( sbuffer[ x-x_res ] > 0 ) sbuffer[ x-x_res ]--;
    }

    for( x=0 ; x<pixel_count ; x++ )
       screen[ x ] = palette[ act_color ].components[ sbuffer[ x ] ];
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
