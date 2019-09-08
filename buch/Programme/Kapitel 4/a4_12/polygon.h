#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"
#include "draw_line.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    long *points;

    void project( void );

    void shape( pixel_32 *sbuffer );
    void rasterize( pixel_32 *sbuffer );

  public:
    void load( FILE *input );
    void display( vertex *vs, pixel_32 *sbuffer );

    polygon( void ) : point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::project( void )
{
  for( long x=0 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wz <= 0.0 ) exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  
    spoint[ x ].sx = long( wpoint[ x ].wx / wpoint[ x ].wz *  800.0 + x_res/2 );
    spoint[ x ].sy = long( wpoint[ x ].wy / wpoint[ x ].wz * -800.0 + y_res/2 );
    spoint[ x ].sz = wpoint[ x ].wz + clear_translation;

    if( spoint[ x ].sx < 0 || spoint[ x ].sx >= x_res || spoint[ x ].sy < 0 || spoint[ x ].sy >= y_res )
      exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  }
}

void polygon::display( vertex *vs, pixel_32 *sbuffer )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = vs[ points[ x ] ];

  project();

  rasterize( sbuffer );
}

void set_side( svertex begin, svertex end, screen_side *side )
{
  long length = end.sy - begin.sy;

  if( length <= 0 )
  {
    side[ begin.sy ].offset = begin.sy * x_res + begin.sx;
    return;
  }

  double a_offset = double( begin.sy * x_res + begin.sx );
  double e_offset = double( end.sy * x_res + end.sx );

  double offset_step = ( e_offset - a_offset ) / length;
  double z_step = (end.sz - begin.sz) / length;

  double act_z = begin.sz;
  for( long act_y=begin.sy ; act_y <= end.sy ; act_y++ )
  {
    side[ act_y ].offset = long( a_offset + 0.5 );
    side[ act_y ].sz = act_z;

    a_offset += offset_step;
    act_z += z_step;
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

  long m = (spoint[ bottom ].sy + spoint[ top ].sy) / 2;
  if( left_side[ m ].offset > right_side[ m ].offset )
  {
    screen_side *t = left_side;  left_side = right_side;  right_side = t;
  }

  long length, offset;
  double act_z, z_step;
  
  for( y = spoint[ top ].sy ; y <= spoint[ bottom ].sy ; y++ )
  { 
    length = right_side[ y ].offset - left_side[ y ].offset;
    if( length <= 0 ) continue;
    
    z_step = (right_side[ y ].sz - left_side[ y ].sz) / length;
    act_z = left_side[ y ].sz;
    offset = left_side[ y ].offset;
    
    while( length-- > 0 )
    {
      if( act_z < zbuffer[ offset ] )
      {
        sbuffer[ offset ] = color;
        zbuffer[ offset ] = act_z;
      }
     
      offset++;
      act_z += z_step; 
    }
  }
}

void polygon::shape( pixel_32 *sbuffer )
{
  long x, y;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, sbuffer );
}

void polygon::load( FILE *input )
{
  fscanf( input, "%ld", &point_count );

  if( point_count >= largest_point_count )
      exit_error( "point_count ist groesser oder gleich largest_point_count.\n" );

  if( (points = new long[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ] );

  long red, green, blue;
  fscanf( input, "%ld %ld %ld", &red, &green, &blue );
  color = pixel_32( uchar( red ), uchar( green ), uchar( blue ) );
}

#endif