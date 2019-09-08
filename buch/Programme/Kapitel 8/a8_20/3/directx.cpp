#include <windows.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polyhedron.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );
  polyhedron cube( "cube.tg2" );

  float angle = 2, angle_step = 0.02f;
  D3DXMATRIXA16 world_matrix, em;

  while( key_pressed() == 0 )
  {
    D3DXMatrixIdentity( &world_matrix );

    D3DXMatrixIdentity( &em );
    D3DXMatrixRotationX( &em, angle );
    D3DXMatrixMultiply( &world_matrix, &world_matrix, &em );

    D3DXMatrixIdentity( &em );
    D3DXMatrixRotationY( &em, angle );
    D3DXMatrixMultiply( &world_matrix, &world_matrix, &em );

    D3DXMatrixIdentity( &em );
    D3DXMatrixTranslation( &em, 0, 0, 20 );
    D3DXMatrixMultiply( &world_matrix, &world_matrix, &em );

    angle += angle_step;

    screen_interface.set_matrix( &world_matrix );

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