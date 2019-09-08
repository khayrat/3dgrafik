#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_32.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res-1, y_res-1, 0 );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glEnable( GL_TEXTURE_2D );

  glClearColor( 1, 1, 1, 0 );

  bitmap_32 blue( "blue.bmp" );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

	glClear( GL_COLOR_BUFFER_BIT );

    blue.display( (x_res - blue.xscale) / 2, (y_res - blue.yscale) / 2 );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

