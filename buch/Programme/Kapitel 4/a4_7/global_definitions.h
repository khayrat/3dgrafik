#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"

const double z_min = 10;
const double z_max = 100000;
const double pr_cnst = 200;

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, xr, yr, bit_depth );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  const double pi = 3.141592654;
  double alpha = (360 / pi) * atan( y_res / (2 * pr_cnst) );
  gluPerspective( alpha, double( x_res ) / y_res, z_min, z_max );

  glScaled( 1, 1, -1 );

  glFrontFace( GL_CW );
  glEnable( GL_CULL_FACE );
}

#endif