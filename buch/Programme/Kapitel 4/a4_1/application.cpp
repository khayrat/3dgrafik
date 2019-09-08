#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 8 ] =
{
  vertex(  12,  30,  0 ),
  vertex(  30,  12,  0 ),
  vertex(  30, -12,  0 ),
  vertex(  12, -30,  0 ),
  vertex( -12, -30,  0 ),
  vertex( -30, -12,  0 ),
  vertex( -30,  12,  0 ),
  vertex( -12,  30,  0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon octagon( 8, pixel_32( 80, 0, 180 ), points );

  matrix m;
  m.translate( 0, 0, 50 );
  octagon.update_pos( m );
  m.clear();

  m.translate( -octagon.pos.wx,  -octagon.pos.wy,  -octagon.pos.wz );
  m.rotate_z( -0.01 );
  m.translate( octagon.pos.wx,  octagon.pos.wy,  octagon.pos.wz );

  while( key_pressed() == 0 )
  {
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    octagon.update_pos( m );

    octagon.display( sbuffer );

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

