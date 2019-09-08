#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"

class point_cloud
{
  private:
    long detail_level;
    long vertex_count;
    vertex *vs;

    void initialise_vertices( long dl );

  public:
    vertex pos;
  
    void plane( long dl );

    void update_pos( matrix m );
    void display( pixel_32 *sbuffer );
    
    point_cloud( void ) : detail_level( 0 ), vertex_count( 0 ), vs( NULL ) { }
   ~point_cloud( void ) {  if( vs ) delete [] vs;  }
};

void point_cloud::initialise_vertices( long dl )
{
  detail_level = dl+1;
  vertex_count = detail_level * detail_level;

  if( (vs = new vertex[ vertex_count ]) == NULL )
	exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<vertex_count ; x++ ) vs[ x ] = vertex( 0, 0, 0 );

  pos = vertex( 0, 0, 0 );
}

void point_cloud::plane( long dl )
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

void point_cloud::update_pos( matrix m )
{
  pos = m * pos;

  for( long x=0 ; x<vertex_count ; x++ ) vs[ x ] = m * vs[ x ];
}

void point_cloud::display( pixel_32 *sbuffer )
{
  svertex sp;

  for( long x=0 ; x<vertex_count ; x++ )
  {
    vertex wp = vs[ x ];

    if( wp.wz > z_min )
    {
      sp.sx = long( wp.wx / wp.wz *  pr_cnst + x_res/2 );
      sp.sy = long( wp.wy / wp.wz * -pr_cnst + y_res/2 );
  
      if( sp.sx > 0 && sp.sx < x_res && sp.sy > 0 && sp.sy < y_res )
      {
        long offset = sp.sy * x_res + sp.sx;
        double sz = 1.0 / wp.wz;

        if( sz < zbuffer[ offset ] ) return;

        sbuffer[ offset ] = pixel_32( 255, 255, 255 );
        zbuffer[ offset ] = sz;
      }
    }
  }
}

#endif