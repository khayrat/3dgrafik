#ifndef PRIMARY_COLOR_H
#define PRIMARY_COLOR_H

#include <fstream.h>

#include "screen_interface.h"
#include "simple_types.h"

class primary_color
{
  public:
    long color_count;
    pixel_32 *colors;

    void load( long count, pixel_32 begin, pixel_32 end );

    primary_color( void ) : color_count( 0 ), colors( NULL ) { }
	primary_color( const primary_color &p ) {  exit_error( "primary_color: Aufruf Kopierkonstruktor.\n" );  }
   ~primary_color( void ) {  if( colors ) delete [] colors;  }
};

void primary_color::load( long count, pixel_32 begin, pixel_32 end )
{
  color_count = count;
  
  if( (colors = new pixel_32[ color_count ]) == NULL )
    exit_error( "*colors: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  double r_step = double( end.red - begin.red ) / color_count;
  double g_step = double( end.green - begin.green ) / color_count;
  double b_step = double( end.blue - begin.blue ) / color_count;

  double act_r = begin.red;
  double act_g = begin.green;
  double act_b = begin.blue;

  long z = 0;

  for( long y=0 ; y<color_count ; y++ )
  {
    colors[ z++ ] = pixel_32( uchar( act_r + 0.5 ), uchar( act_g + 0.5 ), uchar( act_b + 0.5 ) );

    act_r += r_step;  act_g += g_step;  act_b += b_step;
  }

  pixel_32 c = colors[ z-1 ];
  for( ; z < color_count ; z++ ) colors[ z ] = c;
}

#endif