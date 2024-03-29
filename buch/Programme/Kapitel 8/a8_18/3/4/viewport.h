#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "simple_types.h"
#include "global_definitions.h"
#include "draw_line.h"
#include "matrix.h"

class viewport
{
  private:
    long x_resolution, y_resolution;

	double z_minimum, z_maximum;
	long x_minimum, x_maximum, y_minimum, y_maximum;

    double x_constant, y_constant;

	svertex screen_pos;

	pixel_32 *window;
    double *depth_buffer;

  public:
    void initialise( long xr, long yr, long sx, long sy );

	void install( void );
    void display( pixel_32 *screen, uchar frame );

	viewport( long xr, long yr, long sx, long sy ) {  initialise( xr, yr, sx, sy );  }
	viewport( const viewport &c ) {  exit_error( "viewport: Aufruf Kopierkonstruktor.\n" );  }
   ~viewport( void ) {  if( window ) delete [] window;  if( depth_buffer ) delete [] depth_buffer;  }
};

void viewport::install( void )
{
  sbuffer = window;  zbuffer = depth_buffer;

  x_res = x_resolution;  y_res = y_resolution;
  z_min = z_minimum;  z_max = z_maximum;
  x_min = x_minimum;  x_max = x_maximum;  y_min = y_minimum;  y_max = y_maximum;

  xpr_cnst = x_constant;  ypr_cnst = y_constant;

  memset( depth_buffer, 0, x_resolution * y_resolution * sizeof( depth_buffer[ 0 ] ) );
  memset( window, 10, x_resolution * y_resolution * sizeof( window[ 0 ] ) );
}

void viewport::initialise( long xr, long yr, long sx, long sy )
{
  screen_pos = svertex( sx, sy );

  x_resolution = xr;  y_resolution = yr;
  x_minimum = 0;      y_minimum = 0;
  x_maximum = xr-1;   y_maximum = yr-1;

  z_minimum = 10;  z_maximum = 100000;

  if
  (
    screen_pos.sx < 0 || screen_pos.sy < 0 ||
    screen_pos.sx+x_resolution > screen_x_res || screen_pos.sy+y_resolution > screen_y_res
  )
  exit_error( "viewport::initialise(): Die gegebene Bildschirmauflösung ist zu klein für das Darstellungsfenster.\n" );

  x_constant = (800.0 / 640.0) * xr;  y_constant = (800.0 / 480.0) * yr;

  if(
	  (window = new pixel_32[ x_resolution * y_resolution ]) == NULL ||
      (depth_buffer = new double[ x_resolution * y_resolution ]) == NULL
	) 
	exit_error( "viewport::initialise(): Fehler bei der Reservierung von Arbeitsspeicher.\n" );
}

void viewport::display( pixel_32 *screen, uchar frame )
{
  if( frame == 1 )
  {
    long pixel_count = x_resolution * y_resolution;
    for( long x=0 ; x<x_resolution ; x++ )
       window[ x ] = window[ pixel_count-1-x ] = pixel_32( 255, 255, 255 );

    long offset = 0;
    for( long y=0 ; y<y_resolution ; y++ )
    {
      window[ offset ] = window[ offset+x_resolution-1 ] = pixel_32( 255, 255, 255 );

      offset += x_resolution;
    }
  }

  long screen_offset = screen_pos.sy * screen_x_res;
  long window_offset = 0;
  long byte_count = x_resolution * sizeof( window[ 0 ] );

  for( long y=0 ; y<y_resolution ; y++ )
  {
    memmove( screen+screen_offset+screen_pos.sx, window+window_offset, byte_count );

	screen_offset += screen_x_res;
	window_offset += x_resolution;
  }
}

#endif