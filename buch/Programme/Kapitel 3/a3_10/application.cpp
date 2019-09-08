#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

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

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  long pixel_count = x_res * y_res;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ pixel_count ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  polygon pentagon( 5, pixel_32( 255, 255, 0 ), points );

  const double zmax = 320, zmin = 60;

  matrix m[ 2 ];
  m[ 0 ].translate( 0, 0, zmax );
  pentagon.update_pos( m[ 0 ] );
  m[ 0 ].clear();

  m[ 0 ].translate( -pentagon.pos.wx, -pentagon.pos.wy, -pentagon.pos.wz );
  m[ 0 ].rotate_y( 0.03 );
  m[ 0 ].translate( pentagon.pos.wx, pentagon.pos.wy, pentagon.pos.wz - 3 );

  m[ 1 ].translate( -pentagon.pos.wx, -pentagon.pos.wy, -pentagon.pos.wz );
  m[ 1 ].rotate_y( 0.03 );
  m[ 1 ].translate( pentagon.pos.wx, pentagon.pos.wy, pentagon.pos.wz + 3 );

  char matrix_index = 0;

  while( key_pressed() == 0 )
  {
    if( pentagon.pos.wz >= zmax ) matrix_index = 0;
    else if( pentagon.pos.wz <= zmin ) matrix_index = 1;

    pentagon.update_pos( m[ matrix_index ] );

    long x;
    for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = pixel_32( 0, 0, 0 );

    pentagon.display( sbuffer );

    for( x=0 ; x<pixel_count ; x++ ) screen[ x ] = sbuffer[ x ];
  }
  
  if( sbuffer ) delete [] sbuffer;

  screen_interface.release_screen_pointer();

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

