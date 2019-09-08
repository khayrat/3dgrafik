#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"

struct rectangle
{
  long xt, yt, xb, yb;

  rectangle( long x1, long y1, long x2, long y2 ) : xt( x1 ), yt( y1 ), xb( x2 ), yb( y2 ) { }
};

pixel_32 palette[ 256 ];

MSG msg;
uchar key_pressed( void );
void frame_point( rectangle a, rectangle i, long *x, long *y );

void load_palette( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res, y_res, 0 );

  load_palette();

  rectangle a( 80, 80, 560, 400 );
  rectangle i( 160, 150, 480, 330 );

  while( 1 )
  {
    if( key_pressed() ) break;

    long x, y;
    pixel_32 c;

    glBegin( GL_POINTS );

      c = palette[ 64 + (rand() % 64) ];
      glColor3ub( c.red, c.green, c.blue );
      frame_point( a, i, &x, &y );
      glVertex2i( x, y );

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

void load_palette( void )
{
  long x;
  uchar c;

  for( x=0, c=0 ; x<64 ; x++, c+=4 )
  {
    palette[ x     ] = pixel_32( c, 0, 0 );
    palette[ x+64  ] = pixel_32( c, c, c );
    palette[ x+128 ] = pixel_32( 0, c, 0 );
    palette[ x+192 ] = pixel_32( 0, 0, c );
  }
}

void frame_point( rectangle a, rectangle i, long *x, long *y )
{
  do
  {
    *x = a.xt + (rand() % (a.xb - a.xt + 1));
    *y = a.yt + (rand() % (a.yb - a.yt + 1));
  } while( *x >= i.xt && *x <= i.xb && *y >= i.yt && *y <= i.yb );
}