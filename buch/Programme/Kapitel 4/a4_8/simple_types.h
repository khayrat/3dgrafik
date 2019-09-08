#ifndef SIMPLE_TYPES_H
#define SIMPLE_TYPES_H

#include <math.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

struct pixel_32
{
  uchar blue, green, red, alpha;
  
  pixel_32( void ) : red( 150 ), green( 0 ), blue( 255 ), alpha( 0 ) { }
  pixel_32( uchar r, uchar g, uchar b ) : red( r ), green( g ), blue( b ), alpha( 0 ) { }
};

struct vertex
{
  float wx, wy, wz;

  vertex( void ) : wx( 0 ), wy( 0 ), wz( 0 ) { }
  vertex( float x, float y, float z ) : wx( x ), wy( y ), wz( z ) { }
};

struct hvertex
{
  vertex pos;
  pixel_32 color;

  hvertex( void ) { }
  hvertex( vertex p, pixel_32 c ) {  pos = p;  color = c;  }
};

#endif