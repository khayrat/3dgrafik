#ifndef WIREFRAME_H
#define WIREFRAME_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"

struct line
{
  long begin, end;
  pixel_32 color;
};

class wireframe
{
  protected:
    long vertex_count, line_count;
    vertex *vs;
    line *es;

  public:
    local_system ls;
  
    void load( char *filename );

    void update_pos( matrix m ) {  ls = m * ls;  }
    void display( void );

    wireframe( void ) : vertex_count( 0 ), line_count( 0 ), vs( NULL ), es( NULL ) { }
    wireframe( const wireframe &wf ) {  exit_error( "wireframe: Aufruf Kopierkonstruktor.\n" );  }
   ~wireframe( void ) {  if( vs ) delete [] vs;  if( es ) delete [] es;  }
};

void wireframe::display( void )
{
  matrix m;
  m.columns( ls.x_axis, ls.y_axis, ls.z_axis );
  m.translate( ls.pos.wx, ls.pos.wy, ls.pos.wz );

  glBegin( GL_LINES );

    for( long x=0 ; x<line_count ; x++ )
    {
      glColor3ub( es[ x ].color.red, es[ x ].color.green, es[ x ].color.blue );

      vertex begin = m * vs[ es[ x ].begin ];
      vertex end = m * vs[ es[ x ].end ];

      glVertex3d( begin.wx, begin.wy, begin.wz );
      glVertex3d( end.wx, end.wy, end.wz );
    }

  glEnd();
}

void wireframe::load( char *filename )
{
  FILE *input;

  if( (input = fopen( filename, "rb" )) == NULL )
    exit_nofile( "wireframe::wireframe()", filename );

  {
    fscanf( input, "%ld", &vertex_count );

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