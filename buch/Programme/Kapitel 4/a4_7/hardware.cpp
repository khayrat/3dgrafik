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

  polyhedron icosahedron;
  icosahedron.load( "icosahedron.tg1" );

  matrix m;
  m.translate( 0, 0, 100 );
  icosahedron.update_pos( m );
  m.clear();

  m.translate( -icosahedron.pos.wx, -icosahedron.pos.wy, -icosahedron.pos.wz );
  m.rotate_x( 0.007 );
  m.rotate_y( 0.008 );
  m.rotate_z( 0.002 );
  m.translate( icosahedron.pos.wx, icosahedron.pos.wy, icosahedron.pos.wz );

  while( key_pressed() == 0 )
  {  
    icosahedron.update_pos( m );

	glClear( GL_COLOR_BUFFER_BIT );

    icosahedron.display();

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
