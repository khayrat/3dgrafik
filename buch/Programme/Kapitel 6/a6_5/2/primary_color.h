#ifndef PRIMARY_COLOR_H
#define PRIMARY_COLOR_H

#include <fstream.h>

#include "screen_interface.h"
#include "simple_types.h"

class primary_color
{
  public:
    long component_count;
    pixel_32 *components;

    void load( long count, pixel_32 begin, pixel_32 end );

    primary_color( void ) : component_count( 0 ), components( NULL ) { }
	primary_color( const primary_color &p ) {  exit_error( "primary_color: Aufruf Kopierkonstruktor.\n" );  }
   ~primary_color( void ) {  if( components ) delete [] components;  }
};

void primary_color::load( long count, pixel_32 begin, pixel_32 end )
{
  component_count = count;
  
  if( (components = new pixel_32[ component_count ]) == NULL )
    exit_error( "primary_color::*components: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  double r_step = double( end.red - begin.red ) / component_count;
  double g_step = double( end.green - begin.green ) / component_count;
  double b_step = double( end.blue - begin.blue ) / component_count;

  double act_r = begin.red;
  double act_g = begin.green;
  double act_b = begin.blue;

  long z = 0;

  for( long y=0 ; y<component_count ; y++ )
  {
    components[ z++ ] = pixel_32( uchar( act_r + 0.5 ), uchar( act_g + 0.5 ), uchar( act_b + 0.5 ) );

    act_r += r_step;  act_g += g_step;  act_b += b_step;
  }

  pixel_32 c = components[ z-1 ];
  for( ; z < component_count ; z++ ) components[ z ] = c;
}

#endif