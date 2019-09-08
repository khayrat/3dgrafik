#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"

const long largest_point_count = 10000;
vertex wpoint[ largest_point_count+10 ];
vertex c_wpoint[ largest_point_count+10 ];
svertex spoint[ largest_point_count+20 ];
svertex c_spoint[ largest_point_count+20 ];

screen_side *left_side, *right_side;
pixel_32 *sbuffer = NULL;
double *zbuffer = NULL;

const double z_min = 1.0, z_max = 10000.0;
long x_min, x_max, y_min, y_max;
const double pr_cnst = 200;

double clear_translation = 0.0;
const double max_clear_translation = -2147483648.0 + 2 * z_max;

vector light( 0.2, -0.2, 1 );

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, xr, yr, bit_depth );

  x_min = 0;  x_max = xr - 1;
  y_min = 0;  y_max = yr - 1;

  if
  (
    (sbuffer = new pixel_32[ x_res*y_res ]) == NULL ||
    (zbuffer = new double[ x_res*y_res ])   == NULL ||
    (left_side = new screen_side[ y_res ])  == NULL ||
    (right_side = new screen_side[ y_res ]) == NULL
  )
  exit_error( "initialise_world(): Fehler bei der Reservierung von Arbeitsspeicher.\n" );
}

void destroy_world( void )
{
  if( sbuffer ) delete [] sbuffer;
  if( zbuffer ) delete [] zbuffer;
  if( left_side ) delete [] left_side;
  if( right_side ) delete [] right_side;
}

#endif