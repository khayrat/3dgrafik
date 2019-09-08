#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "point_cloud.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();
  
  point_cloud tetrahedron;
  tetrahedron.chaos_game( 100000 );

  matrix m;
  m.scale( 10, 10, 10 );
  m.translate( 0, 0, 200 );
  tetrahedron.update_pos( m );
  m.clear();

  m.translate( -tetrahedron.pos.wx, -tetrahedron.pos.wy, -tetrahedron.pos.wz );
  m.rotate_y( 0.05 );
  m.rotate_z( 0.03 );
  m.translate( tetrahedron.pos.wx, tetrahedron.pos.wy, tetrahedron.pos.wz );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( key_pressed() == 0 )
  {
    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    tetrahedron.update_pos( m );
    tetrahedron.display( sbuffer );

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

