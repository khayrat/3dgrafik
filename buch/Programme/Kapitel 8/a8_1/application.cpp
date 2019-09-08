#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron cube;
  cube.load( "cube.tg3" );

  matrix m;
  m.translate( 0, 0, 400 );
  cube.update_pos( m );
  m.clear();

  m.translate( -cube.pos.wx, -cube.pos.wy, -cube.pos.wz );
  m.rotate_x( 0.007 );
  m.rotate_y( 0.008 );
  m.rotate_z( 0.002 );
  m.translate( cube.pos.wx, cube.pos.wy, cube.pos.wz );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( 1 )
  {
    if( input.check() == 1 ) return 1;
    if( input.event_key != 0 ) return 1;

    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    cube.update_pos( m );
    cube.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}
