#ifndef MARBLE_H
#define MARBLE_H

#include "global_definitions.h"

class marble
{
  private:
    svertex pos;
    vector dir;

  public:
    void load( void );
    void update_pos( void );
    void display( void ) {  display_texture( &colors[ act_color ], pos, blue_radius );  }
};

void marble::update_pos( void )
{
  dir.set_length( blue_speed );

  pos = pos + dir;

  adjust_pos( blue_radius, &pos, &dir );
}

void marble::load( void )
{
  pos.sx = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sx + blue_radius;
  pos.sy = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sy + blue_radius;

  while( dir.length() == 0 ) dir = vector( (rand() % 100) - 50, (rand() % 100) - 50 );
}

#endif