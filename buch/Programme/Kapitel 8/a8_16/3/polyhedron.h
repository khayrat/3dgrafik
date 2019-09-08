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
    long vertex_count, detail_level;
    vertex *vs;

    bitmap_32 surface;
    vector normal;

  public:
    vertex pos;

    void rectangle( long dl, double xscale, double zscale, char *filename );
    void update_pos( matrix m );
    void display_edges( pixel_32 *sbuffer );
    
    polyhedron( const polyhedron &p ) {  exit_error( "polyhedron: Aufruf Kopierkonstruktor\n" );  }
    polyhedron( void ) : vertex_count( 0 ), detail_level( 0 ), vs( NULL ) { }
   ~polyhedron( void ) {  if( vs ) delete [] vs;  }
};

void polyhedron::rectangle( long dl, double xscale, double zscale, char *filename )
{
  surface.load( filename );

  detail_level = dl+1;
  vertex_count = detail_level * 2;

  if( (vs = new vertex[ vertex_count ]) == NULL )
	exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  if( vertex_count >= largest_point_count )
    exit_error( "polyhedron::rectangle(): Der Wert von vertex_count übersteigt largest_vertex_count.\n" );

  long x;

  for( x=0 ; x<detail_level ; x++ )
  {
    double wx = x * xscale / (detail_level - 1);

    vs[ x ] = vertex( wx, 0, 0 );
    vs[ x+detail_level ] = vertex( wx, 0, -zscale );
  }

  matrix m;
  m.translate( -0.5*xscale, 0, 0.5*zscale );
  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];

  pos = vertex( 0, 0, 0 );
  normal = vector( 0, 1, 0 );
}

void polyhedron::update_pos( matrix m )
{
  pos = m * pos;
  normal = m * normal;

  long x;

  for( x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
}

double add = 0;
const double pi = 3.1415926535;

double f( double x, long detail_level )
{  
  double t = x * 2 * pi / (detail_level - 1);

  return 3 * sin( t*2 - add ) + 2 * sin( t - add );
}

uchar project_point( vertex w, svertex *s )
{
  if( w.wz <= z_min ) return 0;

  double inv_z = 1.0 / w.wz;
      
  s->sx = long( w.wx * inv_z *  pr_cnst + x_res / 2 );
  s->sy = long( w.wy * inv_z * -pr_cnst + y_res / 2 );
  s->sz = 1.0 / w.wz;

  if( s->sx < 0 || s->sx >= x_max || s->sy < 0 || s->sy >= y_max ) return 0;
      
  return 1;
}

void draw_line( vertex w_begin, vertex w_end, pixel_32 *sbuffer )
{
  svertex s_begin, s_end;

  if( project_point( w_begin, &s_begin ) && project_point( w_end, &s_end ) )
    draw_line( s_begin, s_end, pixel_32( 255, 255, 255 ), sbuffer );
}

void polyhedron::display_edges( pixel_32 *sbuffer )
{
  draw_line
  (
    vs[ 0 ] + f( 0, detail_level ) * normal,
    vs[ detail_level ] + f( 0, detail_level ) * normal,
    sbuffer
  );

  for( long x=1 ; x<detail_level ; x++ )
  {
    vertex a = vs[ x-1 ] + f( x-1, detail_level ) * normal;
    vertex b = vs[ x ] + f( x, detail_level ) * normal;
    vertex c = vs[ x + detail_level ] + f( x, detail_level ) * normal;
    vertex d = vs[ x-1 + detail_level ] + f( x-1, detail_level ) * normal;

    draw_line( a, b, sbuffer );
    draw_line( b, c, sbuffer );
    draw_line( c, d, sbuffer );
  }

  add += 0.1;  if( add > 2*pi ) add -= 2*pi;
}

#endif