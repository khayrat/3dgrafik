#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "floating_polyhedron.h"
#include "matrix.h"

uchar handle_input( matrix *m, vertex *pos );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\tVerschiebung in Richtung der Weltachsen\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  polyhedron begin;  begin.sphere( 44 );
  polyhedron end;    end.torus( 44, 3 );

  matrix m;

  m.scale( 20, 20, 20 );  begin.update_pos( m );  m.clear();
  m.scale(  5,  5,  5 );  end.update_pos( m );    m.clear();

  floating_polyhedron fp;
  fp.load( &begin, &end );

  m.translate( 0, 0, 80 );
  fp.update_pos( m );

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( handle_input( &m, &fp.pos ) ) break;

    fp.update_pos( m );
    fp.display();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

uchar handle_input( matrix *m, vertex *pos )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos->wx, -pos->wy, -pos->wz );

  if( input.key_pressed( 'R' ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( 'E' ) ) m->rotate_z(  0.01 );
  if( input.key_pressed( 'T' ) ) m->rotate_z( -0.01 );

  m->translate( pos->wx, pos->wy, pos->wz );

  if( input.key_pressed( 'H' ) ) m->translate( -1,  0,  0 );
  if( input.key_pressed( 'K' ) ) m->translate(  1,  0,  0 );
  if( input.key_pressed( 'Z' ) ) m->translate(  0, -1,  0 );
  if( input.key_pressed( 'I' ) ) m->translate(  0,  1,  0 );
  if( input.key_pressed( 'J' ) ) m->translate(  0,  0, -1 );
  if( input.key_pressed( 'U' ) ) m->translate(  0,  0,  1 );

  return 0;
}
