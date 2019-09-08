#include <windows.h>
#include <d3dx9.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polyhedron.h"
#include "input_controller.h"

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevI_instance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );

  polyhedron cube( "thing.tg1" );

  matrix m;
  m.scale( 0.01f, 0.01f, 0.01f );
  m.translate( 0, 0, 5 );
  cube.update_pos( m );
  m.clear();  

  m.translate( -cube.pos.wx, -cube.pos.wy, -cube.pos.wz );
  m.rotate_x( 0.007f );
  m.rotate_y( 0.008f );
  m.rotate_z( 0.002f );
  m.translate( cube.pos.wx, cube.pos.wy, cube.pos.wz );

  screen_interface.backface_removal( 1 );

  while( 1 )
  {
    if( input.check() ) break;
    if( input.event_key != 0 ) break;

    cube.update_pos( m );

    screen_interface.clear();
    cube.display();
    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
