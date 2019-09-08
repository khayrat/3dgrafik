#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"

double t = 3.1415926535 / 180.0;
double add = 0;

double xf( double x )
{
  return 5*sin( x+add ) + 40*sin( x-add ) + 100*sin( x+add );
}

double yf( double y )
{
  return 80*sin( y-add ) + 15*sin( y ) - 20*sin( y-add );
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

  const long xscale = 640;
  const long yscale = 480;

  double y_values[ yscale ];

  long mx = (x_res - xscale) / 2;
  long my = (y_res - yscale) / 2;

  while( 1 )
  {
    for( long y=0 ; y<yscale ; y++ ) y_values[ y ] = yf( y*t );

    for( long x=0 ; x<xscale ; x++ )
    {
      if( input.check() == 1 ) exit( 1 );
      if( input.event_key != 0 ) exit( 1 );

      double x_value = xf( x*t );

      long offset = (my * x_res + mx) + x;
      for( long y=0 ; y<yscale ; y++ )
      {
        long color_offset = uchar( x_value + y_values[ y ] );
        screen[ offset ] = palette.components[ color_offset ];

        offset += x_res;
      }
    }

    add += 0.01;
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
