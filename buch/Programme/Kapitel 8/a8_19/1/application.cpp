#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"
#include "window.h"

uchar handle_input( matrix *m );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
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

  message( "Steuerung:", "R, F, D, G, E, T:\tRotation des Viewports\nU, J, H, K, Z, I:\tVerschiebung des Viewports\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  window vl( 600, 450, 30, 70 );
  window vs( 300, 225, screen_x_res-300-31, screen_y_res-225-71 );

  polyhedron p[ 5 ];
  p[ 0 ].load( "icosahedron.tg2", 0 );
  p[ 1 ].load( "octahedron.tg2", 4 );
  p[ 2 ].load( "dodecahedron.tg2", 2 );
  p[ 3 ].load( "tetrahedron.tg2", 3 );
  p[ 4 ].load( "cube.tg2", 1 );

  matrix m;

  m.scale( 40, 40, 40 );  m.translate( -200, 0, 700 );  p[ 0 ].update_pos( m );  m.clear();
  m.scale( 30, 30, 30 );  m.translate(  200, 0, 700 );  p[ 1 ].update_pos( m );  m.clear();
  m.scale( 40, 40, 40 );  m.translate(    0, 0, 500 );  p[ 2 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  m.translate(  200, 0, 300 );  p[ 3 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  m.translate( -200, 0, 300 );  p[ 4 ].update_pos( m );  m.clear();

  uchar x;

  while( 1 )
  {
    if( handle_input( &m ) ) break;

    for( x=0 ; x<5 ; x++ ) p[ x ].update_pos( m );

    vl.install();
      for( x=0 ; x<5 ; x++ ) p[ x ].display( sbuffer );
    vl.display( screen, 1 );

    vs.install();
      for( x=0 ; x<5 ; x++ ) p[ x ].display( sbuffer );
    vs.display( screen, 1 );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( matrix *m )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  if( input.key_pressed( 'R' ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( 'F' ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( 'E' ) ) m->rotate_z( -0.01 );
  if( input.key_pressed( 'T' ) ) m->rotate_z(  0.01 );

  double step = 2;

  if( input.key_pressed( 'K' ) ) m->translate( -step,  0,     0    );
  if( input.key_pressed( 'H' ) ) m->translate(  step,  0,     0    );
  if( input.key_pressed( 'I' ) ) m->translate(  0,    -step,  0    );
  if( input.key_pressed( 'Z' ) ) m->translate(  0,     step,  0    );
  if( input.key_pressed( 'U' ) ) m->translate(  0,     0,    -step );
  if( input.key_pressed( 'J' ) ) m->translate(  0,     0,     step );

  return 0;
}
