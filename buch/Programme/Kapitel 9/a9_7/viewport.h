#ifndef WIEWPORT_H
#define WIEWPORT_H

#include <stdio.h>

#include "simple_types.h"
#include "wireframe.h"

class viewport : public wireframe
{
  private:
    long x_resolution, y_resolution;
    double pr_constant;
    double z_minimum, z_maximum;

  public:
    double load( long xr, long yr, double pc, double min, double max );
    void get_points( vertex *points );
    void display( void );

    viewport( void ) {  x_resolution = y_resolution = 0;  pr_constant = z_minimum = z_maximum = 0;  }
    viewport( const viewport &v ) {  exit_error( "viewport: Aufruf Kopierkonstruktor.\n" );  }
};

void viewport::get_points( vertex *points )
{
  matrix m;
  m.columns( ls.x_axis, ls.y_axis, ls.z_axis );
  m.translate( ls.pos.wx, ls.pos.wy, ls.pos.wz );

  for( long x=0 ; x<8 ; x++ ) points[ x ] = m * vs[ x ];
}

void viewport::display( void )
{
  matrix m;
  m.columns( ls.x_axis, ls.y_axis, ls.z_axis );
  m.translate( ls.pos.wx, ls.pos.wy, ls.pos.wz );

  glBegin( GL_LINES );

    for( long x=0 ; x<line_count ; x++ )
    {
      glColor3ub( es[ x ].color.red, es[ x ].color.green, es[ x ].color.blue );

      vertex begin = m * vs[ es[ x ].begin ];
      vertex end = m * vs[ es[ x ].end ];

      if( x >= 12 )
      {
        glVertex3d( begin.wx, begin.wy, begin.wz );
        glVertex3d( end.wx, end.wy, end.wz );

        glColor3ub( es[ 0 ].color.red, es[ 0 ].color.green, es[ 0 ].color.blue );
        glVertex3d( begin.wx, 0, begin.wz );
        glVertex3d( end.wx, 0, end.wz );
      }

      else
      {
        glVertex3d( begin.wx, 0, begin.wz );
        glVertex3d( end.wx, 0, end.wz );
      }
    }

  glEnd();
}

double viewport::load( long xr, long yr, double pc, double min, double max )
{
  wireframe::load( "camera.lns" );

  x_resolution = xr;  y_resolution = yr;
  pr_constant = pc;  z_minimum = min;  z_maximum = max;

  vertex window[ 4 ];
  window[ 0 ] = vertex( -x_resolution/2,  y_resolution/2, pr_constant );
  window[ 1 ] = vertex(  x_resolution/2,  y_resolution/2, pr_constant );
  window[ 2 ] = vertex(  x_resolution/2, -y_resolution/2, pr_constant );
  window[ 3 ] = vertex( -x_resolution/2, -y_resolution/2, pr_constant );

  vector d;

  for( long x=0 ; x<4 ; x++ )
  {
    d = window[ x ] - ls.pos;

    vs[ x ] = ls.pos + (z_minimum / pr_constant) * d;
    vs[ x+4 ] = ls.pos + (z_maximum / pr_constant) * d;
  }

  d = vs[ 4 ] - ls.pos;
  return d.length + 1;
}

#endif