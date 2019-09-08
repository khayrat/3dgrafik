#ifndef SCREEN_INTERFACE_H
#define SCREEN_INTERFACE_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ddraw.h>

#include "simple_types.h"

void exit_error( char *message );
void exit_nofile( char *user, char *filename );
void message( char *title, char *message );

#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK main_window_procedure( HWND main_window_handle, UINT message, WPARAM wparam, LPARAM lparam )
{
  if( message == WM_CLOSE ) {  PostQuitMessage( 0 );  return 0;  }
  
  return DefWindowProc( main_window_handle, message, wparam, lparam );
}

class software_interface
{
  private:
    long x_resolution, y_resolution;

    LPDIRECTDRAW main_dd_object;
    LPDIRECTDRAWSURFACE primary_surface;
    DDSURFACEDESC surface_description;
    HWND main_window_handle;
    
    void initialise_platform( long x, long y, long bit_depth );

  public:
    long get_xr( void ) {  return x_resolution;  }
    long get_yr( void ) {  return y_resolution;  }

    void open_window( HINSTANCE hInstance, long x, long y, long bit_depth );
    void close_window( void );

    void *get_screen_pointer( void );
    void release_screen_pointer( void ) {  primary_surface->Unlock( surface_description.lpSurface );  }

    software_interface( void )
    {
      x_resolution = y_resolution = 0;
      main_dd_object = NULL;  primary_surface = NULL;  main_window_handle = NULL;
      memset( &surface_description, 0, sizeof( surface_description ) );
    }
   ~software_interface( void ) {  close_window();  }
} screen_interface;

void software_interface::initialise_platform( long x, long y, long bit_depth )
{
  if( DirectDrawCreate( NULL, &main_dd_object, NULL ) != DD_OK )
    exit_error( "Fehler während der Ausführung von DirectDrawCreate().\n" );

  if
  ( 
    main_dd_object->SetCooperativeLevel
	( 
	  main_window_handle,
	  DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX 
	)
    
	!= DD_OK 
  )
  exit_error( "Fehler während der Ausführung von SetCooperativeLevel().\n" );

  if( main_dd_object->SetDisplayMode( x, y, bit_depth ) != DD_OK )
    exit_error( "Fehler während der Ausführung von SetDisplayMode().\n" );
  
  memset( &surface_description, 0, sizeof( surface_description ) );
  surface_description.dwSize = sizeof( surface_description );
  surface_description.dwFlags = DDSD_CAPS;
  surface_description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

  if( main_dd_object->CreateSurface( &surface_description, &primary_surface, NULL ) != DD_OK )
    exit_error( "Fehler während der Ausführung von CreateSurface().\n" );
}

void software_interface::open_window( HINSTANCE hInstance, long x, long y, long bit_depth )
{
  x_resolution = x;  y_resolution = y;

  WNDCLASS winclass;

  winclass.style = CS_OWNDC;
  winclass.lpfnWndProc = main_window_procedure;
  winclass.cbClsExtra = 0;
  winclass.cbWndExtra = 0;
  winclass.hInstance = hInstance;
  winclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  winclass.hCursor = LoadCursor( NULL, IDC_ARROW );
  winclass.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
  winclass.lpszMenuName = NULL;
  winclass.lpszClassName = "Main Window";
  RegisterClass( &winclass );

  char window_name[] = "3D-Grafik Programmierung";

  if( !(main_window_handle = CreateWindowEx( WS_EX_TOPMOST, "Main Window", window_name, WS_VISIBLE | WS_POPUP, 0,0,
                                             GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), NULL,
                                             NULL, hInstance, NULL )) )
    exit_error( "Fehler beim Öffnen des Applikationsfensters.\n" );

  ShowCursor( 0 );

  initialise_platform( x_resolution, y_resolution, bit_depth );
}

void software_interface::close_window( void )
{   
  if( primary_surface )
  {    
    primary_surface->Release();
    primary_surface = NULL;
  }
  	
  if( main_dd_object )
  {
    main_dd_object->Release();
    main_dd_object = NULL;
  }
  
  DestroyWindow( main_window_handle );
}    

void *software_interface::get_screen_pointer( void )
{
  primary_surface->Lock( NULL, &surface_description, DDLOCK_SURFACEMEMORYPTR, NULL );

  return surface_description.lpSurface;
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