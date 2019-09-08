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
  float x, y, z;

  void set_length( float nl );
  float length( void ) {  return float( sqrt( x*x + y*y + z*z ) );  }

  vector( void ) : x( 0 ), y( 0 ), z( 0 ) { }
  vector( float vx, float vy, float vz ) : x( vx ), y( vy ), z( vz ) { }
};

struct vertex
{
  float wx, wy, wz;
  vector normal;

  vertex( void ) : wx( 0 ), wy( 0 ), wz( 0 ) { }
  vertex( float x, float y, float z ) : wx( x ), wy( y ), wz( z ) { }
};

typedef vertex hvertex;

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

void vector::set_length( float nl )
{
  float t = nl / length();

  x *= t;  y *= t;  z *= t;
}

vector operator - ( vertex v1, vertex v2 )
{
  return vector( (v1.wx - v2.wx), (v1.wy - v2.wy), (v1.wz - v2.wz) );
}

vertex operator + ( vertex pos, vector dir )
{
  vertex new_pos;

  new_pos.wx = pos.wx + dir.x;
  new_pos.wy = pos.wy + dir.y;
  new_pos.wz = pos.wz + dir.z;

  return new_pos;
}

vector operator * ( float t, vector v )
{
  return vector( v.x*t, v.y*t, v.z*t );
}

float dot( vector u, vector v )
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