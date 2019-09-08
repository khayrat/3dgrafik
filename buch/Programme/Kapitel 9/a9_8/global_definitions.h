#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"

const double z_min = 2;
const double local_z_max = 10;
const double z_max = 100000;
const double pr_cnst = 800;

vertex rectangle_hull[ 4 ];

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, xr, yr, bit_depth );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  const double pi = 3.141592654;
  double alpha = (360 / pi) * atan( y_res / (2 * pr_cnst) );
  gluPerspective( alpha, double( x_res ) / y_res, z_min, z_max );

  glScaled( 1, 1, -1 );

  glDepthFunc( GL_LEQUAL );
  glEnable( GL_DEPTH_TEST );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

#endif