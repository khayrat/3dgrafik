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
    void update_pos( void );
    void display( void ) {  display_texture( surface, pos, blue_radius );  }

    marble( texture *t );
};

void marble::update_pos( void )
{
  pos = pos + dir;

  adjust_pos( blue_radius, &pos, &dir );
}

marble::marble( texture *t )
{
  surface = t;

  pos.sx = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sx + blue_radius;
  pos.sy = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sy + blue_radius;

  while( dir.length() == 0 ) dir = vector( (rand() % 100) - 50, (rand() % 100) - 50 );
  dir.set_length( blue_speed );
}

#endif