#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "screen_interface.h"
#include "global_definitions.h"
#include "hardware_palette.h"
#include "matrix.h"

class polygon
{
  private:
    long point_count;
    long color;
    vector normal;

    long *points;

  public:
    void update_pos( matrix m ) {  normal = m * normal;  }
    void load( FILE *input, vertex *vs );
    void display( vertex *vs );

    polygon( void ) : point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }

  friend class polyhedron;
};

void polygon::display( vertex *vs )
{
  long z=0;

  for( long x=1 ; x<point_count-1 ; x++ )
  {
    wpoint[ z++ ] = vs[ points[ 0 ] ];
    wpoint[ z++ ] = vs[ points[ x ] ];
    wpoint[ z++ ] = vs[ points[ x+1 ] ];
  }

  screen_interface.set_material( &palette.materials[ color ] );
  screen_interface.draw_polygon( 3*(point_count-2), wpoint );
}

void polygon::load( FILE *input, vertex *vs )
{
  {
    fscanf( input, "%ld", &point_count );

    if( point_count >= largest_point_count )
        exit_error( "point_count ist größer oder gleich largest_point_count.\n" );

    if( (points = new long[ point_count ]) == NULL ) 
      exit_error( "*points: Fehler während der Reservierung von Arbeitsspeicher.\n" );

    for( long x=0 ; x<point_count ; x++ )
       fscanf( input, "%ld", &points[ x ] );

    fscanf( input, "%ld", &color );

    color = rand() % 2 + 1;

    if( color >= palette.material_count )
      exit_error( "polygon::load(): color besitzt einen unzulässigen Wert.\n" );
  }

  {
    vector a = vs[ points[ 1 ] ] - vs[ points[ 0 ] ]; 
    vector b = vs[ points[ point_count-1 ] ] - vs[ points[ 0 ] ];

    normal = cross( a, b );
    normal.set_length( 1 );
  }
}

#endif