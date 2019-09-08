#ifndef HARDWARE_PALETTE_H
#define HARDWARE_PALETTE_H

struct hardware_palette
{
  long material_count;
  D3DMATERIAL9 *materials;

  hardware_palette( char *filename );
 ~hardware_palette( void ) {  if( materials ) delete [] materials;  }
} palette( "palette.pl1" );

hardware_palette::hardware_palette( char *filename )
{
  FILE *input;

  if( (input = fopen( filename, "rb" )) == NULL )
    exit_nofile( "hardware_palette", filename );

  fscanf( input, "%ld", &material_count );

  if( (materials = new D3DMATERIAL9[ material_count ]) == NULL )
    exit_error( "*materials: Fehler während der Reservierung von Arbeitsspeicher.\n ");

  for( long x=0 ; x<material_count ; x++ )
  {
    memset( &materials[ x ], 0, sizeof( D3DMATERIAL9 ) );

    float r1, g1, b1, r2, g2, b2;

    fscanf( input, "%f %f %f %f %f %f", &r1, &g1, &b1, &r2, &g2, &b2 );
    r1 /= 255;  g1 /= 255;  b1 /= 255;  r2 /= 255;  g2 /= 255;  b2 /= 255;

    materials[ x ].Ambient.r = r1;    materials[ x ].Diffuse.r = r2;
    materials[ x ].Ambient.g = g1;    materials[ x ].Diffuse.g = g2;
    materials[ x ].Ambient.b = b1;    materials[ x ].Diffuse.b = b2;
    materials[ x ].Ambient.a = 0;     materials[ x ].Diffuse.a = 0;
  }

  fclose( input );
}

#endif