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
  float tx, ty;

  hvertex( void ) { }
  hvertex( vertex p, float x, float y ) {  pos = p; tx = x;  ty = y;  }
};

struct vector
{
  float x, y, z;

  float length( void ) {  return float( sqrt( x*x + y*y + z*z ) );  }
  void set_length( float new_length );

  vector( void ) : x( 0 ), y( 0 ), z( 0 ) { }
  vector( float vx, float vy, float vz ) : x( vx ), y( vy ), z( vz ) { }
};

struct local_system
{
  vertex pos;
  vector x_axis, y_axis, z_axis;

  local_system( void )
  {
    pos = vertex( 0, 0, 0 );  x_axis = vector( 1, 0, 0 );  y_axis = vector( 0, 1, 0 );  z_axis = vector( 0, 0, 1 );
  }
};

struct point
{
  long offset;
  float tx, ty;
};

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

void vector::set_length( float new_length )
{
  if( length() == 0 ) return;

  float t = new_length / length();
  x *= t;  y *= t;  z *= t;
}

vector operator * ( float t, vector v )
{
  return vector( t*v.x, t*v.y, t*v.z );
}

vector operator - ( vector v1, vector v2 )
{
  return vector( (v1.x - v2.x), (v1.y - v2.y), (v1.z - v2.z) );
}

vector operator + ( vector v1, vector v2 )
{
  return vector( (v1.x + v2.x), (v1.y + v2.y), (v1.z + v2.z) );
}

vertex operator + ( vertex v, vector dir )
{
  return vertex( (v.wx + dir.x), (v.wy + dir.y), (v.wz + dir.z) );
}

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