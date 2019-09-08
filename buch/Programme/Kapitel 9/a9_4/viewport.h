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
    void load( long xr, long yr, double pc, double min, double max );

    viewport( long xr, long yr, double pc, double min, double max ) {  load( xr, yr, pc, min, max );  }
    viewport( const viewport &v ) {  exit_error( "viewport: Aufruf Kopierkonstruktor.\n" );  }
};

void viewport::load( long xr, long yr, double pc, double min, double max )
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
}

#endif