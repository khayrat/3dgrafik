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
  double wx, wy, wz;

  vertex( void ) : wx( 0 ), wy( 0 ), wz( 0 ) { }
  vertex( double x, double y, double z ) : wx( x ), wy( y ), wz( z ) { }
};

struct svertex
{
  long sx, sy;
  double wz;

  svertex( void ) : sx( 0 ), sy( 0 ), wz( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ), wz( 0 ) { }
};

struct screen_side
{
  long offset;
  double wz;	
};

#endif