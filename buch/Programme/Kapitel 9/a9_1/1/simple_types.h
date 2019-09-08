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

struct local_system
{
  vertex pos;
  vector x_axis, y_axis, z_axis;

  local_system( void )
  {
    pos = vertex( 0, 0, 0 );  x_axis = vector( 1, 0, 0 );  y_axis = vector( 0, 1, 0 );  z_axis = vector( 0, 0, 1 );
  }
};

struct svertex
{
  long sx, sy;
  double sz;

  svertex( void ) : sx( 0 ), sy( 0 ), sz( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ), sz( 0 ) { }
};

vector operator * ( double t, vector v )
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

#endif