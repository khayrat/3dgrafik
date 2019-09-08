#ifndef PALETTE_H
#define PALETTE_H

#include <stdio.h>

#include "simple_types.h"
#include "primary_color.h"

class palette
{
  public:
    long color_count, range_size;

    primary_color *colors;

    void load( long rs, char *filename );  

    palette( const palette &p ) {  exit_error( "palette: Aufruf Kopierkonstruktor\n" );  }
    palette( long rs, char *filename ) {  load( rs, filename );  }
   ~palette( void ) {  if( colors ) delete [] colors;  }
} used_colors( 256, "palette.pl1" );

void palette::load( long rs, char *filename )
{
  range_size = rs;

  FILE *input;

  if( (input = fopen( filename, "rb" )) == NULL )
    exit_nofile( "palette::load()", filename );

  fscanf( input, "%ld", &color_count );

  if( (colors = new primary_color[ color_count ]) == NULL ) 
    exit_error( "palette::*colors: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<color_count ; x++ )
  {
    long r1, g1, b1;
    fscanf( input, "%ld %ld %ld", &r1, &g1, &b1 );

    long r2, g2, b2;
    fscanf( input, "%ld %ld %ld", &r2, &g2, &b2 );

    colors[ x ].load
    (
      range_size,
      pixel_32( uchar( r1 ), uchar( g1 ), uchar( b1 ) ),
      pixel_32( uchar( r2 ), uchar( g2 ), uchar( b2 ) )
    );
  }

  fclose( input );
}

#endif