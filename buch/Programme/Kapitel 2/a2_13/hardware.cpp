#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"

pixel_32 palette[ 256 ];

MSG msg;
uchar key_pressed( void );
void load_palette( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, 640, 480, 0 );

  load_palette();

  const double pi = 3.1415926535;

  uchar running = 1;
  while( running )
  {
    long mx = (rand() % 638) + 2;
    long my = (rand() % 478) + 2;
    long n = (rand() % 8) + 3;

    pixel_32 c = palette[ rand() % 256 ];
    long r = rand() % min( min( mx, 640-mx ), min( my, 480-my ) );

    glBegin( GL_LINE_LOOP );

      glColor3ub( c.red, c.green, c.blue );

      for( long i=0 ; i<n ; i++ )
      {
        double x = r * cos( i * 2*pi / n );
        double y = r * sin( i * 2*pi / n );

        glVertex2d( mx + x, my + y );
      }

    glEnd();

    glFinish();

    for( double wait=0 ; wait < 9999999 ; wait++ )
       if( key_pressed() ) {  running = 0;  break;  }
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

void load_palette( void )
{
  long x;
  uchar c = 64;

  for( x=0 ; x<64 ; x++, c+=3 )
  {
    palette[ x     ] = pixel_32( c, 0, 0 );
    palette[ x+64  ] = pixel_32( c, c, c );
    palette[ x+128 ] = pixel_32( 0, c, 0 );
    palette[ x+192 ] = pixel_32( 0, 0, c );
  }
}
