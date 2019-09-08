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
  cube.load( "cube.tg2" );

  matrix m;
  m.scale( 5, 5, 5 );
  m.translate( 0, 0, 150 );
  cube.update_pos( m );
  m.clear();

  m.translate( -cube.pos.wx, -cube.pos.wy, -cube.pos.wz );
  m.rotate_x( 0.007 );
  m.rotate_y( 0.008 );
  m.rotate_z( 0.002 );
  m.translate( cube.pos.wx, cube.pos.wy, cube.pos.wz );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = 0;

  while( 1 )
  {
    if( clear_translation > max_clear_translation ) clear_translation += (1.0 / z_min);
    else
    {
      memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
      clear_translation = 0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    cube.update_pos( m );
    cube.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}