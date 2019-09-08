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
  double x, y, z, length;

  vector( void ) : x( 0 ), y( 0 ), z( 0 ), length( 0 ) { }
  vector( double vx, double vy, double vz ) :
        x( vx ), y( vy ), z( vz ), length( sqrt( vx*vx + vy*vy + vz*vz ) ) { }
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
  double sz;

  svertex( void ) : sx( 0 ), sy( 0 ), sz( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ), sz( 0 ) { }
};

struct screen_side
{
  long offset;
  double sz;
};

vector operator - ( vertex v1, vertex v2 )
{
  return vector( (v1.wx - v2.wx), (v1.wy - v2.wy), (v1.wz - v2.wz) );
}

vector operator * ( double t, vector v )
{
  return vector( t*v.x, t*v.y, t*v.z );
}

vertex operator + ( vertex v, vector dir )
{
  return vertex( (v.wx + dir.x), (v.wy + dir.y), (v.wz + dir.z) );
}

double dot( vector u, vector v )
{
  return u.x*v.x + u.y*v.y + u.z*v.z;
}

vector cross( vector a, vector b )
{
  double x = a.y * b.z - a.z * b.y;
  double y = a.z * b.x - a.x * b.z;
  double z = a.x * b.y - a.y * b.x;

  return vector( x, y, z );
}

#endif