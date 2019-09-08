#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polyhedron.h"
#include "input_controller.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 640, 480, 32 );

  glClearColor( 1, 1, 1, 0 );

  polyhedron cube;
  cube.load( "cube.tg3" );

  double angle = 0, angle_step = 0.5;

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;
  
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glTranslated( 0, 0, 500 );
    glRotated( angle, 0, 1, 0 );
    glRotated( angle, 1, 0, 0 );

    angle += angle_step;

    cube.display();

	screen_interface.swap_buffers();
  }
  
  return input.msg.wParam;
}
