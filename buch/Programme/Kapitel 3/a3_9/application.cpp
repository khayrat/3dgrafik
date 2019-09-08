#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );
void display_point( vertex wp, pixel_32 *screen );

vertex points[ 6 ] =
{
  vertex( -20, -87,  0 ),
  vertex( -93,  5,   0 ),
  vertex( -30,  69,  0 ),
  vertex(  57,  84,  0 ),
  vertex(  94, -24,  0 ),
  vertex(  60, -75,  0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon hexagon( 6, pixel_32( 70, 3, 130 ), points );

  matrix m;

  m.translate( 100, 0, 200 );
  hexagon.update_pos( m );
  m.clear();

  vertex p( 100, 0, 500 );

  m.translate( -p.wx, -p.wy, -p.wz );
  m.rotate_y( 0.008 );
  m.translate( p.wx, p.wy, p.wz );

  long pixel_count = x_res * y_res;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ pixel_count ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  while( key_pressed() == 0 )
  {
    hexagon.update_pos( m );

    long x;
    for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = pixel_32( 0, 0, 0 );

    hexagon.display( sbuffer );
    display_point( p, sbuffer );

    for( x=0 ; x<pixel_count ; x++ ) screen[ x ] = sbuffer[ x ];
  }
  
  if( sbuffer ) delete [] sbuffer;

  screen_interface.release_screen_pointer();

  return msg.wParam;
}

void display_point( vertex wp, pixel_32 *screen )
{
  svertex sp;

  if( wp.wz > 0.0 )
  {
    sp.sx = long( wp.wx / wp.wz *  200.0 + x_res/2 );
    sp.sy = long( wp.wy / wp.wz * -200.0 + y_res/2 );

    if( sp.sx > 0 && sp.sx < x_res-1 && sp.sy > 0 && sp.sy < y_res-1 )
      screen[ sp.sy * x_res + sp.sx ] = pixel_32( 255, 255, 255 );
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

