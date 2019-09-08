#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_bitmap.h"
#include "font.h"

long act_bitmap = 0;
long act_border = 0;
long border_count = 0;

bitmap_32 *borders;

void load_borders( char *filename );
uchar handle_input( primary_bitmap *adv_surface );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Horizontale Pfeiltasten:\tVeränderung der aktuellen Position\n\t\t\tinnerhalb des Bitmapverlaufes\nVertikale Pfeiltasten:\tWechsel der angezeigten Grenzbitmap\n\nEscape:\t\t\tEnde des Programms\n ");

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();
  
  pixel_32 *sbuffer = new pixel_32[ x_res*y_res ];

  if( sbuffer == NULL )
    exit_error( "*sbuffer: Fehler während der Reservierung von Arbeitsspeicher.\n" );

  primary_bitmap adv_surface( 64, "texture_lib.txt" );

  load_borders( "texture_lib.txt" );

  while( 1 )
  {
    if( handle_input( &adv_surface ) == 1 ) break;

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    borders[ act_border ].display(  45, 150, sbuffer );
    adv_surface.bitmaps[ act_bitmap ].display( 340, 150, sbuffer );

    char string[ 100 ];
    sprintf( string, "%ld. vorgegebene bitmap", (act_border+1) );
    chars.write( 50, 100, string, sbuffer );
    sprintf( string, "bitmap an der position %ld", act_bitmap );
    chars.write( 345, 100, string, sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

uchar handle_input( primary_bitmap *adv_surface )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  if( input.event_key == VK_UP ) 
    if( ++act_border > border_count-1 ) act_border = border_count-1;

  if( input.event_key == VK_DOWN ) if( --act_border < 0 ) act_border = 0;

  if( input.event_key == VK_LEFT ) if( --act_bitmap < 0 ) act_bitmap = 0;

  if( input.event_key == VK_RIGHT )
    if( ++act_bitmap > adv_surface->bitmap_count-1 )
      act_bitmap = adv_surface->bitmap_count-1;

  return 0;
}

void load_borders( char *filename )
{
  FILE *input = fopen( filename, "rb" );
  if( input == NULL ) exit_nofile( "load_borders()", filename );

  fscanf( input, "%ld", &border_count );

  if( (borders = new bitmap_32[ border_count ]) == NULL )
    exit_error( "*borders: Fehler während der Reservierung von Arbeitsspeicher.\n" );

  char string[ 100 ];  
  for( long x=0 ; x<border_count ; x++ )
  {  
    fscanf( input, "%s", string );
    borders[ x ].load( string );
  }

  fclose( input );
}