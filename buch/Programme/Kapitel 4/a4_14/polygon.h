#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "screen_interface.h"
#include "global_definitions.h"
#include "matrix.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    long *points;

  public:
    void load( FILE *input );
    void display( vertex *vs );

    polygon( void ) : point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::display( vertex *vs )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = hvertex( vs[ points[ x ] ], color );

  screen_interface.draw_polygon( point_count, wpoint );
}

void polygon::load( FILE *input )
{
  fscanf( input, "%ld", &point_count );

  if( point_count >= largest_point_count )
      exit_error( "point_count ist größer oder gleich largest_point_count.\n" );

  if( (points = new long[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler während der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ] );

  long red, green, blue;
  fscanf( input, "%ld %ld %ld", &red, &green, &blue );
  color = pixel_32( uchar( red ), uchar( green ), uchar( blue ) );
}

#endif