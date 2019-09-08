#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "screen_interface.h"
#include "global_definitions.h"
#include "matrix.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    vertex *points;

  public:
    vertex pos;

    void update_pos( matrix m );
    void display_shape( void );
    void display_polygon( void );

    polygon( long pc, pixel_32 c, vertex *vs );
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::update_pos( matrix m )
{
  pos = m * pos;

  for( long x=0 ; x<point_count ; x++ )
     points[ x ] = m * points[ x ];
}

void polygon::display_shape( void )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = hvertex( points[ x ], color );

  screen_interface.line_loop( point_count, wpoint );
}

void polygon::display_polygon( void )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = hvertex( points[ x ], color );

  screen_interface.draw_polygon( point_count, wpoint );
}

polygon::polygon( long pc, pixel_32 c, vertex *vs )
{
  point_count = pc;  color = c;

  if( point_count >= largest_point_count )
    exit_error( "polygon::polygon(): point_count größer als largest_point_count.\n" );

  if( (points = new vertex[ point_count ]) == NULL )
    exit_error( "*points: Fehler bei der Reservierung von Arbeitspeicher.\n" );

  long x;

  for( x=0 ; x<point_count ; x++ ) points[ x ] = vs[ x ];

  float xs, xg, ys, yg, zs, zg;  
  xs = xg = points[ 0 ].wx;  ys = yg = points[ 0 ].wy;  zs = zg = points[ 0 ].wz;

  for( x=1 ; x<point_count ; x++ )
  {
    if( points[ x ].wx < xs ) xs = points[ x ].wx;  if( points[ x ].wx > xg ) xg = points[ x ].wx;
    if( points[ x ].wy < ys ) ys = points[ x ].wy;  if( points[ x ].wy > yg ) yg = points[ x ].wy;
    if( points[ x ].wz < zs ) zs = points[ x ].wz;  if( points[ x ].wz > zg ) zg = points[ x ].wz;
  }
  
  pos.wx = (xs + xg) / 2;
  pos.wy = (ys + yg) / 2;
  pos.wz = (zs + zg) / 2;

  matrix m;
  m.translate( -pos.wx, -pos.wy, -pos.wz );
  update_pos( m );
}

#endif