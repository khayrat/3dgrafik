#ifndef MARBLE_H
#define MARBLE_H

#include "global_definitions.h"

class marble
{
  private:
    svertex pos;
    vector dir;

    texture *surface;

  public:
    void load( texture *t );
    void update_pos( void );
    void display( void ) {  display_texture( surface, pos, blue_radius );  }
};

void marble::update_pos( void )
{
  dir.set_length( blue_speed );

  pos = pos + dir;

  adjust_pos( blue_radius, &pos, &dir );
}

void marble::load( texture *t )
{
  surface = t;

  pos.sx = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sx + blue_radius;
  pos.sy = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sy + blue_radius;

  while( dir.length() == 0 ) dir = vector( (rand() % 100) - 50, (rand() % 100) - 50 );
}

#endif