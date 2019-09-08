#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "heightfield.h"

class landscape
{
  private:
    long vertex_count;
    long detail_level;
    vertex *vs;

    void load_heights( void );

  public:
    void load( long dl );

    void display_points( void );
    void display_edges( void );

    landscape( const landscape &p ) {  exit_error( "landscape: Aufruf Kopierkonstruktor\n" );  }
    landscape( char *filename );
    landscape( void ) : vertex_count( 0 ), detail_level( 0 ), vs( NULL ) { }
   ~landscape( void ) {  if( vs ) delete [] vs;  }
};

void landscape::load( long dl )
{
  detail_level = dl+1;
  vertex_count = detail_level * detail_level;

  if( (vs = new vertex[ vertex_count ]) == NULL )
    exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long x, y;

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].wx =  x;
	   vs[ y * detail_level + x ].wy =  0;
	   vs[ y * detail_level + x ].wz = -y;
	 }

  double dist = vs[ vertex_count/2 ].wy;

  matrix m;
  m.translate( -0.5*(detail_level-1), 0, 0.5*(detail_level-1) );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];

  load_heights();
}

void landscape::load_heights( void )
{
  long grad = 350;

  heightfield terrain( detail_level, detail_level );
  terrain.plasma_fractal( grad );

  for( long x=0 ; x<vertex_count ; x++ )
     vs[ x ].wy = 0.05 * terrain.heights[ x ];
}

void landscape::display_points( void )
{
  glPointSize( 2 );
  glColor3ub( 255, 255, 255 );

  glBegin( GL_POINTS );

    for( long x=0 ; x<vertex_count ; x++ )
       glVertex3d( vs[ x ].wx, vs[ x ].wy, vs[ x ].wz );

  glEnd();
}

void landscape::display_edges( void )
{
  glColor3ub( 150, 150, 150 );

  long x, y;

  glBegin( GL_LINES );
    for( y=0 ; y<detail_level ; y++ )
       for( x=y*detail_level ; x<(y+1)*detail_level ; x++ )
       {
         if( x < (y+1)*detail_level-1 )
         {
           glVertex3d( vs[ x ].wx, vs[ x ].wy, vs[ x ].wz );
           glVertex3d( vs[ x+1 ].wx, vs[ x+1 ].wy, vs[ x+1 ].wz );
         }

         if( y < detail_level-1 )
         {
           glVertex3d( vs[ x ].wx, vs[ x ].wy, vs[ x ].wz );
           glVertex3d( vs[ x+detail_level ].wx, vs[ x+detail_level ].wy, vs[ x+detail_level ].wz );
         }
       }
  glEnd();
}

#endif