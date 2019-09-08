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

    uchar visible( void );    
    void project( void );
    void shape( pixel_32 *sbuffer );
    void rasterize( pixel_32 *sbuffer );

  public:
    void load( FILE *input );
    void display( vertex *vs, pixel_32 *sbuffer );

    polygon( const polygon &p ) {  exit_error( "polygon: Aufruf Kopierkonstruktor\n" );  }
    polygon( void ) : point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }
};

void polygon::project( void )
{
  for( long x=0 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wz <= 0.0 ) exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  
    spoint[ x ].sx = long( wpoint[ x ].wx / wpoint[ x ].wz *  1000.0 + x_res/2 + 0.5 );
    spoint[ x ].sy = long( wpoint[ x ].wy / wpoint[ x ].wz * -1000.0 + y_res/2 + 0.5 );

    if( spoint[ x ].sx < 0 || spoint[ x ].sx >= x_res || spoint[ x ].sy < 0 || spoint[ x ].sy >= y_res )
      exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  }
}

uchar polygon::visible( void )
{
  long z = (spoint[ point_count-1 ].sx - spoint[ 0 ].sx) * (spoint[ 1 ].sy - spoint[ 0 ].sy) -
           (spoint[ point_count-1 ].sy - spoint[ 0 ].sy) * (spoint[ 1 ].sx - spoint[ 0 ].sx);
             
  if( z < 1 ) return 1;

  return 0;
}

void polygon::display( vertex *vs, pixel_32 *sbuffer )
{
  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = vs[ points[ x ] ];

  project();

  if( visible() )
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

  long m = (spoint[ bottom ].sy + spoint[ top ].sy) / 2;
  if( left_side[ m ] > right_side[ m ] )
  {
    long *t = left_side;  left_side = right_side;  right_side = t;
  }

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