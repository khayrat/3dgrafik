#include <windows.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polyhedron.h"
#include "hardware_matrix.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );
  polyhedron cube( "cube.tg2" );

  float angle = 0, angle_step = 0.02f;
  hardware_matrix hm;

  while( key_pressed() == 0 )
  {
    hm.rotate_x( angle );
    hm.rotate_y( angle );
    angle += angle_step;
    
    hm.translate( 0, 0, 20 );

    screen_interface.set_matrix( hm );

    hm.clear();

    screen_interface.clear();
    cube.display();
    screen_interface.swap_buffers();
  }

  return msg.wParam;
}

uchar key_pressed( void )
{
  if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    if( msg.message == WM_QUIT || msg.message == WM_KEYDOWN ) return 1;

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return 0;
}