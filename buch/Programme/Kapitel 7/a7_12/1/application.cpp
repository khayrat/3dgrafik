#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"
#include "complex.h"

const long xscale = 450;
const long yscale = 450;

long double begin_cx = -2, end_cx = 2;
long double begin_cy = -2, end_cy = 2;

long mandelbrot( complex c )
{
  complex z = complex( 0, 0 );

  for( long i=0 ; i<256 ; i++ )
  {
    if( input.check() == 1 ) exit( 1 );
    if( input.event_key != 0 ) exit( 1 );

    z = (z * z) + c;

    if( (z.abs() > 2.0) ) return i;
  }

  return 255;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  if( xscale >= x_res || yscale >= y_res )
    exit_error( "Höhe und Breite des Ausgabefensters sind zu groß für die ausgewählte Auflösung.\n" );

  pixel_32 borders[] =
  {
    pixel_32(   0, 0,   0 ), pixel_32(   0,   0, 255 ), pixel_32( 128, 128, 255 ), pixel_32( 255, 255, 255 ),
    pixel_32( 128, 0,   0 ), pixel_32( 255, 255,   0 ), pixel_32(   0, 255,   0 ), pixel_32( 255,   0,   0 ),
    pixel_32( 128, 0, 128 ), pixel_32(   0, 160, 160 ), pixel_32(   0,   0, 255 ), pixel_32(   0,   0,  80 ),
    pixel_32(   0, 0,   0 )
  };
  primary_color palette;  palette.load( 256, 13, borders );

  long mx = (x_res - xscale) / 2;
  long my = (y_res - yscale) / 2;

  while( 1 )
  {
    for( long sy=0 ; sy<yscale ; sy++ )
    {
      for( long sx=0 ; sx<xscale ; sx++ )
      {
        long double cx = (end_cx - begin_cx) / double( xscale ) * sx + begin_cx;
        long double cy = (begin_cy - end_cy) / double( yscale ) * sy + end_cy;

        uchar color = uchar( mandelbrot( complex( cx, cy ) ) );

        screen[ (my+sy)*x_res+(mx+sx) ] = palette.components[ color ];
      }
    }
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
