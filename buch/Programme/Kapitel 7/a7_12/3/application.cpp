#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"
#include "bitmap_32.h"
#include "square.h"
#include "complex.h"

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

const long xscale = 450;
const long yscale = 450;
long fractal[ xscale * yscale ];

long double begin_cx = -2, end_cx = 2;
long double begin_cy = -2, end_cy = 2;

uchar handle_input( square *lens );
void mandelbrot_set( long mx, long my, pixel_32 *pal, pixel_32 *screen );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten:\tBewegung der Bereichauswahl\n\nEscape:\t\tProgramm beenden" );

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  if( xscale >= x_res || yscale >= y_res )
    exit_error( "H�he und Breite des Ausgabefensters sind zu gro� f�r die ausgew�hlte Aufl�sung.\n" );

  pixel_32 borders[] =
  {
    pixel_32(   0, 0,   0 ), pixel_32(   0,   0, 255 ), pixel_32( 128, 128, 255 ), pixel_32( 255, 255, 255 ),
    pixel_32( 128, 0,   0 ), pixel_32( 255, 255,   0 ), pixel_32(   0, 255,   0 ), pixel_32( 255,   0,   0 ),
    pixel_32( 128, 0, 128 ), pixel_32(   0, 160, 160 ), pixel_32(   0,   0, 255 ), pixel_32(   0,   0,  80 ),
    pixel_32(   0, 0,   0 )
  };
  primary_color palette;  palette.load( 256, 13, borders );

  bitmap_32 buffer( xscale, yscale );

  const long scale = 50;
  square lens( scale, svertex( (xscale - scale) / 2, (yscale - scale) / 2 ), pixel_32( 255, 255, 255 ) );

  long mx = (x_res - xscale) / 2;
  long my = (y_res - yscale) / 2;

  mandelbrot_set( mx, my, palette.components, screen );

  while( 1 )
  {
    if( handle_input( &lens ) ) break;

    long pixel_count = xscale * xscale;
    for( long x=0 ; x<pixel_count ; x++ ) buffer.picture[ x ] = palette.components[ fractal[ x ] ];

    lens.display( &buffer );

    buffer.display( mx, my, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

uchar handle_input( square *lens )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  if( input.key_pressed( VK_LEFT ) )
    {  lens->pos.sx--;  if( lens->pos.sx < 0 ) lens->pos.sx = 0;  }

  if( input.key_pressed( VK_RIGHT ) )
    {  lens->pos.sx++;  if( lens->pos.sx+lens->size >= xscale ) lens->pos.sx = xscale-1-lens->size;  }

  if( input.key_pressed( VK_UP ) )
    {  lens->pos.sy--;  if( lens->pos.sy < 0 ) lens->pos.sy = 0;  }

  if( input.key_pressed( VK_DOWN ) )
    {  lens->pos.sy++;  if( lens->pos.sy+lens->size >= yscale ) lens->pos.sy = yscale-1-lens->size;  }

  return 0;
}

void mandelbrot_set( long mx, long my, pixel_32 *pal, pixel_32 *screen )
{
  for( long sy=0 ; sy<yscale ; sy++ )
  {
    for( long sx=0 ; sx<xscale ; sx++ )
    {
      long double cx = (end_cx - begin_cx) / double( xscale ) * sx + begin_cx;
      long double cy = (begin_cy - end_cy) / double( yscale ) * sy + end_cy;

      uchar color = uchar( mandelbrot( complex( cx, cy ) ) );

      fractal[ sy * xscale + sx ] = color;
      screen[ (my+sy) * x_res + (mx+sx) ] = pal[ color ];
    }
  }
}
