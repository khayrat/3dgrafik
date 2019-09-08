#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>

struct complex
{
  long double r, i;
  
  long double abs( void ) {  return sqrt( r*r + i*i );  }
  
  complex( void ) : r( 0 ), i( 0 ) { }
  complex( long double rv, long double iv ) : r( rv ), i( iv ) { }
};

complex operator + ( complex a, complex b )
{  	
  return complex( a.r+b.r, a.i+b.i );
}

complex operator - ( complex a, complex b )
{  	
  return complex( a.r-b.r, a.i-b.i );
}

complex operator * ( complex a, complex b )
{  
  long double r, i;

  r = a.r * b.r - a.i * b.i;
  i = a.r * b.i + a.i * b.r;
 
  return complex( r, i );
}

complex operator / ( complex a, complex b )
{
  complex inv_b;

  inv_b.r =  b.r / (b.r * b.r + b.i * b.i);
  inv_b.i = -b.i / (b.r * b.r + b.i * b.i);

  return a * inv_b;
}

#endif