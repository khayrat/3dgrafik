#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "draw_line.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    vertex *wpoint;
    
    void project( void );
    void shape( pixel_32 *sbuffer );
    void rasterize( pixel_32 *sbuffer );

  public:
    vertex pos;

    void update_pos( matrix m );
    void display( pixel_32 *sbuffer );

    polygon( long pc, pixel_32 c, vertex *vs );
   ~polygon( void ) {  if( wpoint ) delete [] wpoint;  }
};

void polygon::project( void )
{
  for( long x=0 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wz <= 0.0 ) exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  
    spoint[ x ].sx = long( wpoint[ x ].wx / wpoint[ x ].wz *  200.0 + x_res/2 );
    spoint[ x ].sy = long( wpoint[ x ].wy / wpoint[ x ].wz * -200.0 + y_res/2 );

    if( spoint[ x ].sx < 0 || spoint[ x ].sx >= x_res || spoint[ x ].sy < 0 || spoint[ x ].sy >= y_res )
      exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  }
}

void polygon::update_pos( matrix m )
{
  pos = m * pos;

  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = m * wpoint[ x ];
}

void polygon::display( pixel_32 *sbuffer )
{
  project();

  rasterize( sbuffer );
}

void set_side( svertex begin, svertex end, long *side )
{
  long length = end.sy - begin.sy;

  if( length <= 0 )
  {
    side[ begin.sy ] = begin.sy * x_res + begin.sx;
    return;
  }

  double a_offset = double( begin.sy * x_res + begin.sx );
  double e_offset = double( end.sy * x_res + end.sx );

  double offset_step = ( e_offset - a_offset ) / length;

  for( long act_y=begin.sy ; act_y <= end.sy ; act_y++ )
  {
    side[ act_y ] = long( a_offset + 0.5 );

    a_offset += offset_step;
  }
}

inline void inc( long *x, long point_count )
{
  (*x)++;

  if( (*x) >= point_count ) *x = 0;
}

inline void dec( long *x, long point_count )
{
  (*x)--;

  if( (*x) < 0 ) *x = point_count-1;
}

void polygon::rasterize( pixel_32 *sbuffer )
{
  long x, y, top=0, bottom=0;

  for( x=1 ; x<point_count ; x++ )
  {
    if( spoint[ top    ].sy > spoint[ x ].sy ) top = x;
    if( spoint[ bottom ].sy < spoint[ x ].sy ) bottom = x;
  }

  x = y = top;
  for( dec( &y, point_count ) ; x!=bottom ; x=y, dec( &y, point_count ) )
     set_side( spoint[ x ], spoint[ y ], left_side );

  x = y = top;
  for( inc( &y, point_count ) ; x!=bottom ; x=y, inc( &y, point_count ) )
     set_side( spoint[ x ], spoint[ y ], right_side );

  for( y = spoint[ top ].sy ; y <= spoint[ bottom ].sy ; y++ )
     for( x = left_side[ y ] ; x <= right_side[ y ] ; x++ )
        sbuffer[ x ] = color;
}

void polygon::shape( pixel_32 *sbuffer )
{
  long x, y;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, sbuffer );
}

polygon::polygon( long pc, pixel_32 c, vertex *vs )
{
  point_count = pc;  color = c;

  if( point_count >= largest_point_count )
    exit_error( "polygon::polygon(): point_count größer als largest_point_count.\n" );

  if( (wpoint = new vertex[ point_count ]) == NULL )
    exit_error( "polygon::polygon(): Fehler bei der Reservierung von Arbeitspeicher.\n" );

  long x;

  for( x=0 ; x<point_count ; x++ ) wpoint[ x ] = vs[ x ];

  double xs, xg, ys, yg, zs, zg;  
  xs = xg = wpoint[ 0 ].wx;  ys = yg = wpoint[ 0 ].wy;  zs = zg = wpoint[ 0 ].wz;
    
  for( x=1 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wx < xs ) xs = wpoint[ x ].wx;  if( wpoint[ x ].wx > xg ) xg = wpoint[ x ].wx;
    if( wpoint[ x ].wy < ys ) ys = wpoint[ x ].wy;  if( wpoint[ x ].wy > yg ) yg = wpoint[ x ].wy;
    if( wpoint[ x ].wz < zs ) zs = wpoint[ x ].wz;  if( wpoint[ x ].wz > zg ) zg = wpoint[ x ].wz;
  }
  
  pos.wx = (xs + xg) / 2;
  pos.wy = (ys + yg) / 2;
  pos.wz = (zs + zg) / 2;

  matrix m;
  m.translate( -pos.wx, -pos.wy, -pos.wz );
  update_pos( m );
}

#endif