#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "matrix.h"
#include "draw_line.h"

const long largest_point_count = 100;
svertex spoint[ largest_point_count ];

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    vertex *wpoint;
    
    void project( void );

  public:
    vertex pos;

    void load( long pc, pixel_32 c );
    void update_pos( matrix m );
    void display( pixel_32 *screen );

    polygon( void ) : point_count( 0 ), wpoint( NULL ) { }
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

void polygon::display( pixel_32 *screen )
{
  project();

  long x, y;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, screen );
}

void polygon::load( long pc, pixel_32 c )
{
  point_count = pc;  color = c;

  if( point_count >= largest_point_count )
    exit_error( "polygon::load(): point_count größer als largest_point_count.\n" );

  if( (wpoint = new vertex[ point_count ]) == NULL )
    exit_error( "polygon::load(): Fehler bei der Reservierung von Arbeitspeicher.\n" );

  const double pi = 3.1415926535;
  matrix m;

  for( long x=0 ; x<point_count ; x++ ) 
  {
    vertex v = vertex( 10, 0, 0 );

    wpoint[ x ] = m * v;

    m.rotate_z( (2*pi) / point_count );
  }

  pos = vertex( 0, 0, 0 );
}

#endif