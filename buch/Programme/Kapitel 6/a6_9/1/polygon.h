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
    void load( long a, long b, long c, long d, vertex *vs );
    void update_pos( matrix m ) {  normal = m * normal;  }
    void display( vertex *vs );

    polygon( const polygon &p ) {  exit_error( "polygon: Aufruf Kopierkonstruktor.\n" );  }
    polygon( void ) : point_count( 0 ), color_offset( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }

  friend class polyhedron;
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

void polygon::load( long a, long b, long c, long d, vertex *vs )
{
  point_count = 4;
  if( (points = new long[ point_count ]) == NULL )
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  points[ 0 ] = a;  points[ 1 ] = b;
  points[ 2 ] = c;  points[ 3 ] = d;

  color_offset = rand() % 2;
  
  vector r = vs[ points[ 1 ] ] - vs[ points[ 0 ] ]; 
  if( r.length == 0 ) r = vs[ points[ 2 ] ] - vs[ points[ 0 ] ]; 
  vector s = vs[ points[ point_count-1 ] ] - vs[ points[ 0 ] ];

  normal = cross( r, s );
}

#endif