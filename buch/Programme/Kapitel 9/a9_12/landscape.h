#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "heightfield.h"
#include "texture.h"
#include "primary_color.h"
#include "viewport.h"

class landscape
{
  private:
    long vertex_count;
    long detail_level;
    vertex *vs;

    double border;
    texture tx;

    void load_heights( void );
    void draw_square( long offset );

  public:
    void load( long dl, double length );
    void adjust_pos( local_system *ls );

    void display( viewport *user );

    void display_points( void );
    void display_edges( void );
    void display_polygons( void );

    landscape( const landscape &p ) {  exit_error( "landscape: Aufruf Kopierkonstruktor\n" );  }
    landscape( char *filename );
    landscape( void ) : vertex_count( 0 ), detail_level( 0 ), vs( NULL ) { }
   ~landscape( void ) {  if( vs ) delete [] vs;  }
};

void landscape::adjust_pos( local_system *ls )
{
  if( ls->pos.wx < -border ) ls->pos.wx = -border;
  if( ls->pos.wx >  border ) ls->pos.wx =  border;
  if( ls->pos.wz < -border ) ls->pos.wz = -border;
  if( ls->pos.wz >  border ) ls->pos.wz =  border;
}

void landscape::load( long dl, double length )
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

  if( length > (detail_level-1)/2 )
    exit_error( "Viewport ist zu groß für die gegebene Landschaft.\n" );

  border = vs[ detail_level-1 ].wx - length;
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
//    vs[ x ].wy = 0.05 * terrain.heights[ x ];
    vs[ x ].color = hc.components[ terrain.heights[ x ] ];
  }
}

long convex_hull( vertex *points, long pc, vertex *hull )
{
  points[ 0 ].wy = 0;
  hull[ 0 ] = points[ 0 ];
  long left_index = 0;

  for( long x=1 ; x<pc ; x++ )
  {
    points[ x ].wy = 0;

    if( points[ x ].wx < hull[ 0 ].wx )
    {
      hull[ 0 ] = points[ x ];
      left_index = x;
    }
  }

  vector v = vector( 0, 0, -100 );
  long act;

  for( act = 0 ; act < pc-1 ; act++ )
  {
    long index = 0;
    double least_cos = 1;

    for( long x=0 ; x<pc ; x++ )
    {
      vector new_v = points[ x ] - hull[ act ];

      if( new_v.length < 0.00001 ) continue;

      double cos_alpha = dot( v, new_v ) / (v.length * new_v.length);
      if( cos_alpha <= least_cos )
      {
        index = x;
        least_cos = cos_alpha;
      }
    }

    if( index == left_index ) break;

    hull[ act+1 ] = points[ index ];

    v = hull[ act ] - hull[ act+1 ];
  }

  return act + 1;
}

void landscape::display( viewport *user )
{
  vertex points[ 8 ];
  vertex hull[ 8 ];

  user->get_points( points );
  long convex_count = convex_hull( points, 8, hull );

  glColor3ub( 255, 0, 0 );
  glBegin( GL_LINE_LOOP );
    glVertex3d( -border, 0.1,  border );
    glVertex3d(  border, 0.1,  border );
    glVertex3d(  border, 0.1, -border );
    glVertex3d( -border, 0.1, -border );
  glEnd();

  long x;

  glPointSize( 8 );
  glColor3ub( 180, 180, 180 );
  glBegin( GL_POINTS );
    for( x=0 ; x<8 ; x++ ) glVertex3d( points[ x ].wx, points[ x ].wy, points[ x ].wz );
  glEnd();

  glColor3ub( 255, 255, 0 );
  glBegin( GL_LINE_LOOP );
    for( x=0 ; x<convex_count ; x++ ) glVertex3d( hull[ x ].wx, hull[ x ].wy, hull[ x ].wz );
  glEnd();
} 

void landscape::display_points( void )
{
  glPointSize( 1 );
  glColor3ub( 255, 255, 255 );

  long x;

  glBegin( GL_POINTS );

    for( x=0 ; x<vertex_count ; x++ )
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