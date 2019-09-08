#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

uchar handle_input( vertex pos, matrix *m );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\tVerschiebung in Richtung der Weltachsen\n\nEscape:\t\tEnde des Programms\n ");

  if
  (
    MessageBox
    ( 
      NULL, 
      "Dieses Programm schaltet auf Ihrem Bildschirm die Auflösung 1024x768 Pixel mit der Farbtiefe 32 Bit.\nWollen Sie fortfahren?", "Wichtiger Hinweis:",
      MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2
    )

    == IDNO
  ) return 0;

  initialise_world( hInstance, 1024, 768, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p;
  p.plane( 10, "metal.bmp" );

  matrix m;

  m.rotate_y( 0.2 );
  m.rotate_x( -0.5 );
  m.translate( 0, 0, 30 );
  p.update_pos( m );
  m.clear();

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = 0;

  while( 1 )
  {
    if( clear_translation > max_clear_translation ) clear_translation += (1.0 / z_min);
    else
    {
      memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
      clear_translation = 0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    if( handle_input( p.pos, &m ) ) break;

    p.update_pos( m );
    p.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( vertex pos, matrix *m )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  double angle = 0.05;

  if( input.key_pressed( 'R' ) ) m->rotate_x(  angle );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -angle );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  angle );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -angle );
  if( input.key_pressed( 'E' ) ) m->rotate_z(  angle );
  if( input.key_pressed( 'T' ) ) m->rotate_z( -angle );

  m->translate( pos.wx, pos.wy, pos.wz );

  double step = 0.5;

  if( input.key_pressed( 'H' ) ) m->translate( -step,  0,     0    );
  if( input.key_pressed( 'K' ) ) m->translate(  step,  0,     0    );
  if( input.key_pressed( 'Z' ) ) m->translate(  0,    -step,  0    );
  if( input.key_pressed( 'I' ) ) m->translate(  0,     step,  0    );
  if( input.key_pressed( 'J' ) ) m->translate(  0,     0,    -step );
  if( input.key_pressed( 'U' ) ) m->translate(  0,     0,     step );

  return 0;
}
