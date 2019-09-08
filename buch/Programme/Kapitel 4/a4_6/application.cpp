#include <windows.h>
#include <time.h>

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

  polyhedron p[ 5 ];
  p[ 0 ].load( "tetrahedron.tg1" );
  p[ 1 ].load( "cube.tg1" );
  p[ 2 ].load( "octahedron.tg1" );
  p[ 3 ].load( "icosahedron.tg1" );
  p[ 4 ].load( "dodecahedron.tg1" );

  matrix m[ 5 ];

  m[ 0 ].translate( -100,  90, 800 );  p[ 0 ].update_pos( m[ 0 ] );  m[ 0 ].clear();
  m[ 0 ].translate(    0,   0, 800 );  p[ 1 ].update_pos( m[ 0 ] );  m[ 0 ].clear();
  m[ 0 ].translate(  100,  90, 800 );  p[ 2 ].update_pos( m[ 0 ] );  m[ 0 ].clear();
  m[ 0 ].translate(  100, -90, 800 );  p[ 3 ].update_pos( m[ 0 ] );  m[ 0 ].clear();
  m[ 0 ].translate( -100, -90, 800 );  p[ 4 ].update_pos( m[ 0 ] );  m[ 0 ].clear();

  srand( time( NULL ) );

  long x;

  for( x=0 ; x<5 ; x++ )
  {
    m[ x ].translate( -p[ x ].pos.wx, -p[ x ].pos.wy, -p[ x ].pos.wz );
    m[ x ].rotate_x( ((rand() % 20) - 10) * 0.001 );
    m[ x ].rotate_y( ((rand() % 20) - 10) * 0.001 );
    m[ x ].rotate_z( ((rand() % 20) - 10) * 0.001 );
    m[ x ].translate( p[ x ].pos.wx, p[ x ].pos.wy, p[ x ].pos.wz );
  }

  while( key_pressed() == 0 )
  {
    for( x=0 ; x<5 ; x++ ) p[ x ].update_pos( m[ x ] );

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    for( x=0 ; x<5 ; x++ ) p[ x ].display( sbuffer );

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

