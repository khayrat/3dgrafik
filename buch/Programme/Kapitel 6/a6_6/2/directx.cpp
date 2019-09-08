#include <windows.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polyhedron.h"
#include "matrix.h"
#include "input_controller.h"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevI_instance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );
  polyhedron torus( "torus.tg2" );

  matrix m;
  m.translate( 0, 0, 1000 );
  torus.update_pos( m );
  m.clear();
  
  m.translate( -torus.pos.wx, -torus.pos.wy, -torus.pos.wz );
  m.rotate_x( 0.04f );
  m.rotate_y( 0.03f );
  m.translate( torus.pos.wx, torus.pos.wy, torus.pos.wz );

  while( 1 )
  {
    if( input.check() ) break;
    if( input.event_key != 0 ) break;

    torus.update_pos( m );

    screen_interface.clear();
    torus.display();
    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
