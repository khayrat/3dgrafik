#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "polyhedron.h"
#include "matrix.h"

// falls die Planeten nicht rund erscheinen sollten, ist die Projektionskonstante pr_cnst zu klein

MSG msg;
uchar key_pressed( void );
void display_point( vertex wp, pixel_32 *screen );

double system_angle = 0.5236;

class planet
{
  private:
    polyhedron t;
    matrix m;
  
  public:
    void load( double dist, double angle, double scale_factor, vertex sun );
    void display( pixel_32 *sbuffer ) {  t.update_pos( m );  t.display( sbuffer );  }
};

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
  
  const long planet_count = 5;
  vertex sun;  sun.wx = 0;  sun.wy = 0;  sun.wz = 5500;
  
  planet planets[ planet_count ];
  planets[ 0 ].load( 300,  0.1,  50, sun );
  planets[ 1 ].load( 700,  0.08, 80, sun );
  planets[ 2 ].load( 1000, 0.05, 90, sun );
  planets[ 3 ].load( 1400, 0.03, 200, sun );
  planets[ 4 ].load( 1900, 0.02, 30, sun );

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( key_pressed() == 0 )
  {
    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    display_point( sun, sbuffer );
    for( long x=0 ; x<planet_count ; x++ )
       planets[ x ].display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return msg.wParam;
}

void planet::load( double dist, double angle, double scale_factor, vertex sun )
{
  angle *= 0.3;

  t.load( "sphere.tg1" );	
	
  m.scale( scale_factor, scale_factor, scale_factor );
  m.translate( 0, 0, dist );
  m.rotate_x( -system_angle );
  m.translate( sun.wx, sun.wy, sun.wz );
  t.update_pos( m );
  m.clear();

  m.translate( -sun.wx, -sun.wy, -sun.wz );
  m.rotate_x( system_angle );
  m.rotate_y( angle );
  m.rotate_x( -system_angle );
  m.translate( sun.wx, sun.wy, sun.wz );
}

void display_point( vertex wp, pixel_32 *screen )
{
  svertex sp;

  if( wp.wz > 0.0 )
  {
    sp.sx = long( wp.wx / wp.wz *  pr_cnst + x_res/2 );
    sp.sy = long( wp.wy / wp.wz * -pr_cnst + y_res/2 );
  
    if( sp.sx > 0 && sp.sx < x_res && sp.sy > 0 && sp.sy < y_res )
    {
      long offset = sp.sy * x_res + sp.sx;
      double sz = wp.wz + clear_translation;

      if( sz > zbuffer[ offset ] ) return;

      screen[ offset ] = pixel_32( 255, 255, 0 );
      zbuffer[ offset ] = sz;
    }
  }
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

