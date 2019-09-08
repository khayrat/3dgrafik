#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"

pixel_32 palette[ 256 ];

MSG msg;
uchar key_pressed( void );

void load_palette( void );
void white_background( void );
void show_palette( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res, y_res, 0 );

  load_palette();

  white_background();

  while( 1 )
  {
    if( key_pressed() ) break;

    show_palette();
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

void white_background( void )
{
  glColor3ub( 255, 255, 255 );

  glBegin( GL_POINTS );

    for( long x=0 ; x<x_res ; x++ )
       for( long y=0 ; y<y_res ; y++ )
          glVertex2d( x, y );

  glEnd();
}

void show_palette( void )
{
  long start_x = (x_res - 256) / 2;
  long start_y = (y_res - 200) / 2;

  glBegin( GL_POINTS );

    for( long x=0 ; x<256 ; x++ )
    {
      glColor3ub( palette[ x ].red, palette[ x ].green, palette[ x ].blue );

      for( long y=0 ; y<200 ; y++ )
         glVertex2d( x + start_x, y + start_y );
    }

  glEnd();
}
