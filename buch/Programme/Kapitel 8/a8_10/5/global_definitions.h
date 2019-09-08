#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include "simple_types.h"
#include "texture.h"

long window_size = 500;
svertex window_pos;

double blue_radius = 27;
double blue_speed = 5;

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