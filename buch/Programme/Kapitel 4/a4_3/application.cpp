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

  double zmax, zmin, img_z, step;
  zmax = 300;  zmin = 50;
  img_z = zmax;  step = -0.5;

  matrix m;
  m.translate( 0, 0, zmax );
  octagon.update_pos( m );
  m.clear();  

  while( key_pressed() == 0 )
  {
    img_z += step;

    if( img_z >= zmax ) step = -0.5;
    else if( img_z <= zmin ) step = 0.5;

    double scale_factor = (img_z - step) / img_z;

    m.translate( -octagon.pos.wx, -octagon.pos.wy, -octagon.pos.wz );
    m.rotate_x( -0.009 );  m.rotate_y( 0.01 );  m.rotate_z( -0.01 );
    m.scale( scale_factor, scale_factor, scale_factor );
    m.translate( octagon.pos.wx, octagon.pos.wy, octagon.pos.wz );

    octagon.update_pos( m );
    m.clear();

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

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

