#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"
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

    polygon( const polygon &p ) {  exit_error( "polygon: Aufruf Kopierkonstruktor.\n" );  }
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
  glColor3ub( color.red, color.green, color.blue );

  glBegin( GL_POLYGON );

    for( long x=0 ; x<point_count ; x++ )
    {
      vertex pos = vs[ points[ x ] ];

      glVertex3d( pos.wx, pos.wy, pos.wz );
    }

  glEnd();
}

void polygon::load( FILE *input, vertex *vs )
{
  fscanf( input, "%ld", &point_count );

  if( (points = new long[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ] );

  fscanf( input, "%ld", &color_offset );

  vector a = vs[ points[ 1 ] ] - vs[ points[ 0 ] ];  
  vector b = vs[ points[ point_count-1 ] ] - vs[ points[ 0 ] ];

  normal = cross( a, b );
}

#endif