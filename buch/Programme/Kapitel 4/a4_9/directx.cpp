#include <windows.h>
#include <d3dx9.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "polygon.h"

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
  vertex( -12,  30,  0 ),
};

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmd_line, int cmd_show )
{
  screen_interface.open_window( instance, 640, 480 );

  polygon octagon( 8, pixel_32( 255, 255, 0 ), points );

  float zmax, zmin, img_z, step;
  zmax = 300;  zmin = 50;
  img_z = zmax;  step = -2;

  matrix m;
  m.translate( 0, 0, zmax );
  octagon.update_pos( m );
  m.clear();

  while( key_pressed() == 0 )
  {
    img_z += step;

    if( img_z >= zmax ) step = -2;
    else if( img_z <= zmin ) step = 2;

    float scale_factor = (img_z - step) / img_z;

    m.translate( -octagon.pos.wx, -octagon.pos.wy, -octagon.pos.wz );
    m.rotate_x( -0.009f );  m.rotate_y( 0.018f );  m.rotate_z( -0.015f );
    m.scale( scale_factor, scale_factor, scale_factor );
    m.translate( octagon.pos.wx, octagon.pos.wy, octagon.pos.wz );

    octagon.update_pos( m );
    m.clear();

    screen_interface.clear();

    screen_interface.backface_removal( 1 );
    octagon.display_polygon();

    screen_interface.backface_removal( 0 );
    octagon.display_shape();

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