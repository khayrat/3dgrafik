#ifndef MATRIX_H
#define MATRIX_H

#include "simple_types.h"

class matrix
{
  private:
    float mx[ 4 ][ 4 ];

    void multiplicate( float n_m[ 4 ][ 4 ] );

  public:
    void rotate_x( float a );
    void rotate_y( float a );
    void rotate_z( float a );
    void translate( float xt, float yt, float zt );
    void scale( float xs, float ys, float zs );

    vertex operator * ( vertex v );
    vector operator * ( vector v );

    void clear( void );

    matrix( void ) { clear(); }
};

void matrix::multiplicate( float n_m[ 4 ][ 4 ] )
{
  char row, column;
  float tm[ 4 ][ 4 ];

  for( row=0 ; row < 4 ; row++ ) 
     for( column=0 ; column < 4 ; column++ )
        tm[ row ][ column ] = n_m[ row ][ 0 ] * mx[ 0 ][ column ] +
                              n_m[ row ][ 1 ] * mx[ 1 ][ column ] +
                              n_m[ row ][ 2 ] * mx[ 2 ][ column ] +
                              n_m[ row ][ 3 ] * mx[ 3 ][ column ];

  for( row=0 ; row < 4 ; row++ )
     for( column=0 ; column < 4 ; column++ )
        mx[ row ][ column ] = tm[ row ][ column ];
}

float fsin( float a ) {  return float( sin( a ) );  }
float fcos( float a ) {  return float( cos( a ) );  }

void matrix::rotate_x( float a )
{
  float rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = 1;  rm[ 0 ][ 1 ] = 0;          rm[ 0 ][ 2 ] = 0;          rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = 0;  rm[ 1 ][ 1 ] = fcos( a );  rm[ 1 ][ 2 ] =-fsin( a );  rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] = 0;  rm[ 2 ][ 1 ] = fsin( a );  rm[ 2 ][ 2 ] = fcos( a );  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;  rm[ 3 ][ 1 ] = 0;          rm[ 3 ][ 2 ] = 0;          rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::rotate_y( float a )
{
  float rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = fcos( a );  rm[ 0 ][ 1 ] = 0;  rm[ 0 ][ 2 ] = fsin( a );  rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = 0;          rm[ 1 ][ 1 ] = 1;  rm[ 1 ][ 2 ] = 0;          rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] =-fsin( a );  rm[ 2 ][ 1 ] = 0;  rm[ 2 ][ 2 ] = fcos( a );  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;          rm[ 3 ][ 1 ] = 0;  rm[ 3 ][ 2 ] = 0;          rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::rotate_z( float a )
{
  float rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = fcos( a );  rm[ 0 ][ 1 ] =-fsin( a );  rm[ 0 ][ 2 ] = 0;  rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = fsin( a );  rm[ 1 ][ 1 ] = fcos( a );  rm[ 1 ][ 2 ] = 0;  rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] = 0;          rm[ 2 ][ 1 ] = 0;          rm[ 2 ][ 2 ] = 1;  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;          rm[ 3 ][ 1 ] = 0;          rm[ 3 ][ 2 ] = 0;  rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::translate( float xt, float yt, float zt )
{
  float tm[ 4 ][ 4 ];

  tm[ 0 ][ 0 ] = 1;   tm[ 0 ][ 1 ] = 0;   tm[ 0 ][ 2 ] = 0;   tm[ 0 ][ 3 ] = xt;
  tm[ 1 ][ 0 ] = 0;   tm[ 1 ][ 1 ] = 1;   tm[ 1 ][ 2 ] = 0;   tm[ 1 ][ 3 ] = yt;
  tm[ 2 ][ 0 ] = 0;   tm[ 2 ][ 1 ] = 0;   tm[ 2 ][ 2 ] = 1;   tm[ 2 ][ 3 ] = zt;
  tm[ 3 ][ 0 ] = 0;   tm[ 3 ][ 1 ] = 0;   tm[ 3 ][ 2 ] = 0;   tm[ 3 ][ 3 ] = 1;

  multiplicate( tm );
}

void matrix::scale( float xs, float ys, float zs )
{
  float sm[ 4 ][ 4 ];

  sm[ 0 ][ 0 ] = xs;  sm[ 0 ][ 1 ] = 0;   sm[ 0 ][ 2 ] = 0;   sm[ 0 ][ 3 ] = 0;
  sm[ 1 ][ 0 ] = 0;   sm[ 1 ][ 1 ] = ys;  sm[ 1 ][ 2 ] = 0;   sm[ 1 ][ 3 ] = 0;
  sm[ 2 ][ 0 ] = 0;   sm[ 2 ][ 1 ] = 0;   sm[ 2 ][ 2 ] = zs;  sm[ 2 ][ 3 ] = 0;
  sm[ 3 ][ 0 ] = 0;   sm[ 3 ][ 1 ] = 0;   sm[ 3 ][ 2 ] = 0;   sm[ 3 ][ 3 ] = 1;

  multiplicate( sm );
}

vertex matrix::operator * ( vertex p )
{
  float wx = p.wx * mx[ 0 ][ 0 ] + p.wy * mx[ 0 ][ 1 ] + p.wz * mx[ 0 ][ 2 ] + mx[ 0 ][ 3 ];
  float wy = p.wx * mx[ 1 ][ 0 ] + p.wy * mx[ 1 ][ 1 ] + p.wz * mx[ 1 ][ 2 ] + mx[ 1 ][ 3 ];
  float wz = p.wx * mx[ 2 ][ 0 ] + p.wy * mx[ 2 ][ 1 ] + p.wz * mx[ 2 ][ 2 ] + mx[ 2 ][ 3 ];

  p.wx = wx;  p.wy = wy;  p.wz = wz;  return p;
}

vector matrix::operator * ( vector v )
{
  float x = v.x * mx[ 0 ][ 0 ] + v.y * mx[ 0 ][ 1 ] + v.z * mx[ 0 ][ 2 ];
  float y = v.x * mx[ 1 ][ 0 ] + v.y * mx[ 1 ][ 1 ] + v.z * mx[ 1 ][ 2 ];
  float z = v.x * mx[ 2 ][ 0 ] + v.y * mx[ 2 ][ 1 ] + v.z * mx[ 2 ][ 2 ];

  return vector( x, y, z );
}

void matrix::clear( void )
{
  mx[ 0 ][ 0 ] = 1;  mx[ 0 ][ 1 ] = 0;  mx[ 0 ][ 2 ] = 0;  mx[ 0 ][ 3 ] = 0;
  mx[ 1 ][ 0 ] = 0;  mx[ 1 ][ 1 ] = 1;  mx[ 1 ][ 2 ] = 0;  mx[ 1 ][ 3 ] = 0;
  mx[ 2 ][ 0 ] = 0;  mx[ 2 ][ 1 ] = 0;  mx[ 2 ][ 2 ] = 1;  mx[ 2 ][ 3 ] = 0;
  mx[ 3 ][ 0 ] = 0;  mx[ 3 ][ 1 ] = 0;  mx[ 3 ][ 2 ] = 0;  mx[ 3 ][ 3 ] = 1;
}

#endif