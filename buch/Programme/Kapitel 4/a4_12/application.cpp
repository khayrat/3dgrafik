#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "polyhedron.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );
void display_point( vertex wp, pixel_32 *screen );

// Vor allem bei hohen Auflösungen ab 1024x768 merkt man einen deutlichen Geschwindigkeitsvorteil.
// Kommentiert man if und else innerhalb der Hauptschleife aus, wird das Clear Reduction 
// ausgeschaltet und die Ausführungsgeschwindigkeit des Programms wird verringert.

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

  initialise_world( hInstance, 1024, 768, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p( "open.tg1" );

  matrix m;
  m.rotate_y( -1.2 );
  m.scale( 10, 10, 10 );
  m.translate( 0, 0, 80 );
  p.update_pos( m );
  m.clear();  

  m.translate( -p.pos.wx, -p.pos.wy, -p.pos.wz );
  m.rotate_y( 0.008 );
  m.translate( p.pos.wx, p.pos.wy, p.pos.wz );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( key_pressed() == 0 )
  {
    p.update_pos( m );

    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    p.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return msg.wParam;
}

uchar key_pressed( void )
{
  if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    if( msg.message == WM_QUIT || msg.message == WM_KEYDOWN ) return 1;

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return 0;
}

