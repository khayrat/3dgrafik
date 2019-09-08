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
} used_colors( 256, "palette.pl2" );

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
     colors[ x ].load( range_size, input );

  fclose( input );
}

#endif