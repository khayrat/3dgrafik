#ifndef SCREEN_INTERFACE_H
#define SCREEN_INTERFACE_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <d3dx9.h>

#include "simple_types.h"
#include "global_definitions.h"
#include "hardware_matrix.h"

void exit_error( char *message );
void exit_nofile( char *user, char *filename );
void message( char *title, char *message );

#define x_res screen_interface.get_xr()
#define y_res screen_interface.get_yr()

#define render_mode D3DFVF_XYZ | D3DFVF_TEX1

#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK main_window_procedure( HWND main_window_handle, UINT message, WPARAM wparam, LPARAM lparam )
{
  if( message == WM_CLOSE ) {  PostQuitMessage( 0 );  return 0;  }
  
  return DefWindowProc( main_window_handle, message, wparam, lparam );
}

class hardware_interface
{
  private:
    uchar enlarged;
    long x_resolution, y_resolution;
    long texture_count;

    HWND main_window_handle;
    LPDIRECT3D9 hardware_device;
    LPDIRECT3DDEVICE9 rendering_device;
    LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
    LPDIRECT3DTEXTURE9 *textures;

    DEVMODE old_screen_settings;
    
    void initialise_platform( void );
    void initialise_display( void );
    void initialise_textures( void );

  public:
    void fullscreen( HINSTANCE instance, long xr, long yr, long bit_depth );
    void open_window( HINSTANCE instance, long xr, long yr );
    void close_window( void );
    
    long get_xr( void ) {  return x_resolution;  }
    long get_yr( void ) {  return y_resolution;  }

    void set_matrix( D3DXMATRIXA16 *hm );
    void set_matrix( hardware_matrix hm );

    void clear( void );

    void line_loop( long point_count, hvertex *points );
    void line_strip( long point_count, hvertex *points );
    void draw_polygon( long point_count, hvertex *points, long tx_offset );

    void swap_buffers( void ) {  rendering_device->Present( NULL, NULL, NULL, NULL );  }

    void backface_removal( uchar state );

    hardware_interface( void )
    {
      enlarged = 0;  x_resolution = 0;  y_resolution = 0;
      main_window_handle = NULL;  hardware_device = NULL;  rendering_device = NULL;  vertex_buffer = NULL;
      textures = NULL;
      memset( &old_screen_settings, 0, sizeof( old_screen_settings ) );
    }
   ~hardware_interface( void ) {  close_window();  }
} screen_interface;

void hardware_interface::initialise_textures( void )
{
  FILE *input;

  if( (input = fopen( "texture_lib.txt", "rb" )) == NULL )
    exit_nofile( "hardware_interface::initialise_textures()", "texture_lib.txt" );

  fscanf( input, "%ld", &texture_count );

  if( (textures = new LPDIRECT3DTEXTURE9[ texture_count ]) == NULL ) 
    exit_error( "*textures: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
  
  for( long x=0 ; x<texture_count ; x++ )
  {
    char texture_name[ 500 ];

     fscanf( input, "%s", &texture_name );

    if( FAILED( D3DXCreateTextureFromFile( rendering_device, texture_name, &textures[ x ] ) ) )
      exit_nofile( "hardware_interface::initialise_textures()", texture_name );
  }

  fclose( input );

  rendering_device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  rendering_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  rendering_device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
}

void hardware_interface::set_matrix( D3DXMATRIXA16 *hm )
{
  rendering_device->SetTransform( D3DTS_WORLD, hm );
}

void hardware_interface::set_matrix( hardware_matrix hm )
{
  rendering_device->SetTransform( D3DTS_WORLD, &hm.world_matrix );
}

void hardware_interface::backface_removal( uchar state )
{
  if( state == 0 ) rendering_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  else rendering_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
}

void hardware_interface::clear( void )
{
  rendering_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 255, 255, 255 ), 1, 0 );
}

void hardware_interface::line_loop( long point_count, hvertex *vs )
{
  hvertex *buffer;
  
  if( FAILED( vertex_buffer->Lock( 0, 0, (void **) &buffer, 0 ) ) ) return;

  memcpy( buffer, vs, point_count*sizeof( hvertex ) );
  buffer[ point_count ] = vs[ 0 ];

  vertex_buffer->Unlock();

  if( FAILED( rendering_device->BeginScene() ) ) return;

    rendering_device->SetStreamSource( 0, vertex_buffer, 0, sizeof( hvertex ) );
    rendering_device->SetFVF( render_mode );
    rendering_device->DrawPrimitive( D3DPT_LINESTRIP, 0, point_count );

  rendering_device->EndScene();
}

void hardware_interface::line_strip( long point_count, hvertex *vs )
{
  hvertex *buffer;
  
  if( FAILED( vertex_buffer->Lock( 0, 0, (void**) &buffer, 0 ) ) ) return;

  memcpy( buffer, vs, point_count*sizeof( hvertex ) );

  vertex_buffer->Unlock();

  if( FAILED( rendering_device->BeginScene() ) ) return;

    rendering_device->SetStreamSource( 0, vertex_buffer, 0, sizeof( hvertex ) );
    rendering_device->SetFVF( render_mode );
    rendering_device->DrawPrimitive( D3DPT_LINESTRIP, 0, point_count-1 );

  rendering_device->EndScene();
}

void hardware_interface::draw_polygon( long point_count, hvertex *vs, long tx_offset )
{
  hvertex *buffer;
  
  if( FAILED( vertex_buffer->Lock( 0, 0, (void**) &buffer, 0 ) ) ) return;

  memcpy( buffer, vs, point_count*sizeof( hvertex ) );

  vertex_buffer->Unlock();

  rendering_device->SetTexture( 0, textures[ tx_offset ] );

  if( FAILED( rendering_device->BeginScene() ) ) return;

    rendering_device->SetStreamSource( 0, vertex_buffer, 0, sizeof( hvertex ) );
    rendering_device->SetFVF( render_mode );
    rendering_device->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, point_count-2 );

  rendering_device->EndScene();
}

void hardware_interface::open_window( HINSTANCE instance, long xr, long yr )
{
  x_resolution = xr;  y_resolution = yr;
	
  WNDCLASS winclass;

  memset( &winclass, 0, sizeof( WNDCLASS ) );
  winclass.style = CS_OWNDC;
  winclass.lpfnWndProc = main_window_procedure;
  winclass.hInstance = instance;
  winclass.lpszClassName = "Main Window";

  RegisterClass( &winclass );

  int x_add = 2 * (GetSystemMetrics( SM_CXBORDER ) + GetSystemMetrics( SM_CXEDGE ));
  int y_add = 2 * (GetSystemMetrics( SM_CYBORDER ) + GetSystemMetrics( SM_CYEDGE )) + GetSystemMetrics( SM_CYCAPTION );

  char window_name[] = "3D-Grafik Programmierung";

  if
  (
    (
      main_window_handle = CreateWindow
      ( 
        "Main Window", window_name,
         WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
         100, 100, xr+x_add, yr+y_add,
         NULL, NULL, instance, NULL
      )
    ) == NULL
  )
  exit_error( "Fehler beim Öffnen des Programmfensters.\n" );

  initialise_platform();
  initialise_display();
  initialise_textures();
}

void hardware_interface::initialise_platform( void )
{
  if( (hardware_device = Direct3DCreate9( D3D_SDK_VERSION )) == NULL )
    exit_error( "DirectX 9.0 wurde auf dem System nicht gefunden.\n" );

  D3DPRESENT_PARAMETERS parameters;

  memset( &parameters, 0, sizeof( D3DPRESENT_PARAMETERS ) );
  parameters.Windowed = TRUE;
  parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
  parameters.BackBufferFormat = D3DFMT_UNKNOWN;
  parameters.EnableAutoDepthStencil = TRUE;
  parameters.AutoDepthStencilFormat = D3DFMT_D16;

  if
  (
    FAILED
    (
      hardware_device->CreateDevice
      (
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, main_window_handle,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &parameters, &rendering_device
      )
    )
  )
  exit_error( "Ausführung von CreateDevice() ist fehlgeschlagen.\n" );

  if
  ( 
    FAILED
    (
      rendering_device->CreateVertexBuffer
      (
        largest_point_count * sizeof( hvertex ),
        0, render_mode,
        D3DPOOL_DEFAULT, &vertex_buffer, NULL
      )
    )
  )
  exit_error( "Ausführung von CreateVertexBuffer() ist fehlgeschlagen.\n" );

  rendering_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  rendering_device->SetRenderState( D3DRS_LIGHTING, FALSE );
  rendering_device->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void hardware_interface::initialise_display( void )
{
  D3DXVECTOR3 user_pos( 0, 0, 0 );
  D3DXVECTOR3 view_direction( 0, 0, 1 );
  D3DXVECTOR3 up_direction( 0, 1, 0 );

  D3DXMATRIXA16 view_matrix;
  D3DXMatrixLookAtLH( &view_matrix, &user_pos, &view_direction, &up_direction );

  rendering_device->SetTransform( D3DTS_VIEW, &view_matrix );

  float alpha = float( 2 * atan( y_resolution / (2 * pr_const) ) );

  D3DXMATRIXA16 projection_matrix;
  D3DXMatrixPerspectiveFovLH( &projection_matrix, alpha, float( x_resolution ) / y_resolution, z_min, z_max );
  rendering_device->SetTransform( D3DTS_PROJECTION, &projection_matrix );
}

void hardware_interface::close_window( void )
{
  if( enlarged )
    if( ChangeDisplaySettings( &old_screen_settings, 0 ) != DISP_CHANGE_SUCCESSFUL )
	  exit_error( "Fehler beim Einstellen der ursprünglichen Bildschirmbetriebsart.\n" );

  if( vertex_buffer ) vertex_buffer->Release();
  if( rendering_device ) rendering_device->Release();
  if( hardware_device ) hardware_device->Release();

  DestroyWindow( main_window_handle );
}

void hardware_interface::fullscreen( HINSTANCE instance, long xr, long yr, long bit_depth )
{
  x_resolution = xr;  y_resolution = yr;
  enlarged = 1;

  if( EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &old_screen_settings ) != TRUE )
	exit_error( "Fehler während der Ermittlung der aktuellen Bildschirmbetriebsart.\n" );

  WNDCLASS winclass;

  memset( &winclass, 0, sizeof( WNDCLASS ) );
  winclass.style = CS_OWNDC;
  winclass.lpfnWndProc = main_window_procedure;
  winclass.hInstance = instance;
  winclass.lpszClassName = "Main Window";

  RegisterClass( &winclass );

  char window_name[] = "3D-Grafik Programmierung";

  if
  (
    (
      main_window_handle = CreateWindow
      ( 
        "Main Window", window_name,
         WS_VISIBLE | WS_POPUP,
         0, 0, x_resolution, y_resolution,
         NULL, NULL, instance, NULL
      )
    ) == NULL
  )
  exit_error( "Fehler beim Öffnen des Programmfensters.\n" );

  DEVMODE new_screen_settings;

  memset( &new_screen_settings, 0, sizeof( new_screen_settings ) );
  new_screen_settings.dmSize = sizeof( new_screen_settings );
  new_screen_settings.dmPelsWidth = x_resolution;
  new_screen_settings.dmPelsHeight = y_resolution;
  new_screen_settings.dmBitsPerPel = bit_depth;
  new_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

  if( ChangeDisplaySettings( &new_screen_settings, 0 ) != DISP_CHANGE_SUCCESSFUL )
	exit_error( "Fehler beim Einstellen der gewünschten Bildschirmbetriebsart.\n" );

  ShowCursor( 0 );

  initialise_platform();
  initialise_display();
}

void exit_error( char *message )
{
  screen_interface.close_window();
  
  ShowCursor( 1 );
  MessageBox( NULL, message, "Fehler:", MB_OK );
  
  exit( 1 );
}

void exit_nofile( char *user, char *filename )
{
  char string[ 500 ];
  sprintf( string, "%s: Fehler beim Öffnen der Datei '%s'.\n", user, filename );

  exit_error( string );
}

void message( char *title, char *message )
{
  MessageBox( NULL, message, title, MB_OK ); 
}

#endif