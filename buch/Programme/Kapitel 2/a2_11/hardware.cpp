#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "palette.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res, y_res, 0 );

  while( 1 )
  {
    if( key_pressed() ) break;

    int sx = rand() % x_res;
    int sy = rand() % y_res;
    pixel_32 c = palette[ rand() % 16 + 16 ];

    glColor3ub( c.red, c.green, c.blue );

    glBegin( GL_POINTS );

      glVertex2d( sx, sy );

    glEnd();
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
