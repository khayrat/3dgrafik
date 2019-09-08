#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "wireframe.h"
#include "matrix.h"
#include "heightfield.h"
#include "primary_color.h"
#include "bitmap_32.h"

const double pi = 3.1415926535;

local_system view;
vector light( 10, 10, 10 );
double light_angle = 0.25 * pi;

uchar handle_input( void );
void display_vector( vector v );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "D, G, R, F:\tVeränderung der Richtung des einfallenden Lichtes\nPfeiltasten:\tRotation des dreidimensionalen Modells\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  pixel_32 borders3[ 3 ] = {  pixel_32( 0, 100, 0 ), pixel_32( 128, 128, 128 ), pixel_32( 255, 255, 255 )  };
  primary_color palette;
  palette.load( 256, 3, borders3 );

  heightfield terrain( 200, 200 );
  terrain.plasma_fractal( 500 );

  bitmap_32 color_map;   color_map.load( &terrain, palette.components );
  bitmap_32 shadow_map;  shadow_map.load( terrain.xscale, terrain.yscale );

  wireframe system;  system.load( "system.lns" );

  matrix m;

  m.rotate_y( 0.2 );
  m.rotate_x( -0.5 );
  m.translate( 0, 5, 80 );
  view = m * view;
  m.clear();

  m.scale( 14, 14, 14 );
  system.update_pos( m );

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( handle_input() ) break;

    m.clear();
    m.columns( view.x_axis, view.y_axis, view.z_axis );
    m.translate( view.pos.wx, view.pos.wy, view.pos.wz );
    m.adjust_hardware(); 

    system.display();
    display_vector( light );

    m.clear();
    m.adjust_hardware();

    color_map.display( 100, 350 );
    
    shadow_map.shadow( &terrain, light );
    shadow_map.display( 500, 350 );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

void display_vector( vector v )
{
  glEnable( GL_BLEND );

  glPointSize( 8 );
  glBegin( GL_POINTS );

    glColor3d( 255, 0, 0 );
    glVertex3d( v.x, 0, 0 );

    glColor3d( 0, 255, 0 );
    glVertex3d( 0, v.y, 0 );

    glColor3d( 0, 0, 255 );
    glVertex3d( 0, 0, v.z );

    glColor3d( 255, 255, 255 );
    glVertex3d( light.x, 0, light.z );

    glColor3d( 255, 255, 0 );
    glVertex3d( light.x, light.y, light.z );

  glEnd();

  glDisable( GL_BLEND );

  glPointSize( 1 );
  glColor3ub( 200, 200, 200 );
  glBegin( GL_LINE_LOOP );

    for( double t = 0 ; t<2*pi ; t+=0.01 )
       glVertex3d( 10 * sin( t ), 0, 10 * cos( t ) );

  glEnd();

  glBegin( GL_LINES );

    glColor3ub( 255, 255, 0 );
    glVertex3d( 0, 0, 0 );
    glVertex3d( light.x, light.y, light.z );

    glColor3ub( 200, 200, 200 );
    glVertex3d( light.x, light.y, light.z );
    glVertex3d( light.x, 0, light.z );

    glVertex3d( light.x, 0, 0 );
    glVertex3d( light.x, 0, light.z );

    glVertex3d( 0, 0, light.z );
    glVertex3d( light.x, 0, light.z );

  glEnd();
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  matrix m;

  m.translate( -view.pos.wx, -view.pos.wy, -view.pos.wz );

  double a = 0.01;

  if( input.key_pressed( VK_UP    ) ) m.rotate_x(  a );
  if( input.key_pressed( VK_DOWN  ) ) m.rotate_x( -a );
  if( input.key_pressed( VK_LEFT  ) ) m.rotate_y(  a );
  if( input.key_pressed( VK_RIGHT ) ) m.rotate_y( -a );
  if( input.key_pressed( VK_PRIOR ) ) m.rotate_z(  a );
  if( input.key_pressed( VK_NEXT  ) ) m.rotate_z( -a );

  m.translate( view.pos.wx, view.pos.wy, view.pos.wz );

  double step = 2;

  if( input.key_pressed( VK_HOME ) ) m.translate( 0, 0, -step );
  if( input.key_pressed( VK_END  ) ) m.translate( 0, 0,  step );

  view = m * view;

  step = 0.1;

  if( input.key_pressed( 'D' ) ) if( (light_angle -= step) < 0 ) light_angle += 2*pi;
  if( input.key_pressed( 'G' ) ) if( (light_angle += step) > 2*pi ) light_angle -= 2*pi;
  if( input.key_pressed( 'R' ) ) if( (light.y += 0.5) > 10 ) light.y = 10;
  if( input.key_pressed( 'F' ) ) if( (light.y -= 0.5) < 0 ) light.y = 0;

  light.x = 10 * cos( light_angle );
  light.z = 10 * sin( light_angle );
  
  return 0;
}
