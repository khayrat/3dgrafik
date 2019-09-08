#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polyhedron.h"

MSG msg;
uchar key_pressed( void );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );

  glClearColor( 1, 1, 1, 0 );

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

  while( key_pressed() == 0 )
  {  
    cube.update_pos( m );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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
