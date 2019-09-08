#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"

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
  glColor3ub( color.red, color.green, color.blue );

  glBegin( GL_POLYGON );

    for( long x=0 ; x<point_count ; x++ )
    {
      vertex pos = vs[ points[ x ] ];

      glVertex3d( pos.wx, pos.wy, pos.wz );
    }

  glEnd();
}

void polygon::load( FILE *input )
{
  fscanf( input, "%ld", &point_count );

  if( (points = new long[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ] );

  long red, green, blue;
  fscanf( input, "%ld %ld %ld", &red, &green, &blue );
  color = pixel_32( uchar( red ), uchar( green ), uchar( blue ) );
}

#endif