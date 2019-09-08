#ifndef BITMAP_32_H
#define BITMAP_32_H

#include "simple_types.h"
#include "screen_interface.h"

class bitmap_32
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
  
    void clear( void );
    
    void display( long sx, long sy, pixel_32 *sbuffer );
    
    bitmap_32( long xs, long ys );
    bitmap_32( const bitmap_32 &b ) {  exit_error( "bitmap_32: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_32( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_32::clear( void )
{
  memset( picture, 0, xscale * yscale * sizeof( picture[ 0 ] ) );
}

bitmap_32::bitmap_32( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  clear();
}

void bitmap_32::display( long sx, long sy, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) sbuffer[ offset++ ] = picture[ z++ ];

    offset += x_res - xscale;
  }  
}

#endif