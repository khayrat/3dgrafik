#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <stdio.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "polygon.h"

class polyhedron
{
  private:
    long vertex_count, polygon_count;
    vertex *vs;
    polygon *ps;

  public:
    local_system ls;
  
    void display( void );
    
    polyhedron( char *filename );
   ~polyhedron( void ) {  if( vs ) delete [] vs;  if( ps ) delete [] ps;  }
};

void polyhedron::display( void )
{
  for( long x=0 ; x<polygon_count ; x++ ) ps[ x ].display( vs );
}

polyhedron::polyhedron( char *filename )
{
  FILE *input;

  if( (input = fopen( filename, "rb" )) == NULL )
    exit_nofile( "polyhedron::polyhedron()", filename );

  {
    fscanf( input, "%ld", &vertex_count );

    if( vertex_count >= largest_point_count )
      exit_error( "vertex_count ist groesser oder gleich largest_point_count.\n" );

    if( (vs = new vertex[ vertex_count ]) == NULL ) 
      exit_error( "*vs: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    for( long x=0 ; x<vertex_count ; x++ )
    {
      double wx, wy, wz;
      fscanf( input, "%lf %lf %lf", &wx, &wy, &wz );
      vs[ x ] = vertex( float( wx ), float( wy ), float( wz ) );
    }
  }

  {
    fscanf( input, "%ld", &polygon_count );

    if( (ps = new polygon[ polygon_count ]) == NULL )
      exit_error( "polygon_count: Fehler während der Reservierung von Arbeisttpeicher.\n" );

    for( long x=0 ; x<polygon_count ; x++ ) ps[ x ].load( input );
  }

  fclose( input );

  {
    float xs, xg, ys, yg, zs, zg;  
    xs = xg = vs[ 0 ].wx;  ys = yg = vs[ 0 ].wy;  zs = zg = vs[ 0 ].wz;
    
    long x;

    for( x=0 ; x<vertex_count ; x++ )
    {
      if( vs[ x ].wx < xs ) xs = vs[ x ].wx;  if( vs[ x ].wx > xg ) xg = vs[ x ].wx;
      if( vs[ x ].wy < ys ) ys = vs[ x ].wy;  if( vs[ x ].wy > yg ) yg = vs[ x ].wy;
      if( vs[ x ].wz < zs ) zs = vs[ x ].wz;  if( vs[ x ].wz > zg ) zg = vs[ x ].wz;
    }
  
    float xd = (xs + xg) / 2;
    float yd = (ys + yg) / 2;
    float zd = (zs + zg) / 2;

    for( x=0 ; x<vertex_count ; x++ )
    {
      vs[ x ].wx -= xd;  vs[ x ].wy -= yd;  vs[ x ].wz -= zd;
    }   
  }
}

#endif