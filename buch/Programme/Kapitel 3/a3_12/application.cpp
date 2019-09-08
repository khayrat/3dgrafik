#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex eight[ 8 ] =
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

vertex six[ 6 ] =
{
  vertex( -32,  0,   0 ),
  vertex( -16,  28,  0 ),
  vertex(  16,  28,  0 ),
  vertex(  32,  0,   0 ),
  vertex(  16, -28,  0 ),
  vertex( -16, -28,  0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  long pixel_count = x_res * y_res;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ pixel_count ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  polygon ps[ 3 ];
  
  ps[ 0 ].load( 8, pixel_32( 100, 10, 200 ), eight );
  ps[ 1 ].load( 6, pixel_32( 255, 255, 0 ), six );
  ps[ 2 ].load( 8, pixel_32( 100, 10, 200 ), eight );

  double zmax = 380, zmin = 80, step = -2;
  const double pi = 3.1415926535;

  matrix m;

  m.rotate_x( 0.5 * pi );
  m.translate( -40, 0, zmax );
  ps[ 0 ].update_pos( m );
  m.clear();

  m.translate( 0, 0, zmax );
  ps[ 1 ].update_pos( m );
  m.clear();

  m.rotate_x( 0.5 * pi );
  m.translate( 40, 0, zmax );
  ps[ 2 ].update_pos( m );
  m.clear();

  while( key_pressed() == 0 )
  {
    if( ps[ 1 ].pos.wz >= zmax ) step = -2;
    else if( ps[ 1 ].pos.wz <= zmin ) step = 2;

    m.translate( -ps[ 1 ].pos.wx, -ps[ 1 ].pos.wy, -ps[ 1 ].pos.wz );
    m.rotate_x( -0.021 );
    m.rotate_y( 0.012 );
    m.rotate_z( 0.018 );
    m.translate( ps[ 1 ].pos.wx, ps[ 1 ].pos.wy, ps[ 1 ].pos.wz + step );

    long x;
    
    for( x=0 ; x<3 ; x++ ) ps[ x ].update_pos( m );
    m.clear();

    for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = pixel_32( 0, 0, 0 );

    for( x=0 ; x<3 ; x++ ) ps[ x ].display( sbuffer );

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

