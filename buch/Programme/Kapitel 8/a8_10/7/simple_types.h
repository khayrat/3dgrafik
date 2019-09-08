#ifndef SIMPLE_TYPES_H
#define SIMPLE_TYPES_H

#include <math.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

struct pixel_32
{
  uchar blue, green, red, alpha;
  
  pixel_32( void ) : red( 0 ), green( 0 ), blue( 0 ), alpha( 255 ) { }
  pixel_32( uchar r, uchar g, uchar b ) : red( r ), green( g ), blue( b ), alpha( 255 ) { }
};

struct vector
{
  double x, y;

  double length( void ) {  return sqrt( x*x + y*y );  }
  void set_length( double new_length ) {  double t = new_length / length();  x *= t;  y *= t;  }

  vector( void ) : x( 0 ), y( 0 ) { }
  vector( double vx, double vy ) : x( vx ), y( vy ) { }
};

struct svertex
{
  double sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( double x, double y ) : sx( x ), sy( y ) { }
};

svertex operator + ( svertex pos, vector dir )
{
  return svertex( pos.sx + dir.x, pos.sy + dir.y );
}

uchar operator == ( pixel_32 a, pixel_32 b )
{
  return a.red == b.red && a.green == b.green && a.blue == b.blue;
}

#endif