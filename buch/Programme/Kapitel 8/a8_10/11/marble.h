#ifndef MARBLE_H
#define MARBLE_H

#include "global_definitions.h"

class marble
{
  public:
    svertex pos;
    vector dir;

    texture *surface;

    void load( texture *t );
    vector update_pos( svertex yellow_pos );
    void display( void ) {  display_texture( surface, pos, blue_radius );  }

    marble( void ) : surface( NULL ) { }
};

vector marble::update_pos( svertex yp )
{
  if( dir.length() > 0 ) dir.set_length( blue_speed );

  pos = pos + dir;

  adjust_pos( blue_radius, &pos, &dir );

  vector v = yp - pos;
  vector prev_v = yp - (pos + (-1) * dir);

  if( v.length() < blue_radius + yellow_radius && v.length() > prev_v.length() )
  {
    v.set_length( blue_radius + yellow_radius );
    pos = yp + (-1) * v;
  }

  else if( v.length() < blue_radius + yellow_radius )
  {
    svertex p = pos + (dot( yp - pos, dir ) / dot( dir, dir )) * dir;
    v = yp - p;
    double t = sqrt( (blue_radius+yellow_radius)*(blue_radius+yellow_radius) - v.length()*v.length() );
    v = (-1) * dir;  v.set_length( t );  pos = p + v;
    
    v = yp - pos;
    double lambda = dot( -1*dir, v ) / dot( v, v );
    svertex q = pos + lambda * v;
    svertex c = q + (q - (pos + (-1) * dir));

    dir = c - pos;  dir.set_length( blue_speed );

    v.set_length( (blue_mass / yellow_mass) * blue_speed );
    return v;
  }

  return vector( 0, 0 );
}

void marble::load( texture *t )
{
  surface = t;

  vector v;

  do
  {
    pos.sx = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sx + blue_radius;
    pos.sy = (rand() % long( window_size - 2 * blue_radius )) + window_pos.sy + blue_radius;

    v = pos - svertex( window_pos.sx + window_size/2, window_pos.sy + window_size/2 );
  } while( v.length() < (blue_radius + yellow_radius) );

  while( dir.length() == 0 ) dir = vector( (rand() % 100) - 50, (rand() % 100) - 50 );
}

#endif