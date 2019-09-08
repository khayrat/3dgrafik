#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "screen_interface.h"
#include "global_definitions.h"
#include "matrix.h"
#include "palette.h"

class polygon
{
  private:
    long point_count;
    long color_offset;

    long *points;
    vector normal;

  public:
    void load( FILE *input, vertex *vs );
    void update_pos( matrix m ) {  normal = m * normal;  }
    void display( vertex *vs );

    polygon( void ) : point_count( 0 ), color_offset( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

double get_color( vector normal )
{
  double t = dot( normal, light ) / (normal.length * light.length);
    
  double c = -t * (used_colors.range_size - 1);
  if( c > 0 ) return c;
  
  return 0;
}

void polygon::display( vertex *vs )
{
  long c = long( get_color( normal ) );
  pixel_32 color = used_colors.colors[ color_offset ].components[ c ];

  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = hvertex( vs[ points[ x ] ], color );

  screen_interface.draw_polygon( point_count, wpoint );
}

void polygon::load( FILE *input, vertex *vs )
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

  color_offset = rand() % 2;

  vector a = vs[ points[ 1 ] ] - vs[ points[ 0 ] ];  
  vector b = vs[ points[ point_count-1 ] ] - vs[ points[ 0 ] ];

  normal = cross( a, b );
}

#endif