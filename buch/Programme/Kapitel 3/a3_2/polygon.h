#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
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
    void display( pixel_32 *screen );

    polygon( long pc, pixel_32 c, vertex *vs );
   ~polygon( void ) {  if( wpoint ) delete [] wpoint;  }
};

void polygon::project( void )
{
  for( long x=0 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wz <= 0.0 ) exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  
    spoint[ x ].sx = long( wpoint[ x ].wx / wpoint[ x ].wz *  200.0 + x_res/2 + 0.5 );
    spoint[ x ].sy = long( wpoint[ x ].wy / wpoint[ x ].wz * -200.0 + y_res/2 + 0.5 );

    if( spoint[ x ].sx < 0 || spoint[ x ].sx >= x_res || spoint[ x ].sy < 0 || spoint[ x ].sy >= y_res )
      exit_error( "polygon_project(): Polygon befindet sich ausserhalb des Viewports.\n" );
  }
}

void polygon::display( pixel_32 *screen )
{
  project();

  long x, y;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, screen );
}

polygon::polygon( long pc, pixel_32 c, vertex *vs )
{
  point_count = pc;  color = c;

  if( point_count >= largest_point_count )
    exit_error( "polygon::polygon(): point_count größer als largest_point_count.\n" );

  if( (wpoint = new vertex[ point_count ]) == NULL )
    exit_error( "polygon::polygon(): Fehler bei der Reservierung von Arbeitspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ ) wpoint[ x ] = vs[ x ];
}

#endif