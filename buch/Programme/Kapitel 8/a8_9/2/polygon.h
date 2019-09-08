#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "screen_interface.h"
#include "global_definitions.h"

class polygon
{
  private:
    long point_count;
    long tx_offset;

    point *points;

  public:
    void load( FILE *input );
    void display( vertex *vs );

    polygon( void ) : point_count( 0 ), tx_offset( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::display( vertex *vs )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = hvertex( vs[ points[ x ].offset ], points[ x ].tx, points[ x ].ty );

  screen_interface.draw_polygon( point_count, wpoint, tx_offset );
}

void polygon::load( FILE *input )
{
  fscanf( input, "%ld", &point_count );

  if( point_count < 3 || point_count >= largest_point_count )
      exit_error( "polygon::load(): point_count besitzt einen ung�ltigen Wert.\n" );

  if( (points = new point[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler w�hrend der Reservierung von Arbeitsspeicher.\n" );

  if( point_count != 4 ) exit_error( "Es werden lediglich viereckige Polygone unterst�tzt.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ].offset );

  fscanf( input, "%ld", &tx_offset );

  points[ 0 ].tx = 0;    points[ 0 ].ty = 0;
  points[ 1 ].tx = 1;    points[ 1 ].ty = 0;
  points[ 2 ].tx = 1;    points[ 2 ].ty = 1;
  points[ 3 ].tx = 0;    points[ 3 ].ty = 1;
}

#endif