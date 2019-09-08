#ifndef SCREEN_INTERFACE_H
#define SCREEN_INTERFACE_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "simple_types.h"

void exit_error( char *message );
void exit_nofile( char *user, char *filename );
void message( char *title, char *message );

#define x_res screen_interface.get_xr()
#define y_res screen_interface.get_yr()

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

    HWND main_window_handle;
    HDC device_context;
    HGLRC rendering_context;

    void create_window
    (
      HINSTANCE instance, DWORD style,
      int xp, int yp, int xres, int yres, int x_add, int y_add
    );    
    void initialise_platform( void );

  public:
    void fullscreen( HINSTANCE instance, long xr, long yr, long bit_depth );
	void open_window( HINSTANCE instance, long xr, long yr, long bit_depth );
    void close_window( void );
    
	long get_xr( void ) {  return x_resolution;  }
    long get_yr( void ) {  return y_resolution;  }

    void swap_buffers( void ) {  SwapBuffers( device_context );  }

	hardware_interface( void ) : enlarged( 0 ), x_resolution( 0 ), y_resolution( 0 )
    {
      main_window_handle = NULL;  device_context = NULL;  rendering_context = NULL;
    }
   ~hardware_interface( void ) {  close_window();  }
} screen_interface;

void hardware_interface::create_window( HINSTANCE instance, DWORD style, int xp, int yp, int xres, int yres, int x_add, int y_add )
{
  WNDCLASS winclass;

  winclass.style = CS_OWNDC;
  winclass.lpfnWndProc = main_window_procedure;
  winclass.cbClsExtra = 0;
  winclass.cbWndExtra = 0;
  winclass.hInstance = instance;
  winclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  winclass.hCursor = LoadCursor( NULL, IDC_ARROW );
  winclass.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
  winclass.lpszMenuName = NULL;
  winclass.lpszClassName = "Main Window";

  RegisterClass( &winclass );
  char window_name[] = "3D-Grafik Programmierung";

  if
  (
    (
      main_window_handle = CreateWindow
      ( 
        "Main Window", window_name, style,
         xp, yp, xres+x_add, yres+y_add,
         NULL, NULL, instance, NULL
      )
    ) == NULL
  )
  exit_error( "Fehler beim Öffnen des Programmfensters.\n" );
}

void hardware_interface::open_window( HINSTANCE instance, long xr, long yr, long bit_depth )
{
  x_resolution = xr;  y_resolution = yr;

  int x_add = 2 * (GetSystemMetrics( SM_CXBORDER ) + GetSystemMetrics( SM_CXEDGE ));
  int y_add = 2 * (GetSystemMetrics( SM_CYBORDER ) + GetSystemMetrics( SM_CYEDGE )) + GetSystemMetrics( SM_CYCAPTION );

  create_window
  (
    instance, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
    100, 100, x_resolution, y_resolution, x_add, y_add
  );

  initialise_platform();
}

void hardware_interface::fullscreen( HINSTANCE instance, long xr, long yr, long bit_depth )
{
  x_resolution = xr;  y_resolution = yr;
  enlarged = 1;

  create_window( instance, WS_VISIBLE | WS_POPUP, 0, 0, x_resolution, y_resolution, 0, 0 );

  DEVMODE new_screen_settings;
  
  memset( &new_screen_settings, 0, sizeof( new_screen_settings ) );
  new_screen_settings.dmSize = sizeof( new_screen_settings );
  new_screen_settings.dmPelsWidth = xr;
  new_screen_settings.dmPelsHeight = yr;
  new_screen_settings.dmBitsPerPel = bit_depth;
  new_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

  if( ChangeDisplaySettings( &new_screen_settings, 0 ) != DISP_CHANGE_SUCCESSFUL )
  {
    char string[ 100 ];
	sprintf( string, "Fehler beim Einstellen des Vollbildmodus %ldx%ldx%ld.\n", xr, yr, bit_depth );
    exit_error( string );
  }

  ShowCursor( 0 );

  initialise_platform();
}

void hardware_interface::initialise_platform( void )
{	
  device_context = GetDC( main_window_handle );

  PIXELFORMATDESCRIPTOR pfd;

  memset( &pfd, 0, sizeof( pfd ) );
  pfd.nSize = sizeof( pfd );
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  pfd.iLayerType = PFD_MAIN_PLANE;

  int format = ChoosePixelFormat( device_context, &pfd );
  SetPixelFormat( device_context, format, &pfd );

  rendering_context = wglCreateContext( device_context );
  wglMakeCurrent( device_context, rendering_context );	
}

void hardware_interface::close_window( void )
{
  if( enlarged )
    if( ChangeDisplaySettings( NULL, 0 ) != DISP_CHANGE_SUCCESSFUL )
	  exit_error( "Fehler beim Einstellen der ursprünglichen Bildschirmbetriebsart.\n" );

  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( rendering_context );
  ReleaseDC( main_window_handle, device_context );

  DestroyWindow( main_window_handle );
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