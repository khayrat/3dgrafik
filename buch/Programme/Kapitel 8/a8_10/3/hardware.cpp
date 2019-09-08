#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "texture.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res-1, y_res-1, 0 );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glEnable( GL_TEXTURE_2D );

  glClearColor( 1, 1, 1, 0 );

  texture blue( "blue.bmp" );

  long top_x = long( (x_res - blue.xscale) / 2 );
  long top_y = long( (y_res - blue.yscale) / 2 );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    glClear( GL_COLOR_BUFFER_BIT );

    blue.activate();

    glBegin( GL_POLYGON );

      glTexCoord2d( 0, 0 );  glVertex2d( top_x,               top_y );
      glTexCoord2d( 1, 0 );  glVertex2d( top_x + blue.xscale, top_y );
      glTexCoord2d( 1, 1 );  glVertex2d( top_x + blue.xscale, top_y + blue.yscale );
      glTexCoord2d( 0, 1 );  glVertex2d( top_x,               top_y + blue.yscale );

    glEnd();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
