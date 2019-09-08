#include <windows.h>
#include <time.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "floating_polyhedron.h"
#include "matrix.h"

double n = 1.0;

double get_length( vertex v )
{
  if( n < 0.001 )
    exit_error( "Die Konstante n zur Berechnung der Länge von Ortsvektoren besitzt einen ungültigen Wert.\n ");

  double base = pow(fabs(v.wx),n) + pow(fabs(v.wy),n) + pow(fabs(v.wz),n);

  return pow( base, 1/n );
}

uchar handle_input( matrix *m, vertex *pos );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\tVerschiebung in Richtung der Weltachsen\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  polyhedron borders[ 7 ];  
  
  borders[ 0 ].sphere( 44 );
  borders[ 1 ].sphere( 44 );
  borders[ 2 ].torus( 44, 3 );
  borders[ 3 ].cone( 44, 3, 5 );
  borders[ 4 ].cone( 44, 3, 3 );
  borders[ 5 ].sphere( 44 );
  borders[ 6 ].cylinder( 44, 3 );

  n = 0.7;  borders[ 0 ].set_unit_length( get_length );
  n = 1;    borders[ 5 ].set_unit_length( get_length );

  matrix m;

  m.scale( 20, 20, 20 );  borders[ 0 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  borders[ 1 ].update_pos( m );  m.clear();
  m.scale( 5, 5, 5 );     borders[ 2 ].update_pos( m );  m.clear();
  m.scale( 0.45, 0.45, 0.45 );   m.translate( 0, 10, 0 );   borders[ 3 ].update_pos( m );  m.clear();
  m.scale( 0.45, -0.45, 0.45 );  m.translate( 0, -10, 0 );  borders[ 4 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  borders[ 5 ].update_pos( m );  m.clear();
  m.scale( 5, 0.9, 5 );     borders[ 6 ].update_pos( m );  m.clear();

  floating_polyhedron fp;
  fp.load( 7, borders );

  m.translate( 0, 0, 80 );
  fp.update_pos( m );

  srand( time( NULL ) );

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
