#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"

const long largest_point_count = 1000;
vertex wpoint[ largest_point_count+10 ];
vertex c_wpoint[ largest_point_count+10 ];
vertex d_wpoint[ largest_point_count+10 ];
svertex spoint[ largest_point_count+20 ];
svertex c_spoint[ largest_point_count+20 ];

const long screen_x_res = 1024;
const long screen_y_res = 768;

pixel_32 *sbuffer = NULL;
double *zbuffer = NULL;
screen_side *left_side, *right_side;

long x_res, y_res;

double z_min, z_max;
long x_min, x_max, y_min, y_max;
double xpr_cnst, ypr_cnst;

vector light( -1, -1, 1 );

local_system camera;

void initialise_world( HINSTANCE hInstance )
{
  screen_interface.open_window( hInstance, screen_x_res, screen_y_res, 32 );

  if
  (
    (left_side = new screen_side[ screen_y_res ]) == NULL || 
    (right_side = new screen_side[ screen_y_res ]) == NULL
  )
  exit_error("initialise_world(): Fehler bei der Reservierung von Arbeitsspeicher.\n");
}

void destroy_world( void )
{
  if( left_side ) delete [] left_side;
  if( right_side ) delete [] right_side;
}

#endif