#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

long act_sphere = 0;

double n = 1.0;

double get_length( vertex v )
{
  if( n < 0.001 )
    exit_error( "Die Konstante n zur Berechnung der Länge von Ortsvektoren besitzt einen ungültigen Wert.\n ");

  double base = pow(fabs(v.wx),n) + pow(fabs(v.wy),n) + pow(fabs(v.wz),n);

  return pow( base, 1/n );
}

uchar handle_input( vertex pos, matrix *m );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\t\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\t\tVerschiebung in Richtung der Weltachsen\n\nPfeiltaste links:\t\tVorherige Sphäre\nPfeiltaste rechts:\t\tNächste Sphäre\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );

  polyhedron p[ 5 ];

  p[ 0 ].sphere( 80 );  n = 0.7;  p[ 0 ].set_unit_length( get_length );
  p[ 1 ].sphere( 80 );  n = 1;    p[ 1 ].set_unit_length( get_length );
  p[ 2 ].sphere( 80 );  n = 1.3;  p[ 2 ].set_unit_length( get_length );
  p[ 3 ].sphere( 80 );  n = 2;    p[ 3 ].set_unit_length( get_length );
  p[ 4 ].sphere( 80 );  n = 5;    p[ 4 ].set_unit_length( get_length );

  matrix m;

  m.scale( 40, 40, 40 );
  m.translate( 0, 0, 180 );
  for( long x=0 ; x<3 ; x++ ) p[ x ].update_pos( m );
  m.translate( 0, 0, 70 );
  p[ 3 ].update_pos( m );  p[ 4 ].update_pos( m );

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( handle_input( p[ act_sphere ].pos, &m ) == 1 ) break;

    p[ act_sphere ].update_pos( m );
    p[ act_sphere ].display_polygons();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

uchar handle_input( vertex pos, matrix *m )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  if( input.key_pressed( 'R' ) ) m->rotate_x(  0.02 );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -0.02 );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  0.02 );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -0.02 );
  if( input.key_pressed( 'E' ) ) m->rotate_z(  0.02 );
  if( input.key_pressed( 'T' ) ) m->rotate_z( -0.02 );

  m->translate( pos.wx, pos.wy, pos.wz );

  if( input.key_pressed( 'H' ) ) m->translate( -1,  0,  0 );
  if( input.key_pressed( 'K' ) ) m->translate(  1,  0,  0 );
  if( input.key_pressed( 'Z' ) ) m->translate(  0, -1,  0 );
  if( input.key_pressed( 'I' ) ) m->translate(  0,  1,  0 );
  if( input.key_pressed( 'J' ) ) m->translate(  0,  0, -1 );
  if( input.key_pressed( 'U' ) ) m->translate(  0,  0,  1 );

  if( input.event_key == VK_LEFT  ) {  --act_sphere;  if( act_sphere < 0 ) act_sphere = 4;  }
  if( input.event_key == VK_RIGHT ) {  ++act_sphere;  if( act_sphere > 4 ) act_sphere = 0;  }

  return 0;
}