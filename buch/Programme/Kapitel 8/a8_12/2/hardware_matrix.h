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

void hardware_matrix::clear( void )
{
  D3DXMatrixIdentity( &world_matrix );
}

#endif