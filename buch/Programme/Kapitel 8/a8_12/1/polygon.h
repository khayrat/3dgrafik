#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"
#include "texture.h"

class polygon
{
  private:
    long point_count;
    texture surface;

    point *points;

  public:
    void load( FILE *input );
    void display( vertex *vs );

    polygon( void ) : point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::display( vertex *vs )
{
  surface.activate();

  glBegin( GL_POLYGON );

    for( long x=0 ; x<point_count ; x++ )
    {
      tx_translation += 0.001;

      glTexCoord2d( points[ x ].tx*tx_scale + tx_translation, points[ x ].ty*ty_scale + ty_translation );

      vertex pos = vs[ points[ x ].vertex_offset ];

      glVertex3d( pos.wx, pos.wy, pos.wz );
    }

  glEnd();
}

void polygon::load( FILE *input )
{
  fscanf( input, "%ld", &point_count );

  if( (points = new point[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  if( point_count != 4 ) exit_error( "polygon::load: Es werden lediglich viereckige Polygone unterstützt.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ].vertex_offset );

  char filename[ 500 ];
  fscanf( input, "%s", filename );
  surface.load( filename );

  points[ 0 ].tx = 0;  points[ 0 ].ty = 0;
  points[ 1 ].tx = 1;  points[ 1 ].ty = 0;
  points[ 2 ].tx = 1;  points[ 2 ].ty = 1;
  points[ 3 ].tx = 0;  points[ 3 ].ty = 1;
}

#endif