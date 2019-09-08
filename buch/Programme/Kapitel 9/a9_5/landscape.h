#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "heightfield.h"
#include "texture.h"
#include "primary_color.h"

class landscape
{
  private:
    long vertex_count;
    long detail_level;
    vertex *vs;

    texture tx;

    void load_heights( void );
    void draw_square( long offset );

  public:
    void load( long dl );

    void display_points( void );
    void display_edges( void );
    void display_polygons( void );

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

       vs[ y * detail_level + x ].tx = x * (8.0 / (detail_level-1));
       vs[ y * detail_level + x ].ty = y * (8.0 / (detail_level-1));
	 }

  matrix m;
  m.translate( -0.5*(detail_level-1), 0, 0.5*(detail_level-1) );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];

  tx.load( "tx.bmp" );

  load_heights();
}

void landscape::load_heights( void )
{
  pixel_32 borders[ 5 ] = 
  {
    pixel_32( 10, 60, 0 ), pixel_32( 80, 130, 50 ), pixel_32( 140, 170, 90 ),
    pixel_32( 160, 160, 190 ), pixel_32( 255, 255, 255 )
  };
  primary_color hc;  hc.load( 256, 5, borders );

  long grad = 350;

  heightfield terrain( detail_level, detail_level );
  terrain.plasma_fractal( grad );

  for( long x=0 ; x<vertex_count ; x++ )
  {
    vs[ x ].wy = 0.15 * terrain.heights[ x ];
    vs[ x ].color = hc.components[ terrain.heights[ x ] ];
  }
}

void landscape::display_points( void )
{
  glPointSize( 2 );
  glColor3ub( 150, 150, 150 );

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

void landscape::draw_square( long offset )
{
  long pos;

  glBegin( GL_TRIANGLE_STRIP );
    pos = offset + detail_level;
    glColor3ub( vs[ pos ].color.red, vs[ pos ].color.green, vs[ pos ].color.blue );
    glTexCoord2d( vs[ pos ].tx, vs[ pos ].ty );
    glVertex3d( vs[ pos ].wx, vs[ pos ].wy, vs[ pos ].wz );

    pos = offset;
    glColor3ub( vs[ pos ].color.red, vs[ pos ].color.green, vs[ pos ].color.blue );
    glTexCoord2d( vs[ pos ].tx, vs[ pos ].ty );
    glVertex3d( vs[ pos ].wx, vs[ pos ].wy, vs[ pos ].wz );

    pos = offset + detail_level + 1;
    glColor3ub( vs[ pos ].color.red, vs[ pos ].color.green, vs[ pos ].color.blue );
    glTexCoord2d( vs[ pos ].tx, vs[ pos ].ty );
    glVertex3d( vs[ pos ].wx, vs[ pos ].wy, vs[ pos ].wz );

    pos = offset + 1;
    glColor3ub( vs[ pos ].color.red, vs[ pos ].color.green, vs[ pos ].color.blue );
    glTexCoord2d( vs[ pos ].tx, vs[ pos ].ty );
    glVertex3d( vs[ pos ].wx, vs[ pos ].wy, vs[ pos ].wz );
  glEnd();
}

void landscape::display_polygons( void )
{
  glEnable( GL_TEXTURE_2D );
  tx.activate();

  long begin_offset = 0;

  for( long y=0 ; y<detail_level-1 ; y++ )
  {
    for( long x=0 ; x<detail_level-1 ; x++ )
       draw_square( begin_offset + x );

    begin_offset += detail_level;
  }

  glDisable( GL_TEXTURE_2D );
}

#endif