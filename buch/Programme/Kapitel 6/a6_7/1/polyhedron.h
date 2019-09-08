#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polygon.h"

class polyhedron
{
  private:
    long vertex_count, polygon_count;
    long detail_level;
    vertex *vs;
    polygon *ps;

    void initialise_vertices( long dl );

  public:
    vertex pos;

    void plane( long dl );
    void update_pos( matrix m );
    void display_points( void );

    polyhedron( const polyhedron &p ) {  exit_error( "polyhedron: Aufruf Kopierkonstruktor\n" );  }
    polyhedron( char *filename );
    polyhedron( void ) : vertex_count( 0 ), polygon_count( 0 ), detail_level( 0 ), vs( NULL ), ps( NULL ) { }
   ~polyhedron( void ) {  if( vs ) delete [] vs;  if( ps ) delete [] ps;  }
};

void polyhedron::initialise_vertices( long dl )
{
  detail_level = dl+1;
  vertex_count = detail_level * detail_level;

  if( (vs = new vertex[ vertex_count ]) == NULL )
	exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<vertex_count ; x++ ) vs[ x ] = vertex( 0, 0, 0 );

  pos = vertex( 0, 0, 0 );
}

void polyhedron::plane( long dl )
{
  initialise_vertices( dl );

  long x, y;

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].wx =  x;
	   vs[ y * detail_level + x ].wy =  0;
	   vs[ y * detail_level + x ].wz = -y;
	 }

  matrix m;
  m.translate( -0.5*(detail_level-1), 0, 0.5*(detail_level-1) );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
}

void polyhedron::update_pos( matrix m )
{
  pos = m * pos;

  long x;

  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
  for( x=0 ; x<polygon_count ; x++ ) ps[ x ].update_pos( m );
}

void polyhedron::display_points( void )
{
  glPointSize( 8 );
  glColor3ub( 255, 255, 255 );

  glBegin( GL_POINTS );

    for( long x=0 ; x<vertex_count ; x++ )
       glVertex3d( vs[ x ].wx, vs[ x ].wy, vs[ x ].wz );

  glEnd();
}

#endif