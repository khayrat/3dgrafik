#ifndef PRIMARY_COLOR_H
#define PRIMARY_COLOR_H

#include "screen_interface.h"
#include "simple_types.h"

class primary_color
{
  public:
    long component_count;
    pixel_32 *components;

    void load( long count, FILE *input );
    void load( long count, long border_count, pixel_32 *borders );

    primary_color( void ) : component_count( 0 ), components( NULL ) { }
	primary_color( const primary_color &p ) {  exit_error( "primary_color: Aufruf Kopierkonstruktor.\n" );  }
   ~primary_color( void ) {  if( components ) delete [] components;  }
};

void primary_color::load( long count, long border_count, pixel_32 *borders )
{
  component_count = count;
  
  if( (components = new pixel_32[ component_count ]) == NULL )
    exit_error( "*components: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long local_range = component_count / (border_count - 1);

  long z = 0;

  for( long x=0 ; x<border_count-1 ; x++ )
  {
    double r_step = double( borders[ x+1 ].red - borders[ x ].red ) / local_range;
    double g_step = double( borders[ x+1 ].green - borders[ x ].green ) / local_range;
    double b_step = double( borders[ x+1 ].blue - borders[ x ].blue ) / local_range;

    double act_r = borders[ x ].red;
    double act_g = borders[ x ].green;
    double act_b = borders[ x ].blue;

    for( long y=0 ; y<local_range ; y++ )
    {
      components[ z++ ] = pixel_32( uchar( act_r + 0.5 ), uchar( act_g + 0.5 ), uchar( act_b + 0.5 ) );

      act_r += r_step;  act_g += g_step;  act_b += b_step;
    }
  }

  pixel_32 c = components[ z-1 ];
  for( ; z < component_count ; z++ ) components[ z ] = c;
}

void primary_color::load( long count, FILE *input )
{
  long border_count;
  pixel_32 *borders;

  fscanf( input, "%ld", &border_count );

  if( border_count < 2 )
    exit_error( "primary_color::load(): Ein Farbverlauf muss mindestens zwei Grenzfarben besitzen.\n" );

  if( (borders = new pixel_32[ border_count ]) == NULL )
    exit_error( "*borders: Fehler bei der Reservierung von Arbeitsspeicher.\n ");

  for( long x=0 ; x<border_count ; x++ )
  {
    long red, green, blue;

    fscanf( input, "%ld %ld %ld", &red, &green, &blue );
    borders[ x ] = pixel_32( uchar( red ), uchar( green ), uchar( blue ) );
  }

  load( count, border_count, borders );

  delete [] borders;
}

#endif