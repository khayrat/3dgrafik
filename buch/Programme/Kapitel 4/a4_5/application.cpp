#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "polyhedron.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron cube( "cube.tg1" );

  matrix m;
  m.scale( 50, 50, 50 );
  m.translate( 0, 0, 80 );
  cube.update_pos( m );
  m.clear();  

  m.translate( -cube.pos.wx, -cube.pos.wy, -cube.pos.wz );
  m.rotate_x( 0.007 );
  m.rotate_y( 0.008 );
  m.rotate_z( 0.002 );
  m.translate( cube.pos.wx, cube.pos.wy, cube.pos.wz );

  while( key_pressed() == 0 )
  {
    cube.update_pos( m );

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    cube.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

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

