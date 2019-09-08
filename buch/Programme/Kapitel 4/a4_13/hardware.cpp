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

  polyhedron t;
  t.load( "thing.tg1" );

  matrix m;
  m.scale( 80, 80, 80 );
  m.translate( 0, 0, 130 );
  t.update_pos( m );
  m.clear();

  m.translate( -t.pos.wx, -t.pos.wy, -t.pos.wz );
  m.rotate_x( 0.007 );
  m.rotate_y( 0.008 );
  m.rotate_z( 0.002 );
  m.translate( t.pos.wx, t.pos.wy, t.pos.wz );

  while( key_pressed() == 0 )
  {  
    t.update_pos( m );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    t.display();

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
