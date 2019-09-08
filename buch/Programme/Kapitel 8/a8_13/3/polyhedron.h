#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polygon.h"
#include "bitmap_32.h"

class polyhedron
{
  private:
    long vertex_count, polygon_count, detail_level;
    vertex *vs;
    polygon *ps;

    bitmap_32 surface;

    void initialise_vertices( long dl );
    void rotation_symmetry( void );
    void initialise_polygons( void );

  public:
    vertex pos;

    void plane( long dl, char *filename );
    void sphere( long dl, char *filename );
    void torus( long dl, double radius, char *filename );

    void update_pos( matrix m );
    void display( pixel_32 *sbuffer );
    
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

void polyhedron::initialise_polygons( void )
{
  polygon_count = (detail_level-1)*(detail_level-1);

  if( (ps = new polygon[ polygon_count ]) == NULL )
	exit_error( "*ps: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long z=0;
  for( long y=0 ; y<detail_level-1 ; y++ )
     for( long x=0 ; x<detail_level-1 ; x++ )
     {
       long a = y * detail_level + x;

       ps[ z++ ].load( a, a+1, a+detail_level+1, a+detail_level );
     }
}

void polyhedron::rotation_symmetry( void )
{
  const double pi = 3.1415926535;
  matrix m;
  m.rotate_y( -(2*pi)/(detail_level-1) );

  for( long x=1 ; x<detail_level ; x++ )
     for( long y=0 ; y<detail_level ; y++ )
     {
       vs[ y*detail_level+x ] = vs[ y*detail_level+x-1 ];

       vs[ y*detail_level+x ] = m * vs[ y*detail_level+x ];
     }
}

void polyhedron::sphere( long dl, char *filename )
{
  initialise_vertices( dl );

  const double pi = 3.1415926535;
  matrix m;
  m.rotate_z( pi/(detail_level-1) );

  long x, y;

  vs[ 0 ] = vertex( 0, 1, 0 );
  for( y=1 ; y<detail_level ; y++ )
  {
    vs[ y*detail_level ] = vs[ (y-1)*detail_level ];

    vs[ y*detail_level ] = m * vs[ y*detail_level ];
  }

  rotation_symmetry();

  surface.load( filename );

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].tx = x * ( 6.0 * (surface.xscale-1) / (detail_level-1) );
       vs[ y * detail_level + x ].ty = y * ( 3.0 * (surface.yscale-1) / (detail_level-1) );
	 }

  initialise_polygons();
}

void polyhedron::torus( long dl, double radius, char *filename )
{
  initialise_vertices( dl );

  const double pi = 3.1415926535;
  matrix m;
  m.rotate_z( (2*pi)/(detail_level-1) );

  long x, y;

  vs[ 0 ] = vertex( -1, 0, 0 );
  for( y=1 ; y<detail_level ; y++ )
  {
    vs[ y*detail_level ] = vs[ (y-1)*detail_level ];

    vs[ y*detail_level ] = m * vs[ y*detail_level ];
  }

  m.clear();
  m.translate( -radius, 0, 0 );
  for( y=0 ; y<detail_level ; y++ )
     vs[ y*detail_level ] = m * vs[ y*detail_level ];

  rotation_symmetry();
 
  surface.load( filename );

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].tx = x * ( 16.0 * (surface.xscale-1) / (detail_level-1) );
       vs[ y * detail_level + x ].ty = y * (  4.0 * (surface.yscale-1) / (detail_level-1) );
	 }

  initialise_polygons();
}

void polyhedron::plane( long dl, char *filename )
{
  initialise_vertices( dl );
  surface.load( filename );

  long x, y;

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].wx =  x;
	   vs[ y * detail_level + x ].wy =  0;
	   vs[ y * detail_level + x ].wz = -y;

       vs[ y * detail_level + x ].tx = x * ( 4.0 * (surface.xscale-1) / (detail_level-1) );
       vs[ y * detail_level + x ].ty = y * ( 4.0 * (surface.yscale-1) / (detail_level-1) );
	 }

  matrix m;
  m.translate( -0.5*(detail_level-1), 0, 0.5*(detail_level-1) );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];

  initialise_polygons();
}

void polyhedron::update_pos( matrix m )
{
  pos = m * pos;

  long x;

  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
  for( x=0 ; x<polygon_count ; x++ ) ps[ x ].update_pos( m );
}

void polyhedron::display( pixel_32 *sbuffer )
{
  surface.display( 0, 0, sbuffer );

  for( long x=0 ; x<polygon_count ; x++ ) ps[ x ].display( vs, &surface, sbuffer );
}

#endif