#ifndef MATRIX_H
#define MATRIX_H

#include "simple_types.h"

class matrix
{
  private:
    double mx[ 4 ][ 4 ];

    void multiplicate( double n_m[ 4 ][ 4 ] );

  public:
    void rotate_x( double a );
    void rotate_y( double a );
    void rotate_z( double a );
    void translate( double xt, double yt, double zt );
    void scale( double xs, double ys, double zs );

    vertex operator * ( vertex v );
    vector operator * ( vector v );
    local_system operator * ( local_system ls );

    void rows( vector a, vector b, vector c );
    void columns( vector a, vector b, vector c );

    void adjust_hardware( void );
    void clear( void );

    matrix( void ) { clear(); }

  friend matrix operator * ( matrix a, matrix b );
};

void matrix::adjust_hardware( void )
{
  short x, y, z;
  double m[ 16 ];

  z = 0;
  for( y=0 ; y<4 ; y++ )
	 for( x=0 ; x<4 ; x++ )
		m[ z++ ] = mx[ x ][ y ];

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glMultMatrixd( m );
}

matrix operator * ( matrix a, matrix b )
{
  b.multiplicate( a.mx );  return b;
}

void matrix::multiplicate( double n_m[ 4 ][ 4 ] )
{
  char row, column;
  double tm[ 4 ][ 4 ];

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

void matrix::rotate_x( double a )
{
  double rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = 1;  rm[ 0 ][ 1 ] = 0;         rm[ 0 ][ 2 ] = 0;         rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = 0;  rm[ 1 ][ 1 ] = cos( a );  rm[ 1 ][ 2 ] =-sin( a );  rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] = 0;  rm[ 2 ][ 1 ] = sin( a );  rm[ 2 ][ 2 ] = cos( a );  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;  rm[ 3 ][ 1 ] = 0;         rm[ 3 ][ 2 ] = 0;         rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::rotate_y( double a )
{
  double rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = cos( a );  rm[ 0 ][ 1 ] = 0;  rm[ 0 ][ 2 ] = sin( a );  rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = 0;         rm[ 1 ][ 1 ] = 1;  rm[ 1 ][ 2 ] = 0;         rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] =-sin( a );  rm[ 2 ][ 1 ] = 0;  rm[ 2 ][ 2 ] = cos( a );  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;         rm[ 3 ][ 1 ] = 0;  rm[ 3 ][ 2 ] = 0;         rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::rotate_z( double a )
{
  double rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = cos( a );  rm[ 0 ][ 1 ] =-sin( a );  rm[ 0 ][ 2 ] = 0;  rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = sin( a );  rm[ 1 ][ 1 ] = cos( a );  rm[ 1 ][ 2 ] = 0;  rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] = 0;         rm[ 2 ][ 1 ] = 0;         rm[ 2 ][ 2 ] = 1;  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;         rm[ 3 ][ 1 ] = 0;         rm[ 3 ][ 2 ] = 0;  rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::translate( double xt, double yt, double zt )
{
  double tm[ 4 ][ 4 ];

  tm[ 0 ][ 0 ] = 1;   tm[ 0 ][ 1 ] = 0;   tm[ 0 ][ 2 ] = 0;   tm[ 0 ][ 3 ] = xt;
  tm[ 1 ][ 0 ] = 0;   tm[ 1 ][ 1 ] = 1;   tm[ 1 ][ 2 ] = 0;   tm[ 1 ][ 3 ] = yt;
  tm[ 2 ][ 0 ] = 0;   tm[ 2 ][ 1 ] = 0;   tm[ 2 ][ 2 ] = 1;   tm[ 2 ][ 3 ] = zt;
  tm[ 3 ][ 0 ] = 0;   tm[ 3 ][ 1 ] = 0;   tm[ 3 ][ 2 ] = 0;   tm[ 3 ][ 3 ] = 1;

  multiplicate( tm );
}

void matrix::scale( double xs, double ys, double zs )
{
  double sm[ 4 ][ 4 ];

  sm[ 0 ][ 0 ] = xs;  sm[ 0 ][ 1 ] = 0;   sm[ 0 ][ 2 ] = 0;   sm[ 0 ][ 3 ] = 0;
  sm[ 1 ][ 0 ] = 0;   sm[ 1 ][ 1 ] = ys;  sm[ 1 ][ 2 ] = 0;   sm[ 1 ][ 3 ] = 0;
  sm[ 2 ][ 0 ] = 0;   sm[ 2 ][ 1 ] = 0;   sm[ 2 ][ 2 ] = zs;  sm[ 2 ][ 3 ] = 0;
  sm[ 3 ][ 0 ] = 0;   sm[ 3 ][ 1 ] = 0;   sm[ 3 ][ 2 ] = 0;   sm[ 3 ][ 3 ] = 1;

  multiplicate( sm );
}

vertex matrix::operator * ( vertex p )
{
  double wx = p.wx * mx[ 0 ][ 0 ] + p.wy * mx[ 0 ][ 1 ] + p.wz * mx[ 0 ][ 2 ] + mx[ 0 ][ 3 ];
  double wy = p.wx * mx[ 1 ][ 0 ] + p.wy * mx[ 1 ][ 1 ] + p.wz * mx[ 1 ][ 2 ] + mx[ 1 ][ 3 ];
  double wz = p.wx * mx[ 2 ][ 0 ] + p.wy * mx[ 2 ][ 1 ] + p.wz * mx[ 2 ][ 2 ] + mx[ 2 ][ 3 ];

  p.wx = wx;  p.wy = wy;  p.wz = wz;  return p;
}

vector matrix::operator * ( vector v )
{
  double x = v.x * mx[ 0 ][ 0 ] + v.y * mx[ 0 ][ 1 ] + v.z * mx[ 0 ][ 2 ];
  double y = v.x * mx[ 1 ][ 0 ] + v.y * mx[ 1 ][ 1 ] + v.z * mx[ 1 ][ 2 ];
  double z = v.x * mx[ 2 ][ 0 ] + v.y * mx[ 2 ][ 1 ] + v.z * mx[ 2 ][ 2 ];

  return vector( x, y, z );
}

local_system matrix::operator * ( local_system ls )
{
  ls.pos = this->operator * ( ls.pos );
  ls.x_axis = this->operator * ( ls.x_axis );
  ls.y_axis = this->operator * ( ls.y_axis );
  ls.z_axis = this->operator * ( ls.z_axis );

  return ls;
}

void matrix::clear( void )
{
  mx[ 0 ][ 0 ] = 1;  mx[ 0 ][ 1 ] = 0;  mx[ 0 ][ 2 ] = 0;  mx[ 0 ][ 3 ] = 0;
  mx[ 1 ][ 0 ] = 0;  mx[ 1 ][ 1 ] = 1;  mx[ 1 ][ 2 ] = 0;  mx[ 1 ][ 3 ] = 0;
  mx[ 2 ][ 0 ] = 0;  mx[ 2 ][ 1 ] = 0;  mx[ 2 ][ 2 ] = 1;  mx[ 2 ][ 3 ] = 0;
  mx[ 3 ][ 0 ] = 0;  mx[ 3 ][ 1 ] = 0;  mx[ 3 ][ 2 ] = 0;  mx[ 3 ][ 3 ] = 1;
}

void matrix::rows( vector a, vector b, vector c )
{
  double rm[ 4 ][ 4 ];

  rm[ 0 ][ 0 ] = a.x;  rm[ 0 ][ 1 ] = a.y;  rm[ 0 ][ 2 ] = a.z;  rm[ 0 ][ 3 ] = 0;
  rm[ 1 ][ 0 ] = b.x;  rm[ 1 ][ 1 ] = b.y;  rm[ 1 ][ 2 ] = b.z;  rm[ 1 ][ 3 ] = 0;
  rm[ 2 ][ 0 ] = c.x;  rm[ 2 ][ 1 ] = c.y;  rm[ 2 ][ 2 ] = c.z;  rm[ 2 ][ 3 ] = 0;
  rm[ 3 ][ 0 ] = 0;    rm[ 3 ][ 1 ] = 0;    rm[ 3 ][ 2 ] = 0;    rm[ 3 ][ 3 ] = 1;

  multiplicate( rm );
}

void matrix::columns( vector a, vector b, vector c )
{
  double cm[ 4 ][ 4 ];

  cm[ 0 ][ 0 ] = a.x;  cm[ 0 ][ 1 ] = b.x;  cm[ 0 ][ 2 ] = c.x;  cm[ 0 ][ 3 ] = 0;
  cm[ 1 ][ 0 ] = a.y;  cm[ 1 ][ 1 ] = b.y;  cm[ 1 ][ 2 ] = c.y;  cm[ 1 ][ 3 ] = 0;
  cm[ 2 ][ 0 ] = a.z;  cm[ 2 ][ 1 ] = b.z;  cm[ 2 ][ 2 ] = c.z;  cm[ 2 ][ 3 ] = 0;
  cm[ 3 ][ 0 ] = 0;    cm[ 3 ][ 1 ] = 0;    cm[ 3 ][ 2 ] = 0;    cm[ 3 ][ 3 ] = 1;

  multiplicate( cm );
}

#endif