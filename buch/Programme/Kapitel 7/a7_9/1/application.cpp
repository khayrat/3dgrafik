#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"

double simple_xf( double x )
{
  return 5*sin( x ) + 40*sin( x ) + 100*sin( x );
}

double simple_yf( double y )
{
  return 80*sin( y ) + 15*sin( y ) - 20*sin( y );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 borders[ 9 ] = 
  {
    pixel_32( 0, 0, 0 ),       pixel_32( 0, 0, 255 ), pixel_32( 255, 0, 0 ), pixel_32( 255, 255, 0 ),
    pixel_32( 255, 255, 255 ),
    pixel_32( 255, 255, 0 ),   pixel_32( 255, 0, 0 ), pixel_32( 0, 0, 255 ), pixel_32( 0, 0, 0 )
  };
  primary_color palette;  palette.load( 256, 9, borders );

  double t = 3.1415926535 / 180.0;
  const long xscale = 640;
  const long yscale = 480;

  while( 1 )
  {
    for( long x=0 ; x<xscale ; x++ )
    {
      double x_value = simple_xf( x*t );

      long offset = x;
      for( long y=0 ; y<yscale ; y++ )
      {
        if( input.check() == 1 ) exit( 1 );
        if( input.event_key != 0 ) exit( 1 );

        double y_value = simple_yf( y*t );

        long color_offset = uchar( x_value + y_value );
        screen[ offset ] = palette.components[ color_offset ];

        offset += x_res;
      }
    }
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
