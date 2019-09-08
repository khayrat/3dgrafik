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

struct vertex
{
  double wx, wy;

  vertex( void ) : wx( 0 ), wy( 0 ) { }
  vertex( double x, double y ) : wx( x ), wy( y ) { }
};

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

#endif