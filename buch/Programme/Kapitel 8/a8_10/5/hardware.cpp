#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "texture.h"
#include "marble.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res-1, y_res-1, 0 );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

  glClearColor( 1, 1, 1, 0 );

  window_pos = svertex( (x_res - window_size) / 2, (y_res - window_size) / 2 );

  texture blue( "blue.bmp" );

  marble m( &blue );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

	glClear( GL_COLOR_BUFFER_BIT );

    display_window();

    m.update_pos();
    m.display();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
