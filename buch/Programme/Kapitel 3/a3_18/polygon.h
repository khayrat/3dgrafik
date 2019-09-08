#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "matrix.h"

class polygon
{
  private:
    long point_count;
    pixel_32 color;

    vertex *wpoint;

  public:
    vertex pos;

    void update_pos( matrix m );
    void display( void );

    polygon( long pc, pixel_32 c, vertex *vs );
   ~polygon( void ) {  if( wpoint ) delete [] wpoint;  }
};

void polygon::update_pos( matrix m )
{
  pos = m * pos;

  for( long x=0 ; x<point_count ; x++ )
     wpoint[ x ] = m * wpoint[ x ];
}

void polygon::display( void )
{
  glColor3ub( color.red, color.green, color.blue );

  glBegin( GL_LINE_LOOP );

    for( long x=0 ; x<point_count ; x++ )
       glVertex3d( wpoint[ x ].wx, wpoint[ x ].wy, wpoint[ x ].wz );

  glEnd();
}

polygon::polygon( long pc, pixel_32 c, vertex *vs )
{
  point_count = pc;  color = c;

  if( (wpoint = new vertex[ point_count ]) == NULL )
    exit_error( "polygon::polygon(): Fehler bei der Reservierung von Arbeitspeicher.\n" );

  long x;

  for( x=0 ; x<point_count ; x++ ) wpoint[ x ] = vs[ x ];

  double xs, xg, ys, yg, zs, zg;  
  xs = xg = wpoint[ 0 ].wx;  ys = yg = wpoint[ 0 ].wy;  zs = zg = wpoint[ 0 ].wz;
    
  for( x=1 ; x<point_count ; x++ )
  {
    if( wpoint[ x ].wx < xs ) xs = wpoint[ x ].wx;  if( wpoint[ x ].wx > xg ) xg = wpoint[ x ].wx;
    if( wpoint[ x ].wy < ys ) ys = wpoint[ x ].wy;  if( wpoint[ x ].wy > yg ) yg = wpoint[ x ].wy;
    if( wpoint[ x ].wz < zs ) zs = wpoint[ x ].wz;  if( wpoint[ x ].wz > zg ) zg = wpoint[ x ].wz;
  }
  
  pos.wx = (xs + xg) / 2;
  pos.wy = (ys + yg) / 2;
  pos.wz = (zs + zg) / 2;

  matrix m;
  m.translate( -pos.wx, -pos.wy, -pos.wz );
  update_pos( m );
}

#endif