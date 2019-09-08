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

struct vector
{
  float x, y, z, length;

  vector( void ) : x( 0 ), y( 0 ), z( 0 ), length( 0 ) { }
  vector( float vx, float vy, float vz ) :
        x( vx ), y( vy ), z( vz ), length( float( sqrt( vx*vx + vy*vy + vz*vz ) ) ) { }
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

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

vector operator - ( vertex v1, vertex v2 )
{
  return vector( (v1.wx - v2.wx), (v1.wy - v2.wy), (v1.wz - v2.wz) );
}

double dot( vector u, vector v )
{
  return u.x*v.x + u.y*v.y + u.z*v.z;
}

vector cross( vector a, vector b )
{
  float x = a.y * b.z - a.z * b.y;
  float y = a.z * b.x - a.x * b.z;
  float z = a.x * b.y - a.y * b.x;

  return vector( x, y, z );
}

#endif