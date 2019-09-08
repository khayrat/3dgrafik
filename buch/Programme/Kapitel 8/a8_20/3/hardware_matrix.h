#ifndef HARDWARE_MATRIX_H
#define HARDWARE_MATRIX_H

#include <d3dx9.h>

#include "simple_types.h"

class hardware_matrix
{
  private:
    D3DXMATRIXA16 world_matrix;

  public:
    void rotate_x( float a );
    void rotate_y( float a );
    void rotate_z( float a );
    void translate( float xt, float yt, float zt );
    void scale( float xs, float ys, float zs );

    void rows( vector a, vector b, vector c );
    void columns( vector a, vector b, vector c );

    void clear( void );

    hardware_matrix( void ) {  clear();  }

  friend class hardware_interface;
};

void hardware_matrix::rotate_x( float angle )
{
  D3DXMATRIXA16 rm;

  D3DXMatrixRotationX( &rm, angle );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &rm );
}

void hardware_matrix::rotate_y( float angle )
{
  D3DXMATRIXA16 rm;

  D3DXMatrixRotationY( &rm, angle );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &rm );
}

void hardware_matrix::rotate_z( float angle )
{
  D3DXMATRIXA16 rm;

  D3DXMatrixRotationZ( &rm, angle );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &rm );
}

void hardware_matrix::translate( float xt, float yt, float zt )
{
  D3DXMATRIXA16 tm;

  D3DXMatrixTranslation( &tm, xt, yt, zt );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &tm );
}

void hardware_matrix::scale( float xs, float ys, float zs )
{
  D3DXMATRIXA16 sm;

  D3DXMatrixScaling( &sm, xs, ys, zs );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &sm );
}

void hardware_matrix::rows( vector a, vector b, vector c )
{
  D3DXMATRIXA16 rm = D3DXMATRIXA16
  (
    a.x,  a.y,  a.z,  0,
    b.x,  b.y,  b.z,  0,
    c.x,  c.y,  c.z,  0,
    0,    0,    0,    1
  );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &rm );
}

void hardware_matrix::columns( vector a, vector b, vector c )
{
  D3DXMATRIXA16 rm = D3DXMATRIXA16
  (
    a.x,  b.x,  c.x,  0,
    a.y,  b.y,  c.y,  0,
    a.z,  b.z,  c.z,  0,
    0,    0,    0,    1
  );

  D3DXMatrixMultiply( &world_matrix, &world_matrix, &rm );
}

void hardware_matrix::clear( void )
{
  D3DXMatrixIdentity( &world_matrix );
}

#endif