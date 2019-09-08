#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "texture.h"

double xscale, yscale;

uchar handle_input( void );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten nach links und rechts:\tHorizontale Skalierung\nPfeiltasten nach oben und unten:\tVertikale Skalierung\n\nEscape:\t\t\t\tEnde des Programms\n ");

  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res-1, y_res-1, 0 );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glEnable( GL_TEXTURE_2D );

  glClearColor( 1, 1, 1, 0 );

  texture tx( "blue.bmp" );

  xscale = tx.xscale;
  yscale = tx.yscale;

  while( 1 )
  {
    if( handle_input() == 1 ) break;

	glClear( GL_COLOR_BUFFER_BIT );

    tx.activate();

    long top_x = long( (x_res - xscale) / 2 );
    long top_y = long( (y_res - yscale) / 2 );

    glBegin( GL_POLYGON );

      glTexCoord2d( 0, 0 );  glVertex2d( top_x,          top_y );
      glTexCoord2d( 1, 0 );  glVertex2d( top_x + xscale, top_y );
      glTexCoord2d( 1, 1 );  glVertex2d( top_x + xscale, top_y + yscale );
      glTexCoord2d( 0, 1 );  glVertex2d( top_x,          top_y + yscale );

    glEnd();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.key_pressed( VK_ESCAPE ) ) return 1;

  double step = 0.8;

  if( input.key_pressed( VK_LEFT  ) ) if( (xscale -= step) < 15 ) xscale = 15;
  if( input.key_pressed( VK_RIGHT ) ) if( (xscale += step) > 500 ) xscale = 500;
  if( input.key_pressed( VK_UP    ) ) if( (yscale -= step) < 15 ) yscale = 15;
  if( input.key_pressed( VK_DOWN  ) ) if( (yscale += step) > 500 ) yscale = 500;

  return 0;
}