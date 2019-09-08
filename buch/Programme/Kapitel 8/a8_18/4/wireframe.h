#ifndef WIREFRAME_H
#define WIREFRAME_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "draw_line.h"

struct line
{
  long begin, end;
  pixel_32 color;
};

class wireframe
{
  private:
    long vertex_count, line_count;
    vertex *vs;
    line *es;

  public:
    local_system ls;
  
    void update_pos( matrix m ) {  ls = m * ls;  }
    void display( pixel_32 *sbuffer );
    
    wireframe( char *filename );
    wireframe( const wireframe &wf ) {  exit_error( "wireframe: Aufruf Kopierkonstruktor.\n" );  }
   ~wireframe( void ) {  if( vs ) delete [] vs;  if( es ) delete [] es;  }
};

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

void wireframe::display( pixel_32 *sbuffer )
{
  vertex w_begin, w_end;
  svertex s_begin, s_end;

  matrix m;
  m.columns( ls.x_axis, ls.y_axis, ls.z_axis );
  m.translate( ls.pos.wx, ls.pos.wy, ls.pos.wz );

  matrix n;
  n.translate( -camera.pos.wx, -camera.pos.wy, -camera.pos.wz );
  n.rows( camera.x_axis, camera.y_axis, camera.z_axis );

  for( long x=0 ; x<line_count ; x++ )
  {
    w_begin = n * m * vs[ es[ x ].begin ];
    w_end = n * m * vs[ es[ x ].end ];

    if( project_point( w_begin, &s_begin ) && project_point( w_end, &s_end ) )
      draw_line( s_begin, s_end, es[ x ].color, sbuffer );
  }
}

wireframe::wireframe( char *filename )
{
  FILE *input;

  if( (input = fopen( filename, "rb" )) == NULL )
    exit_nofile( "wireframe::wireframe()", filename );

  {
    fscanf( input, "%ld", &vertex_count );

    if( vertex_count >= largest_point_count )
      exit_error( "vertex_count ist groesser oder gleich largest_point_count.\n" );

    if( (vs = new vertex[ vertex_count ]) == NULL ) 
      exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    for( long x=0 ; x<vertex_count ; x++ )
       fscanf( input, "%lf %lf %lf", &vs[ x ].wx, &vs[ x ].wy, &vs[ x ].wz );
  }

  {
    fscanf( input, "%ld", &line_count );

    if( (es = new line[ line_count ]) == NULL )
      exit_error( "line_count: Fehler bei der Reservierung von Arbeisttpeicher.\n" );

    for( long x=0 ; x<line_count ; x++ )
    {
      fscanf( input, "%ld %ld", &es[ x ].begin, &es[ x ].end );

      long red, green, blue;
      fscanf( input, "%ld %ld %ld", &red, &green, &blue );
      es[ x ].color = pixel_32( uchar( red ), uchar( green ), uchar( blue ) );
    }
  }

  fclose( input );

  {
    double xs, xg, ys, yg, zs, zg;  
    xs = xg = vs[ 0 ].wx;  ys = yg = vs[ 0 ].wy;  zs = zg = vs[ 0 ].wz;
    
    for( long x=0 ; x<vertex_count ; x++ )
    {
      if( vs[ x ].wx < xs ) xs = vs[ x ].wx;  if( vs[ x ].wx > xg ) xg = vs[ x ].wx;
      if( vs[ x ].wy < ys ) ys = vs[ x ].wy;  if( vs[ x ].wy > yg ) yg = vs[ x ].wy;
      if( vs[ x ].wz < zs ) zs = vs[ x ].wz;  if( vs[ x ].wz > zg ) zg = vs[ x ].wz;
    }
  
    vertex pos;
    pos.wx = (xs + xg) / 2;
    pos.wy = (ys + yg) / 2;
    pos.wz = (zs + zg) / 2;

    matrix m;
    m.translate( -pos.wx, -pos.wy, -pos.wz );
    update_pos( m );
  }
}

#endif