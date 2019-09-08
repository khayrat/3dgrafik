#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"

class point_cloud
{
  private:
    long vertex_count;
    vertex *vs;

  public:
    vertex pos;
  
    void chaos_game( long detail_level );
    void update_pos( matrix m );
    void display( pixel_32 *sbuffer );

    point_cloud( void ) : vertex_count( 0 ), vs( NULL ) { }    
    point_cloud( const point_cloud &p ) {  exit_error( "point_cloud: Aufruf kopierkonstruktor.\n" );  }
   ~point_cloud( void ) {  if( vs ) delete [] vs;  }
};

void point_cloud::chaos_game( long detail_level )
{
  vertex_count = detail_level;

  if( (vs = new vertex[ vertex_count ]) == NULL )
	exit_error( "*vs: Not enough memory.\n" );
  
  vs[ 0 ] = vertex(  1,  1, -1 );
  vs[ 1 ] = vertex(  1, -1,  1 );
  vs[ 2 ] = vertex( -1, -1, -1 );
  vs[ 3 ] = vertex( -1,  1,  1 );
  
  vertex act_point = vs[ rand() % 4 ];

  for( long x=4 ; x<vertex_count ; x++ )
  {
    uchar index = rand() % 4;

    act_point.wx = ( act_point.wx + vs[ index ].wx ) / 2;
    act_point.wy = ( act_point.wy + vs[ index ].wy ) / 2;
    act_point.wz = ( act_point.wz + vs[ index ].wz ) / 2;

    vs[ x ] = act_point;
  }

  pos = vertex( 0, 0, 0 );
}

void point_cloud::update_pos( matrix m )
{
  pos = m * pos;

  for( long x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
}

void display_point( vertex wp, pixel_32 *screen )
{
  svertex sp;

  if( wp.wz > 0.0 )
  {
    sp.sx = long( wp.wx / wp.wz *  pr_cnst + x_res/2 );
    sp.sy = long( wp.wy / wp.wz * -pr_cnst + y_res/2 );
  
    if( sp.sx > 0 && sp.sx < x_res && sp.sy > 0 && sp.sy < y_res )
    {
      long offset = sp.sy * x_res + sp.sx;
      double sz = wp.wz + clear_translation;

      if( sz > zbuffer[ offset ] ) return;

      screen[ offset ] = pixel_32( 255, 255, 255 );
      zbuffer[ offset ] = sz;
    }
  }
}

void point_cloud::display( pixel_32 *sbuffer )
{
  for( long x=0 ; x<vertex_count ; x++ )
     display_point( vs[ x ], sbuffer );
}

#endif