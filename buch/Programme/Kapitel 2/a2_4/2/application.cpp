#include <windows.h>

#include "screen_interface.h"
#include "palette.h"

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }  
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

MSG msg;
void draw_triangle( svertex points[ 3 ], uchar color, pixel_32 *screen );
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  svertex points[ 3 ];

  while( 1 )
  {
    for( long a=0 ; a<4 ; a++ )
    {
      for( long x=0 ; x<3 ; x++ )
         points[ x ] = svertex( rand() % x_res, rand() % y_res );

      draw_triangle( points, rand() % 250, screen );

      for( double wait=0 ; wait<9999999 ; wait++ )
         if( key_pressed() )
         {
           screen_interface.release_screen_pointer();
           return msg.wParam;
         }
    }

    for( long x=0 ; x<y_res*x_res ; x++ )
       screen[ x ] = palette[ 0 ];
  }
}

void draw_triangle( svertex points[ 3 ], uchar color, pixel_32 *screen )
{
  svertex act_point = points[ rand() % 3 ];

  for( long a=0 ; a<100000 ; a++ )
  {
    uchar index = rand() % 3;

    act_point.sx = (act_point.sx + points[ index ].sx) / 2;
    act_point.sy = (act_point.sy + points[ index ].sy) / 2;

    screen[ act_point.sy * x_res + act_point.sx ] = palette[ color ];
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
