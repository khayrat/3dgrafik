#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "polyhedron.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );
void display_point( vertex wp, pixel_32 *screen );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  vertex sun;  sun.wx = 300;  sun.wy = 0;  sun.wz = 1200;
  
  polyhedron planet( "sphere.tg1" );

  matrix m;
  m.scale( 120, 120, 120 );
  m.translate( sun.wx, sun.wy, sun.wz + 300 );
  planet.update_pos( m );
  m.clear();
  
  m.translate( -sun.wx, -sun.wy, -sun.wz );
  m.rotate_y( 0.008 );
  m.translate( sun.wx, sun.wy, sun.wz );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( key_pressed() == 0 )
  {
    planet.update_pos( m );

    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    display_point( sun, sbuffer );
    planet.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return msg.wParam;
}

void display_point( vertex wp, pixel_32 *screen )
{
  svertex sp;

  if( wp.wz > 0.0 )
  {
    sp.sx = long( wp.wx / wp.wz *  pr_cnst + x_res/2 );
    sp.sy = long( wp.wy / wp.wz * -pr_cnst + y_res/2 );
  
    if( sp.sx > 0 && sp.sx < x_res && sp.sy > 0 && sp.sy < y_res )
    {
      long offset = sp.sy * x_res + sp.sx;
      double sz = wp.wz + clear_translation;

      if( sz > zbuffer[ offset ] ) return;

      screen[ offset ] = pixel_32( 255, 255, 0 );
      zbuffer[ offset ] = sz;
    }
  }
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

