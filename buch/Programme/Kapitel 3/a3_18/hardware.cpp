#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polygon.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 5 ] =
{
  vertex( -29,   9,  0 ),
  vertex(   0,  30,  0 ),
  vertex(  29,   9,  0 ),
  vertex(  18, -24,  0 ),
  vertex( -18, -24,  0 )
};

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );

  glClearColor( 0, 0, 0, 0 );

  polygon pentagon( 5, pixel_32( 255, 255, 0 ), points );

  double zmax = 320, zmin = 50, step = -2;

  matrix m;
  m.translate( 0, 0, zmax );
  pentagon.update_pos( m );
  m.clear();

  while( key_pressed() == 0 )
  {
    if( pentagon.pos.wz >= zmax ) step = -2;
    else if( pentagon.pos.wz <= zmin ) step = 2;

    m.translate( -pentagon.pos.wx, -pentagon.pos.wy, -pentagon.pos.wz );
    m.rotate_x( -0.021 );
    m.rotate_y( 0.012 );
    m.rotate_z( 0.018 );
    m.translate( pentagon.pos.wx, pentagon.pos.wy, pentagon.pos.wz + step );
    
    pentagon.update_pos( m );
    m.clear();

	glClear( GL_COLOR_BUFFER_BIT );

    pentagon.display();

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
