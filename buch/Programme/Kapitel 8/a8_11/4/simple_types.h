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

struct vector
{
  double x, y, z;

  double length( void ) {  return sqrt( x*x + y*y + z*z );  }

  vector( void ) : x( 0 ), y( 0 ), z( 0 ) { }
  vector( double vx, double vy, double vz ) : x( vx ), y( vy ), z( vz ) { }
};

struct vertex
{
  double wx, wy, wz;

  vertex( void ) : wx( 0 ), wy( 0 ), wz( 0 ) { }
  vertex( double x, double y, double z ) : wx( x ), wy( y ), wz( z ) { }
};

struct tvertex
{
  double wx, wy, wz;
  double tx, ty;
};

struct svertex
{
  long sx, sy;
  double sz, tx, ty;
};

struct point
{
  long vertex_offset;
  double tx, ty;
};

struct screen_side
{
  long offset;
  double sz, tx, ty;
};

#endif