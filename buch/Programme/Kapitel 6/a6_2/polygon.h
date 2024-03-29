#ifndef POLYGON_H
#define POLYGON_H

#include "simple_types.h"
#include "global_definitions.h"
#include "draw_line.h"

class polygon
{
  private:
    long point_count, cp_point_count;

    long *points;
    vector normal;

    void z_clipping( void );    
    void project( void );
    void xy_clipping( void );

    void shape( pixel_32 *sbuffer, pixel_32 color );
    void rasterize( pixel_32 *sbuffer );

  public:
    void load( FILE *input, vertex *vs );
    void update_pos( matrix m ) {  normal = m * normal;  }
    void display( vertex *vs, pixel_32 *sbuffer );

    polygon( const polygon &p ) {  exit_error( "polygon: Aufruf Kopierkonstruktor.\n" );  }
    polygon( void ) : point_count( 0 ), cp_point_count( 0 ), points( NULL ) { }
   ~polygon( void ) {  if( points ) delete [] points;  }

  friend class polyhedron;
};

cvertex get_zplane_coords( cvertex a, cvertex b, double plane_z )
{
  double m = (plane_z - b.wz) / (a.wz - b.wz);
  cvertex c;

  c.wx = (a.wx - b.wx) * m + b.wx;
  c.wy = (a.wy - b.wy) * m + b.wy;
  c.wz = plane_z;
  c.color = (a.color - b.color) * m + b.color;

  return c;  
}

void polygon::z_clipping( void )
{
  long x, y;
  long local_point_count = 0;

  for( x=point_count-1, y=0 ; y<point_count ; x=y, y++ )
  {
    if( wpoint[ x ].wz >= z_min && wpoint[ y ].wz >= z_min )                                   // inside
      c_wpoint[ local_point_count++ ] = wpoint[ y ];
      
    else if( wpoint[ x ].wz >= z_min && wpoint[ y ].wz < z_min )                               // leaving
      c_wpoint[ local_point_count++ ] = get_zplane_coords( wpoint[ x ], wpoint[ y ], z_min );
      
    else if( wpoint[ x ].wz < z_min && wpoint[ y ].wz >= z_min )                               // entering
    {
      c_wpoint[ local_point_count++ ] = get_zplane_coords( wpoint[ x ], wpoint[ y ], z_min );
      c_wpoint[ local_point_count++ ] = wpoint[ y ];	
    }	    
  }
  
  cp_point_count = 0;
  for( x=local_point_count-1, y=0 ; y<local_point_count ; x=y, y++ )
  {
    if( c_wpoint[ x ].wz <= z_max && c_wpoint[ y ].wz <= z_max )                               // inside
      wpoint[ cp_point_count++ ] = c_wpoint[ y ];
      
    else if( c_wpoint[ x ].wz <= z_max && c_wpoint[ y ].wz > z_max )                           // leaving
      wpoint[ cp_point_count++ ] = get_zplane_coords( c_wpoint[ x ], c_wpoint[ y ], z_max );
      
    else if( c_wpoint[ x ].wz > z_max && c_wpoint[ y ].wz <= z_max )                           // entering
    {
      wpoint[ cp_point_count++ ] = get_zplane_coords( c_wpoint[ x ], c_wpoint[ y ], z_max );
      wpoint[ cp_point_count++ ] = c_wpoint[ y ];	
    }	    
  }  
}

void polygon::project( void )
{
  for( long x=0 ; x<cp_point_count ; x++ )
  {
    if( wpoint[ x ].wz <= 0.0 ) exit_error( "polygon_project(): Ung�ltige z-Koordinate.\n" );
  
    spoint[ x ].sx = long( wpoint[ x ].wx / wpoint[ x ].wz *  pr_cnst + x_res/2 );
    spoint[ x ].sy = long( wpoint[ x ].wy / wpoint[ x ].wz * -pr_cnst + y_res/2 );
    spoint[ x ].sz = wpoint[ x ].wz + clear_translation;
    spoint[ x ].color = wpoint[ x ].color;
  }
}

svertex get_xplane_coords( svertex a, svertex b, long plane_x )
{
  double m = (plane_x - b.sx) / double( a.sx - b.sx );
  svertex c;

  c.sx = plane_x;
  c.sy = long( (a.sy - b.sy) * m + b.sy );
  c.sz = (a.sz - b.sz) * m + b.sz;
  c.color = (a.color - b.color) * m + b.color;

  return c;  
}

svertex get_yplane_coords( svertex a, svertex b, long plane_y )
{
  double m = (plane_y - b.sy) / double( a.sy - b.sy );
  svertex c;

  c.sx = long( (a.sx - b.sx) * m + b.sx );
  c.sy = plane_y;
  c.sz = (a.sz - b.sz) * m + b.sz;
  c.color = (a.color - b.color) * m + b.color;
  
  return c;
}

void polygon::xy_clipping( void )
{
  long x, y;
  long local_point_count = 0;

  for( x=cp_point_count-1, y=0 ; y<cp_point_count ; x=y, y++ )
  {
    if( spoint[ x ].sx >= x_min && spoint[ y ].sx >= x_min )                                   // inside
      c_spoint[ local_point_count++ ] = spoint[ y ];
      
    else if( spoint[ x ].sx >= x_min && spoint[ y ].sx < x_min )                               // leaving
      c_spoint[ local_point_count++ ] = get_xplane_coords( spoint[ x ], spoint[ y ], x_min );
      
    else if( spoint[ x ].sx < x_min && spoint[ y ].sx >= x_min )                               // entering
    {
      c_spoint[ local_point_count++ ] = get_xplane_coords( spoint[ x ], spoint[ y ], x_min );
      c_spoint[ local_point_count++ ] = spoint[ y ];	
    }	    
  }
  
  cp_point_count = 0;
  for( x=local_point_count-1, y=0 ; y<local_point_count ; x=y, y++ )
  {
    if( c_spoint[ x ].sx <= x_max && c_spoint[ y ].sx <= x_max )                               // inside
      spoint[ cp_point_count++ ] = c_spoint[ y ];
      
    else if( c_spoint[ x ].sx <= x_max && c_spoint[ y ].sx > x_max )                           // leaving
      spoint[ cp_point_count++ ] = get_xplane_coords( c_spoint[ x ], c_spoint[ y ], x_max );
      
    else if( c_spoint[ x ].sx > x_max && c_spoint[ y ].sx <= x_max )                           // entering
    {
      spoint[ cp_point_count++ ] = get_xplane_coords( c_spoint[ x ], c_spoint[ y ], x_max );
      spoint[ cp_point_count++ ] = c_spoint[ y ];	
    }	    
  }

  local_point_count = 0;
  for( x=cp_point_count-1, y=0 ; y<cp_point_count ; x=y, y++ )
  {
    if( spoint[ x ].sy >= y_min && spoint[ y ].sy >= y_min )                                   // inside
      c_spoint[ local_point_count++ ] = spoint[ y ];
      
    else if( spoint[ x ].sy >= y_min && spoint[ y ].sy < y_min )                               // leaving
      c_spoint[ local_point_count++ ] = get_yplane_coords( spoint[ x ], spoint[ y ], y_min );
      
    else if( spoint[ x ].sy < y_min && spoint[ y ].sy >= y_min )                               // entering
    {
      c_spoint[ local_point_count++ ] = get_yplane_coords( spoint[ x ], spoint[ y ], y_min );
      c_spoint[ local_point_count++ ] = spoint[ y ];	
    }	    
  }

  cp_point_count = 0;
  for( x=local_point_count-1, y=0 ; y<local_point_count ; x=y, y++ )
  {
    if( c_spoint[ x ].sy <= y_max && c_spoint[ y ].sy <= y_max )                               // inside
      spoint[ cp_point_count++ ] = c_spoint[ y ];
      
    else if( c_spoint[ x ].sy <= y_max && c_spoint[ y ].sy > y_max )                           // leaving
      spoint[ cp_point_count++ ] = get_yplane_coords( c_spoint[ x ], c_spoint[ y ], y_max );
      
    else if( c_spoint[ x ].sy > y_max && c_spoint[ y ].sy <= y_max )                           // entering
    {
      spoint[ cp_point_count++ ] = get_yplane_coords( c_spoint[ x ], c_spoint[ y ], y_max );
      spoint[ cp_point_count++ ] = c_spoint[ y ];	
    }	    
  }
}

double get_color( vector normal )
{
  double t = dot( normal, light ) / (normal.length * light.length);
    
  double c = -t * (color_count - 1);
  if( c > 0 ) return c;
  
  return 0;
}

void polygon::display( vertex *vs, pixel_32 *sbuffer )
{
  for( long x=0 ; x<point_count ; x++ )
  {
    wpoint[ x ].wx    = vs[ points[ x ] ].wx;
    wpoint[ x ].wy    = vs[ points[ x ] ].wy;
    wpoint[ x ].wz    = vs[ points[ x ] ].wz;
    wpoint[ x ].color = get_color( vs[ points[ x ] ].normal );
  }

  z_clipping();
  project();
  xy_clipping(); if( !cp_point_count ) return;

  rasterize( sbuffer );
}

void set_side( svertex begin, svertex end, screen_side *side )
{
  long length = end.sy - begin.sy;

  if( length <= 0 )
  {
    side[ begin.sy ].offset = begin.sy * x_res + begin.sx;
    return;
  }

  double a_offset = double( begin.sy * x_res + begin.sx );
  double e_offset = double( end.sy * x_res + end.sx );

  double offset_step = ( e_offset - a_offset ) / length;
  double z_step = (end.sz - begin.sz) / length;
  double c_step = (end.color - begin.color) / length;

  double act_z = begin.sz;
  double act_c = begin.color;
  for( long act_y=begin.sy ; act_y <= end.sy ; act_y++ )
  {
    side[ act_y ].offset = long( a_offset + 0.5 );
    side[ act_y ].sz = act_z;
    side[ act_y ].color = act_c;

    a_offset += offset_step;
    act_z += z_step;
    act_c += c_step;
  }
}

inline void inc( long *x, long point_count )
{
  (*x)++;

  if( (*x) >= point_count ) *x = 0;
}

inline void dec( long *x, long point_count )
{
  (*x)--;

  if( (*x) < 0 ) *x = point_count-1;
}

void polygon::rasterize( pixel_32 *sbuffer )
{
  long x, y, top=0, bottom=0;

  for( x=1 ; x<cp_point_count ; x++ )
  {
    if( spoint[ top    ].sy > spoint[ x ].sy ) top = x;
    if( spoint[ bottom ].sy < spoint[ x ].sy ) bottom = x;
  }

  x = y = top;
  for( dec( &y, cp_point_count ) ; x!=bottom ; x=y, dec( &y, cp_point_count ) )
     set_side( spoint[ x ], spoint[ y ], left_side );

  x = y = top;
  for( inc( &y, cp_point_count ) ; x!=bottom ; x=y, inc( &y, cp_point_count ) )
     set_side( spoint[ x ], spoint[ y ], right_side );

  long m = (spoint[ bottom ].sy + spoint[ top ].sy) / 2;
  if( left_side[ m ].offset > right_side[ m ].offset )
  {
    screen_side *t = left_side;  left_side = right_side;  right_side = t;
  }

  long length, offset;
  double act_z, z_step;
  double act_c, c_step;
  
  for( y = spoint[ top ].sy ; y <= spoint[ bottom ].sy ; y++ )
  { 
    length = right_side[ y ].offset - left_side[ y ].offset;
    if( length <= 0 ) continue;
    
    z_step = (right_side[ y ].sz - left_side[ y ].sz) / length;
    c_step = (right_side[ y ].color - left_side[ y ].color) / length;

    offset = left_side[ y ].offset;
    act_z = left_side[ y ].sz;
    act_c = left_side[ y ].color;

    while( length-- > 0 )
    {
      if( act_z < zbuffer[ offset ] )
      {
        sbuffer[ offset ] = palette[ long( act_c ) ];
        zbuffer[ offset ] = act_z;
      }
     
      offset++;
      act_z += z_step;
      act_c += c_step;
    }
  }
}

void polygon::shape( pixel_32 *sbuffer, pixel_32 color )
{
  long x, y;

  for( x=cp_point_count-1, y=0 ; y<cp_point_count ; x=y, y++ )
     draw_line( spoint[ x ], spoint[ y ], color, sbuffer );
}

void polygon::load( FILE *input, vertex *vs )
{
  fscanf( input, "%ld", &point_count );

  if( point_count >= largest_point_count )
      exit_error( "point_count ist groesser oder gleich largest_point_count.\n" );

  if( (points = new long[ point_count ]) == NULL ) 
    exit_error( "*points: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<point_count ; x++ )
     fscanf( input, "%ld", &points[ x ] );

  long red, green, blue;
  fscanf( input, "%ld %ld %ld", &red, &green, &blue );

  vector a = vs[ points[ 1 ] ] - vs[ points[ 0 ] ];  
  vector b = vs[ points[ point_count-1 ] ] - vs[ points[ 0 ] ];

  normal = cross( a, b );
}

#endif