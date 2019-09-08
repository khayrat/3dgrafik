#ifndef SPLINE_H
#define SPLINE_H

const long largest_pc = 100;

double p[ largest_pc ];
double s[ largest_pc ];
double y[ largest_pc ];

double v[ largest_pc ];
double q[ largest_pc ];

struct polynom
{
  double a, b, c, d;

  double execute( double x ) {  return a + b*x + c*x*x + d*x*x*x;  }
  polynom( void ) : a( 0 ), b( 0 ), c( 0 ), d( 0 ) { }
};

class spline
{
  private:
    long point_count;

    polynom *xf, *yf;

    void draw_points( void );
    void build_polynoms( double *points, polynom *f );
    
  public:
    void build( svertex *points );
    void draw( void );

    spline( long pc );
   ~spline( void ) {  if( xf ) delete [] xf;  if( yf ) delete [] yf; }
};

spline::spline( long pc )
{
  if( pc >= largest_pc ) 
    exit_error( "spline::spline(): Die Konstante 'largest_pc' ist zu klein.\n" );

  point_count = pc;

  xf = new polynom[ point_count-1 ];
  yf = new polynom[ point_count-1 ];
  if( xf == NULL || yf == NULL )
    exit_error( "spline::spline(): Fehler bei der Reservierung von Arbeitsspeicher.\n" );
}

void spline::build( svertex *points )
{
  long x;

  for( x=0 ; x<point_count ; x++ ) p[ x ] = points[ x ].sx;
  build_polynoms( p, xf );

  for( x=0 ; x<point_count ; x++ ) p[ x ] = points[ x ].sy;
  build_polynoms( p, yf );
}

void spline::build_polynoms( double *points, polynom *f )
{
  long i;

  y[ 0 ] = 3 * (points[ 1 ] - points[ 0 ]);
  for( i=1 ; i<point_count-1 ; i++ )
     y[ i ] = 3 * (points[ i+1 ] - points[ i-1 ]);
  y[ point_count-1 ] = 3 * (points[ point_count-1 ] - points[ point_count-2 ]);

  v[ 0 ] = 0.5;
  q[ 0 ] = 0.5 * y[ 0 ];
  for( i=1 ; i<point_count-1 ; i++ )
  {
    v[ i ] = 1.0 / (4 - v[ i-1 ]);
    
    q[ i ] = v[ i ] * (y[ i ] - q[ i-1 ]);
  }
  q[ point_count-1 ] = (1.0 / (2-v[ point_count-2 ])) * (y[ point_count-1 ] - q[ point_count-2 ]);

  s[ point_count-1 ] = q[ point_count-1 ];
  for( i=point_count-2 ; i>=0 ; i-- )
     s[ i ] = q[ i ] - v[ i ]*s[ i+1 ];

  for( i=0 ; i<point_count-1 ; i++ )
  {
    f[ i ].a = points[ i ];
    f[ i ].b = s[ i ];
    f[ i ].c = 3*points[ i+1 ] - 3*points[ i ] - 2*s[ i ] - s[ i+1 ];
    f[ i ].d = 2*points[ i ] - 2*points[ i+1 ] + s[ i ] + s[ i+1 ];
  }
}

void spline::draw( void )
{
  glColor3ub( 255, 255, 0 );

  for( long i=0 ; i<point_count-1 ; i++ )
  {
    glBegin( GL_LINE_STRIP );

      for( double t=0 ; t<=1 ; t+=0.01 ) 
         glVertex2d( xf[ i ].execute( t ), yf[ i ].execute( t ) );

    glEnd();
  }

  draw_points();
}

void spline::draw_points( void )
{
  glPointSize( 5 );
  glColor3d( 1, 0, 0 );

  glBegin( GL_POINTS );

    for( long i=0 ; i<point_count-1 ; i++ )
       glVertex2d( xf[ i ].execute( 0 ), yf[ i ].execute( 0 ) );

    glVertex2d( xf[ point_count-2 ].execute( 1 ), yf[ point_count-2 ].execute( 1 ) );

  glEnd();
}

#endif