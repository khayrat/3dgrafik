#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_color.h"
#include "heightfield.h"

uchar act_pal = 0;
uchar rebuild_fractal = 0;

short lens_radius = 50;
double lens_scale = 1.5;
svertex lens_pos;

uchar handle_input( void );
void draw_lens( uchar *picture, pixel_32 *pal, pixel_32 *sbuffer );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten:\tBewegung der Linse auf dem Bildschirm\nLeertaste:\tWechsel der Farbpalette\nEnter:\t\tWechsel des Fraktals\n\nEscape:\t\tProgramm beenden" );

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ x_res*y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  pixel_32 borders3[ 3 ] = {  pixel_32( 40, 40, 255 ), pixel_32( 128, 128, 128 ), pixel_32( 255, 255, 255 )  };

  pixel_32 borders5[ 5 ] =
  {
    pixel_32(  40,  40, 255 ), pixel_32( 128, 128, 128 ),
    pixel_32( 255, 255, 255 ),
    pixel_32( 128, 128, 128 ), pixel_32(  40,  40, 255 )
  };

  primary_color palette[ 2 ];
  palette[ 0 ].load( 256, 3, borders3 );
  palette[ 1 ].load( 256, 5, borders5 );

  long grad = 500;

  heightfield clouds( x_res, y_res );
  clouds.plasma_fractal( grad );

  lens_pos = svertex( x_res/2, y_res/2 );

  long mx = (x_res - clouds.xscale) / 2;
  long my = (y_res - clouds.yscale) / 2;

  while( 1 )
  {
    if( handle_input() ) break;
    if( rebuild_fractal == 1 ) {  clouds.plasma_fractal( grad );  rebuild_fractal = 0;  }

    clouds.display( mx, my, palette[ act_pal ].components, sbuffer );

    draw_lens( clouds.heights, palette[ act_pal ].components, sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( sbuffer[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

void draw_lens( uchar *picture, pixel_32 *pal, pixel_32 *sbuffer )
{
  short r = lens_radius;

  long offset = (lens_pos.sy - r) * x_res + lens_pos.sx;

  for( short y = -r ; y <= r ; y++ )
  {
    short b = short( sqrt( r*r - y*y ) );

    for( short x = -b ; x <= b ; x++ )
    {
      long dx = long( lens_scale * x );
      long dy = long( lens_scale * y );
      long color = (dy+lens_pos.sy) * x_res + (dx+lens_pos.sx);

      sbuffer[ offset + x ] = pal[ picture[ color ] ];
   }

    offset += x_res;
  }
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  if( input.event_key == VK_SPACE ) if( ++act_pal > 1 ) act_pal = 0;
  if( input.event_key == VK_RETURN ) rebuild_fractal = 1;

  long s = 2;
  long radius = max( lens_radius, long( lens_radius * lens_scale ) + 2 );

  if( input.key_pressed( VK_LEFT  ) )
    if( (lens_pos.sx -= s) < radius+1 ) lens_pos.sx = radius+1;

  if( input.key_pressed( VK_RIGHT ) )
    if( (lens_pos.sx += s) > x_res-radius-1 ) lens_pos.sx = x_res-radius-1;

  if( input.key_pressed( VK_UP  ) )
    if( (lens_pos.sy -= s) < radius+1 ) lens_pos.sy = radius+1;

  if( input.key_pressed( VK_DOWN ) )
    if( (lens_pos.sy += s) > y_res-radius-1 ) lens_pos.sy = y_res-radius-1;

  return 0;
}