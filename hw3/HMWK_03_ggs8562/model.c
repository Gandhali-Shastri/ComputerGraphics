// Dalio, Brian A.
// dalioba
// 2019-10-20
//----------------------------------------------------------
#include <ctype.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "model.h"

#define min(a,b) ({ __typeof__(a) _a = (a);  __typeof__(b) _b = (b); _a < _b ? _a : _b; })
#define max(a,b) ({ __typeof__(a) _a = (a);  __typeof__(b) _b = (b); _a > _b ? _a : _b; })

//----------------------------------------------------------
Model *allocModel( int numVertices, int numFaces )
{
  Model *model;

  model = (Model *) calloc( 1, sizeof( Model ) );
  if ( model == NULL ) {
    fprintf( stderr, "allocModel: Unable to allocate Model.\n" );
    exit( 1 );
  }

  if ( numVertices > 0 ) {
    model->m_numVertices = numVertices;
    model->m_vertex = (Vertex *) calloc( numVertices, sizeof( Vertex ) );
    if ( model->m_vertex == NULL ) {
      fprintf( stderr, "allocModel: Unable to allocate %d Vert%s.\n",
        numVertices, (numVertices == 1) ? "ex" : "ices" );
    }
  }

  if ( numFaces > 0 ) {
    model->m_numFaces = numFaces;
    model->m_face = (Face *) calloc( numFaces, sizeof( Face ) );
    if ( model->m_face == NULL ) {
      fprintf( stderr, "allocModel: Unable to allocate %d Face%s.\n",
        numFaces, (numFaces == 1) ? "" : "s" );
    }
  }

  return model;
}

//----------------------------------------------------------
void dumpModel( Model *model )
{
  printf( "#- Model parameters --------------------\n" );
  printf( "Vertex count : %5d\n", model->m_numVertices );
  printf( "Face count   : %5d\n", model->m_numFaces );
  printf( "Center       : ( %13.6f, %13.6f, %13.6f )\n",
    model->m_center.x, model->m_center.y, model->m_center.z );

  printf( "\n#- Vertex list -------------------------\n" );
  for ( int i=0; i<model->m_numVertices; i++ ) {
    printf( "  [%5d]  ", i );
    dumpVertex( &model->m_vertex[i] );
    putc( '\n', stdout );
  }

  printf( "\n#- Face list ---------------------------\n" );
  for ( int i=0; i<model->m_numFaces; i++ ) {
    printf( "  [%5d]  ", i );
    dumpFace( &model->m_face[i] );
    putc( '\n', stdout );
  }

  printf( "#---------------------------------------\n" );
}

//----------------------------------------------------------
void dumpFace( Face *face )
{
  printf( "%5d, %5d, %5d", face->v1, face->v2, face->v3 );
}

//----------------------------------------------------------
void freeModel( Model *model )
{
  if ( model->m_vertex != NULL ) {
    free( model->m_vertex );
  }

  if ( model->m_face != NULL ) {
    free( model->m_face );
  }

  free( model );
}

//----------------------------------------------------------
Model *loadModel( char *fName )
{
  FILE *fp = fopen( fName, "r" );
  if ( fp == NULL ) {
    fprintf( stderr, "loadModel: Unable to open '%s' for read.\n", fName );
    exit( 1 );
  }
	
  //--------------------------------------
  int     numVertices = 0;
  int     numFaces    = 0;
  char   *line        = NULL;
  size_t  len         = 0;
  size_t  read;

  // TODO: Declare variables to hold the mins and maxes for
  //       x, y, and z.  Initialize them properly.
  
  double Xmin = 0;
  double Ymin = 0;
  double Zmin = 0;
  double Xmax = 0;
  double Ymax = 0;
  double Zmax = 0;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ssize_t getdelim(char **linep, size_t *n, int delim, FILE *fp){
    int ch;
    size_t i = 0;
    if(!linep || !n || !fp){
        errno = EINVAL;
        return -1;
    }
    if(*linep == NULL){
        if(NULL==(*linep = malloc(*n=128))){
            *n = 0;
            errno = ENOMEM;
            return -1;
        }
    }
    while((ch = fgetc(fp)) != EOF){
        if(i + 1 >= *n){
            char *temp = realloc(*linep, *n + 128);
            if(!temp){
                errno = ENOMEM;
                return -1;
            }
            *n += 128;
            *linep = temp;
        }
        (*linep)[i++] = ch;
        if(ch == delim)
            break;
    }
    (*linep)[i] = '\0';
    return !i && ch == EOF ? -1 : i;
}

ssize_t getline(char **linep, size_t *n, FILE *fp){
    return getdelim(linep, n, '\n', fp);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  while ( ( read = getline( &line, &len, fp ) ) != -1 ) {
    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;
    if ( *ptr == 'v' ) ++numVertices;
    if ( *ptr == 'f' ) ++numFaces;
  }

  rewind( fp );

  //--------------------------------------
  Model *model = allocModel( numVertices, numFaces );

  Vertex *vertex  = model->m_vertex;
  Face   *face    = model->m_face;
  int     lineNum = 0;

  while ( ( read = getline( &line, &len, fp ) ) != -1 ) {
    lineNum++;

    char *ptr = line;

    while ( *ptr && isspace( *ptr ) ) ptr++;

    if ( *ptr == 'v' ) {
      int conv = sscanf( ptr+1, "%lf %lf %lf",
        &vertex->x, &vertex->y, &vertex->z );

      if ( conv != 3 ) {
        fprintf( stderr, "loadModel: Unable to load vertex at line %d.", lineNum );
      }

      // TODO: Keeping running values for mins and maxes
      //       for x, y, z.  (Use the min, max functions
      //       declared above.)
	  
	  Xmin = min(Xmin,vertex->x);
	  Ymin = min(Ymin,vertex->y);
	  Zmin = min(Zmin,vertex->z);
	  
	  Xmax = max(Xmax,vertex->x);
	  Ymax = max(Ymax,vertex->y);
	  Zmax = max(Zmax,vertex->z);
	  
	  //fprintf(stderr, "%f %f %f \n", Xmin, Ymin,Zmin);
	  //fprintf(stderr, "%f %f %f \n", Xmax, Ymax,Zmax);
	  
      vertex++;
      continue;
    }

    if ( *ptr == 'f' ) {
      int conv = sscanf( ptr+1, "%d %d %d",
        &face->v1, &face->v2, &face->v3 );

      face->v1--;
      face->v2--;
      face->v3--;

      if ( conv != 3 ) {
        fprintf( stderr, "loadModel: Unable to load face at line %d.", lineNum );
      }

      face++;
      continue;
    }
  }

  // TODO: Using the tracked mins and maxes for x, y, and z
  //       compute the center of the model and store its x,
  //       y, z coordinates in model->m_center.
	model->m_center.x = Xmin + ((Xmax - Xmin)/2);
	model->m_center.y = Ymin + ((Ymax - Ymin)/2);
	model->m_center.z = Zmin + ((Zmax - Zmin)/2);

  //--------------------------------------
  if ( line != NULL ) {
    free( line );
  }

  fclose( fp );

  //--------------------------------------
  return model;
}

//----------------------------------------------------------

