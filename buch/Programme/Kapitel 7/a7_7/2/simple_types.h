#ifndef SIMPLE_TYPES_H
#define SIMPLE_TYPES_H

#include <math.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

struct pixel_32
{
  uchar blue, green, red, alpha;
  
  pixel_32( void ) : red( 0 ), green( 0 ), blue( 0 ), alpha( 0 ) { }
  pixel_32( uchar r, uchar g, uchar b ) : red( r ), green( g ), blue( b ), alpha( 0 ) { }
};

struct svertex
{
  double sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

struct vector
{
  double x, y, length;
  
  vector( void ) : x( 0 ), y( 0 ), length( 0 ) { }
  vector( double xt, double yt ) : x( xt ), y( yt ), length( sqrt( xt*xt + yt*yt ) ) { }
};

uchar operator != ( pixel_32 c1, pixel_32 c2 )
{
  return c1.red != c2.red || c1.green != c2.green ||
         c1.blue != c2.blue || c1.alpha != c2.alpha;
}

#endif