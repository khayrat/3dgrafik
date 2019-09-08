#ifndef FLOATING_POLYHEDRON_H
#define FLOATING_POLYHEDRON_H

#include "simple_types.h"
#include "polyhedron.h"
#include "matrix.h"

class floating_polyhedron
{
  private:
    long step_count, act_step;

    polyhedron primary;

    vector *ds;

  public:
    vertex pos;
 
    void load( polyhedron *begin, polyhedron *end );

    void update_pos( matrix m );
    void display( void );

    floating_polyhedron( const floating_polyhedron &fp ) { exit_error( "floating_polyhedron: Aufruf Kopierkonstruktor.\n" ); }
    floating_polyhedron( void ) : step_count( 0 ), act_step( 0 ), ds( NULL ) { }
   ~floating_polyhedron( void ) {  if( ds ) delete [] ds;  }
};

void floating_polyhedron::load( polyhedron *begin, polyhedron *end )
{
  primary.load( begin );

  {
    if( begin->vertex_count != end->vertex_count )
      exit_error( "Anfangs- und Endpolyeder besitzen eine unterschiedliche Anzahl an Eckpunkten.\n" );

    if( (ds = new vector[ primary.vertex_count ]) == NULL )
      exit_error( "*ds: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    step_count = 1000;

    for( long x=0 ; x<primary.vertex_count ; x++ )
       ds[ x ] = (1.0 / step_count) * (end->vs[ x ] - begin->vs[ x ]);
  }
}

void floating_polyhedron::update_pos( matrix m )
{
  pos = m * pos;

  primary.update_pos( m );

  for( long x=0 ; x<primary.vertex_count ; x++ ) ds[ x ] = m * ds[ x ];
}

void floating_polyhedron::display( void )
{
  if( act_step < step_count )
  {
    for( long x=0 ; x<primary.vertex_count ; x++ )
       primary.vs[ x ] = primary.vs[ x ] + ds[ x ];

    act_step++;
  }

  primary.display_edges();
}

#endif