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
    long *side[ 2 ];
    void set_square( long sx, long sz, char s );
    void set_side( svertex begin, svertex end, char s );

    void load_heights( long dl, long length );
    void draw_square( long offset );
    void rasterize( long point_count, svertex *spoint );

  public:
    double load( long dl, long length );
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
  if( ls->pos.wx <= -border ) ls->pos.wx += 2*border;
  if( ls->pos.wx >=  border ) ls->pos.wx -= 2*border;
  if( ls->pos.wz <= -border ) ls->pos.wz += 2*border;
  if( ls->pos.wz >=  border ) ls->pos.wz -= 2*border;
}

double landscape::load( long dl, long length )
{
  if( dl < 3 ) exit_error( "landscape::load(): Der Mindestwert des Parameters dl ist 3.\n" );

  detail_level = dl*length + 2*length;
  vertex_count = detail_level * detail_level;

  if
  ( 
    (vs = new vertex[ vertex_count ]) == NULL ||
    (side[ 0 ] = new long[ detail_level ]) == NULL ||
    (side[ 1 ] = new long[ detail_level ]) == NULL
  )
  exit_error( "landscape::load(): Fehler während der Reservierung von Arbeitsspeicher.\n" );

  long x, y;

  for( y=0 ; y<detail_level ; y++ )
     for( x=0 ; x<detail_level ; x++ )
	 {
       vs[ y * detail_level + x ].wx =  x;
	   vs[ y * detail_level + x ].wy =  0;
	   vs[ y * detail_level + x ].wz = -y;

       vs[ y * detail_level + x ].tx = x * ( double( dl+2 ) / (detail_level-1));
       vs[ y * detail_level + x ].ty = y * ( double( dl+2 ) / (detail_level-1));
	 }

  matrix m;
  m.translate( -0.5*(detail_level-1), 0, 0.5*(detail_level-1) );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];

  tx.load( "tx.bmp" );

  load_heights( dl*length, length );

  border = vs[ detail_level-1 ].wx - length;

  return vs[ vertex_count/2 ].wy + 50;
}

void landscape::load_heights( long dl, long length )
{
  pixel_32 borders[ 5 ] = 
  {
    pixel_32( 10, 60, 0 ), pixel_32( 80, 130, 50 ), pixel_32( 140, 170, 90 ),
    pixel_32( 160, 160, 190 ), pixel_32( 255, 255, 255 )
  };
  primary_color hc;  hc.load( 256, 5, borders );

  long grad = 300;

  heightfield terrain( dl, dl );
  terrain.seamless_plasma( grad );
  terrain.enlarge( length );

  for( long x=0 ; x<vertex_count ; x++ )
  {
    vs[ x ].wy = 0.6 * terrain.heights[ x ];
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

void landscape::set_square( long sx, long sz, char s )
{
  long offset = sz * detail_level + sx;

  if( s == 0 && offset < side[ s ][ sz ] ) side[ s ][ sz ] = offset;
  if( s == 1 && offset > side[ s ][ sz ] ) side[ s ][ sz ] = offset;
}

void landscape::set_side( svertex begin, svertex end, char s )
{
  long add = 0;
  if( (begin.wx >= end.wx && s == 0) || (begin.wx <= end.wx && s == 1) )
    add = -1;

  if( begin.sz == end.sz )
  {
	if( s == 0 ) set_square( min( begin.sx, end.sx ), begin.sz, s );
	else set_square( max( begin.sx, end.sx ), begin.sz, s );
    return;
  }

  set_square( begin.sx, begin.sz, s );
  set_square( end.sx, end.sz, s );

  double act_wz = vs[ 0 ].wz - (begin.sz + 1);
  vector dir = vector( end.wx - begin.wx, 0, end.wz - begin.wz );

  for( long sz = begin.sz+1 ; sz <= end.sz ; sz++ )
  {
	double t = (act_wz - begin.wz) / dir.z;
    long sx = long( (begin.wx + t * dir.x) + 0.5*(detail_level-1) );
    set_square( sx, (sz+add), s );

    act_wz--;
  }
}

inline void inc( long *x, long point_count )
{
  if( ++(*x) >= point_count ) *x = 0;
}

inline void dec( long *x, long point_count )
{
  if( --(*x) < 0 ) *x = point_count-1;
}

void landscape::rasterize( long convex_count, svertex *spoint )
{
  long x, y, top=0, bottom=0;

  for( x=1 ; x<convex_count ; x++ )
  {
    if( spoint[ top    ].wz < spoint[ x ].wz ) top = x;
    if( spoint[ bottom ].wz > spoint[ x ].wz ) bottom = x;
  }

  long sz;

  for( sz = spoint[ top ].sz ; sz <= spoint[ bottom ].sz ; sz++ )
  {
    side[ 0 ][ sz ] = (sz+1) * detail_level - 1;
    side[ 1 ][ sz ] = sz * detail_level;
  }

  x = y = top;
  for( dec( &y, convex_count ) ; x!=bottom ; x=y, dec( &y, convex_count ) )
     set_side( spoint[ x ], spoint[ y ], 0 );

  x = y = top;
  for( inc( &y, convex_count ) ; x!=bottom ; x=y, inc( &y, convex_count ) )
     set_side( spoint[ x ], spoint[ y ], 1 );

  glEnable( GL_TEXTURE_2D );

    tx.activate();

    for( sz = spoint[ top ].sz ; sz <= spoint[ bottom ].sz ; sz++ )
       for( long offset = side[ 0 ][ sz ] ; offset <= side[ 1 ][ sz ] ; offset++ )
          draw_square( offset );

  glDisable( GL_TEXTURE_2D );
}

void landscape::display( viewport *user )
{
  vertex points[ 8 ];
  vertex hull[ 8 ];

  user->get_points( points );
  long convex_count = convex_hull( points, 8, hull );

  long x;
  svertex spoint[ 8 ];

  for( x=0 ; x<convex_count ; x++ )
  {
    spoint[ x ].wx = hull[ x ].wx;
    spoint[ x ].wz = hull[ x ].wz;

    spoint[ x ].sx = long(  hull[ x ].wx + 0.5*(detail_level-1) );
    spoint[ x ].sz = long( -hull[ x ].wz + 0.5*(detail_level-1) );
  }

  rasterize( convex_count, spoint );
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