#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "landscape.h"
#include "matrix.h"
#include "viewport.h"

vertex begin( 1.75, 0, 2.75 );
vertex end( -1, 0, -3 );

char s = 'l';

uchar handle_input( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tBewegung des gelb dargestellten Anfangspunktes\nU, J, H, K, Z, I:\tBewegung des blau dargestellten Endpunktes\n\nEnter:\t\tÄnderung der Kategorie der Polygonseite\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  double length = 0;
  landscape planet;  planet.load( 15, length );

  matrix m;  
  m.rotate_x( -0.5*3.1415926535 );
  m.translate( 0, 0, 22 );
  m.adjust_hardware(); 

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( handle_input() ) break;

    planet.display_side( begin, end, s );
    planet.display_points();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  double step = 0.01;

  if( input.key_pressed( 'R' ) ) begin.wz += step;
  if( input.key_pressed( 'F' ) ) begin.wz -= step;
  if( input.key_pressed( 'G' ) ) begin.wx += step;
  if( input.key_pressed( 'D' ) ) begin.wx -= step;
  if( input.key_pressed( 'U' ) ) end.wz += step;
  if( input.key_pressed( 'J' ) ) end.wz -= step;
  if( input.key_pressed( 'H' ) ) end.wx -= step;
  if( input.key_pressed( 'K' ) ) end.wx += step;

  if( input.key_pressed( VK_ESCAPE ) ) return 1;

  if( input.event_key == VK_RETURN ) {  if( s == 'l' ) s = 'r';  else s = 'l';  }

  return 0;
}
