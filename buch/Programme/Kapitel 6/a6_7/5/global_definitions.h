#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"

const double z_min = 10;
const double z_max = 100000;
const double pr_cnst = 800;

vector light( -1, 0, 0 );

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, xr, yr, bit_depth );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  const double pi = 3.141592654;
  double alpha = (360 / pi) * atan( y_res / (2 * pr_cnst) );
  gluPerspective( alpha, double( x_res ) / y_res, z_min, z_max );

  glScaled( 1, 1, -1 );

//  glFrontFace( GL_CW );
//  glEnable( GL_CULL_FACE );

  glDepthFunc( GL_LEQUAL );
  glEnable( GL_DEPTH_TEST );

  glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glEnable( GL_POINT_SMOOTH );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
}

#endif