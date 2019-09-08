#ifndef BITMAP_8_H
#define BITMAP_8_H

#include "simple_types.h"

class bitmap_8
{
  public:
    long xscale, yscale;
    long *picture;
  
    void update( long component_count );
    void display( long sx, long sy, pixel_32 *pal, pixel_32 *sbuffer );

    bitmap_8( long xs, long ys );
    bitmap_8( const bitmap_8 &b ) {  exit_error( "bitmap_8: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_8( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_8::update( long component_count )
{
  long pixel_count = xscale * yscale;
  long x;

  for( x=pixel_count-2*xscale ; x<pixel_count ; x++ )
  {
    if( (rand() % 100) < 30 ) picture[ x ] = 0;

    else picture[ x ] = component_count - 1;
  }

  for( x=xscale ; x<pixel_count-xscale-1 ; x++ )
  {
    picture[ x-xscale ] = (picture[ x ] + picture[ x-1 ] +
                           picture[ x+1 ] + picture[ x+xscale-1 ] +
                           picture[ x+xscale ] + picture[ x+xscale+1 ]) / 6;

    if( picture[ x-xscale ] > 0 ) picture[ x-xscale ]--;
  }
}

void bitmap_8::display( long sx, long sy, pixel_32 *pal, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) sbuffer[ offset++ ] = pal[ picture[ z++ ] ];

    offset += x_res - xscale;
  }  
}

bitmap_8::bitmap_8( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (picture = new long[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<xscale*yscale ; x++ ) picture[ x ] = 0;
}

#endif