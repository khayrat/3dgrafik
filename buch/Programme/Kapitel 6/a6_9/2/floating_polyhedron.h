#ifndef FLOATING_POLYHEDRON_H
#define FLOATING_POLYHEDRON_H

#include "simple_types.h"
#include "polyhedron.h"
#include "matrix.h"

class floating_polyhedron
{
  private:
    long step_count, act_step, border_count, dest_offset;

    polyhedron primary;
    polyhedron *borders;

    vector *ds;

  public:
    vertex pos;
 
    void load( long count, polyhedron *p );

    void update_pos( matrix m );
    void display( void );

    floating_polyhedron( const floating_polyhedron &fp ) { exit_error( "floating_polyhedron: Aufruf Kopierkonstruktor.\n" ); }
    floating_polyhedron( void ) : 
      step_count( 0 ), act_step( 0 ), border_count( 0 ), dest_offset( 0 ),
      borders( NULL ), ds( NULL ) { }
   ~floating_polyhedron( void ) {  if( ds ) delete [] ds;  }
};

void floating_polyhedron::load( long count, polyhedron *p )
{
  {
    border_count = count;
    dest_offset = rand() % border_count;

    primary.load( &p[ dest_offset ] );

    if( (borders = new polyhedron[ border_count ]) == NULL )
        exit_error( "*borders: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    for( long x=0 ; x<border_count ; x++ ) borders[ x ].load( &p[ x ] );
  }

  {
    for( long x=1 ; x<border_count ; x++ )
       if( p[ x ].vertex_count != primary.vertex_count )
         exit_error( "Anfangs- und Endpolyeder besitzen eine unterschiedliche Anzahl an Eckpunkten.\n" );

    if( (ds = new vector[ primary.vertex_count ]) == NULL )
      exit_error( "*ds: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    step_count = 100;
    act_step = step_count;
  }
}

void floating_polyhedron::update_pos( matrix m )
{
  pos = m * pos;

  long x;

  primary.update_pos( m );
  for( x=0 ; x<border_count ; x++ ) borders[ x ].update_pos( m );

  for( x=0 ; x<primary.vertex_count ; x++ ) ds[ x ] = m * ds[ x ];
}

void floating_polyhedron::display( void )
{
  if( act_step < step_count )
  {
    for( long x=0 ; x<primary.vertex_count ; x++ )
       primary.vs[ x ] = primary.vs[ x ] + ds[ x ];

    act_step++;
  }

  else
  {
    long act = dest_offset;
    do { dest_offset = rand() % border_count; } while( act == dest_offset );

    for( long x=0 ; x<primary.vertex_count ; x++ )
       ds[ x ] = (1.0 / step_count) * (borders[ dest_offset ].vs[ x ] - borders[ act ].vs[ x ]);

    act_step = 0;
  }

  primary.display_edges();
}

#endif