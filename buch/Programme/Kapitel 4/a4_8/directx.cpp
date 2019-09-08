#include <windows.h>
#include <d3dx9.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polyhedron.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevI_instance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );

  polyhedron dodecahedron( "dodecahedron.tg1" );

  matrix m;
  m.scale( 10, 10, 10 );
  m.translate( 0, 0, 700 );
  dodecahedron.update_pos( m );
  m.clear();  

  m.translate( -dodecahedron.pos.wx, -dodecahedron.pos.wy, -dodecahedron.pos.wz );
  m.rotate_x( 0.007f );
  m.rotate_y( 0.008f );
  m.rotate_z( 0.002f );
  m.translate( dodecahedron.pos.wx, dodecahedron.pos.wy, dodecahedron.pos.wz );

  screen_interface.backface_removal( 1 );

  while( key_pressed() == 0 )
  {
    dodecahedron.update_pos( m );

    screen_interface.clear();
    dodecahedron.display();
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