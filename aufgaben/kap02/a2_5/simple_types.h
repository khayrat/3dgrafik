#ifndef SIMPLE_TYPES_H
#define SIMPLE_TYPES_H

#include <math.h>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;
typedef unsigned int   uint;

struct pixel_32
{
  uchar blue, green, red, alpha;
  
  pixel_32( void ) : red( 0 ), green( 0 ), blue( 0 ), alpha( 0 ) { }
  pixel_32( uchar r, uchar g, uchar b ) : red( r ), green( g ), blue( b ), alpha( 0 ) { }
};

struct vertex {
  uint x;
  uint y;

  vertex(void) : x(0), y(0) {}
  vertex(uint x_coordinate, uint y_coordinate) : x(x_coordinate), y(y_coordinate) {}
};

#endif
