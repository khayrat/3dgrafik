#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "draw_line.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    svertex *spoint;

  public:
    void display( pixel_32 *screen );

    polygon( long pc, pixel_32 c, svertex *s );
   ~polygon( void ) {  if( spoint ) delete [] spoint;  }
};

void polygon::display( pixel_32 *screen )
{
  long x, y;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, screen );
}

polygon::polygon( long pc, pixel_32 c, svertex *s )
{
  point_count = pc;  color = c;

  if( (spoint = new svertex[ point_count ]) == NULL )
    exit_error( "polygon::polygon(): Fehler bei der Reservierung von Arbeitspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ ) spoint[ x ] = s[ x ];
}

#endif