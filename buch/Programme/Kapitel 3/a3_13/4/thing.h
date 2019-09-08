#ifndef THING_H
#define THING_H

#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

class thing
{
  private:
    polygon ps[ 3 ];
        
    void update_polygons( void );
     
  public:
   vertex pos;

    void update_pos( matrix m );
    void display( pixel_32 *sbuffer );
    
    thing( long pc, pixel_32 color );
};

thing::thing( long pc, pixel_32 color )
{
  for( uchar x=0 ; x<3 ; x++ ) ps[ x ].load( pc, color );

  const double pi = 3.1415926535;  
  matrix tm;

  tm.rotate_x( 0.5 * pi );
  ps[ 1 ].update_pos( tm );
  tm.clear();
  
  tm.rotate_y( 0.5 * pi );
  ps[ 2 ].update_pos( tm );

  pos = vertex( 0, 0, 0 );
}

void thing::update_polygons( void )
{
  matrix m[ 3 ];

  uchar x;

  for( x=0 ; x<3 ; x++ ) m[ x ].translate( -pos.wx, -pos.wy, -pos.wz );
  
  m[ 0 ].rotate_x( 0.01 );  m[ 0 ].rotate_y( 0.02 );  m[ 0 ].rotate_z( 0.01 );
  m[ 1 ].rotate_x( 0.03 );  m[ 1 ].rotate_y( 0.01 );  m[ 1 ].rotate_z( 0.01 );
  m[ 2 ].rotate_x( 0    );  m[ 2 ].rotate_y( 0.01 );  m[ 2 ].rotate_z( 0.04 );

  for( x=0 ; x<3 ; x++ ) 
  {
    m[ x ].translate( pos.wx, pos.wy, pos.wz );
    
    ps[ x ].update_pos( m[ x ] );
  }
}

void thing::update_pos( matrix m )
{
  pos = m * pos;

  for( uchar x=0 ; x<3 ; x++ ) ps[ x ].update_pos( m );
}

void thing::display( pixel_32 *sbuffer )
{
  update_polygons();	
	
  for( uchar x=0 ; x<3 ; x++ ) ps[ x ].display( sbuffer );
}

#endif