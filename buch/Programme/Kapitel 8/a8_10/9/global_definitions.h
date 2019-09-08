#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include "simple_types.h"
#include "screen_interface.h"
#include "texture.h"

long window_size = 500;
svertex window_pos = svertex( 31, 17 );

texture colors[ 7 ];
long act_color = 0;

long running = 1;
double blue_radius = 55;
double blue_speed = 5;

void end_program( void ) {  running = 0;  }
void increase_blue_radius( void ) {  if( (blue_radius += 0.5) > 80 ) blue_radius = 80;  }
void decrease_blue_radius( void ) {  if( (blue_radius -= 0.5) <  8 ) blue_radius =  8;  }
void increase_speed( void ) {  if( (blue_speed += 0.02) > 20 ) blue_speed = 20;  }
void decrease_speed( void ) {  if( (blue_speed -= 0.02) < 0.1 ) blue_speed = 0.1;  }
void increase_color( void ) {  if( (++act_color) > 6 ) act_color = 0;  }
void decrease_color( void ) {  if( (--act_color) < 0 ) act_color = 6;  }

void initialise_world( HINSTANCE hInstance, long xr, long yr, long bit_depth )
{
  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res-1, y_res-1, 0 );

  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );

  glClearColor( 1, 1, 1, 0 );

  for( long x=0 ; x<7 ; x++ )
     colors[ x ].load( "blue.bmp", pixel_32( 255, 255, 254 ), x );
}

void adjust_pos( double radius, svertex *pos, vector *dir )
{
  double min_sx = window_pos.sx + radius;  double max_sx = window_pos.sx + window_size - radius;
  double min_sy = window_pos.sy + radius;  double max_sy = window_pos.sy + window_size - radius;

  if( pos->sx < min_sx ) {  pos->sx = min_sx;  dir->x = -dir->x;  }
  if( pos->sx > max_sx ) {  pos->sx = max_sx;  dir->x = -dir->x;  }
  if( pos->sy < min_sy ) {  pos->sy = min_sy;  dir->y = -dir->y;  }
  if( pos->sy > max_sy ) {  pos->sy = max_sy;  dir->y = -dir->y;  }
}

void display_texture( texture *surface, svertex pos, double radius )
{
  surface->activate();

  glEnable( GL_TEXTURE_2D );

  glBegin( GL_POLYGON );

    glTexCoord2d( 0, 0 );  glVertex2d( pos.sx - radius, pos.sy - radius );
    glTexCoord2d( 1, 0 );  glVertex2d( pos.sx + radius, pos.sy - radius );
    glTexCoord2d( 1, 1 );  glVertex2d( pos.sx + radius, pos.sy + radius );
    glTexCoord2d( 0, 1 );  glVertex2d( pos.sx - radius, pos.sy + radius );

  glEnd();

  glDisable( GL_TEXTURE_2D );
}

void display_window( void )
{
  glColor3ub( 0, 0, 0 );

  glBegin( GL_LINE_LOOP );

    glVertex2d( window_pos.sx,               window_pos.sy );
    glVertex2d( window_pos.sx + window_size, window_pos.sy );
    glVertex2d( window_pos.sx + window_size, window_pos.sy + window_size );
    glVertex2d( window_pos.sx,               window_pos.sy + window_size );

  glEnd();
}

#endif