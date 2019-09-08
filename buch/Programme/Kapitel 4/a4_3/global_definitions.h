#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"

const long largest_point_count = 100;
svertex spoint[ largest_point_count ];

long *left_side, *right_side;
pixel_32 *sbuffer;

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, xr, yr, bit_depth );

  if( (sbuffer = new pixel_32[ x_res*y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  if
  ( (left_side = new long[ y_res ]) == NULL ||
    (right_side = new long[ y_res ]) == NULL
  )
  exit_error( "*left_sede, *right_side: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
}

void destroy_world( void )
{
  if( sbuffer ) delete [] sbuffer;

  if( left_side ) delete [] left_side;
  if( right_side ) delete [] right_side;
}

#endif