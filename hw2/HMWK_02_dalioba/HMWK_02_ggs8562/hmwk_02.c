// Shastri, Gandhali
// ggs8562
// 2019-10-09
//----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "view.h"

//----------------------------------------------------------
int main( int argc, char *argv[] )
{
  if ( argc != 3 ) {
    fprintf( stderr, "Usage: %s <modelFileName> <viewFileName>\n", argv[0] );
    exit( 1 );
  }

  //--------------------------------------
  Model *m;
  m = loadModel( argv[1] );
  dumpModel( m );

  putc( '\n', stdout );
  View *v;
  v = loadView( argv[2] );
  dumpView( v );

  //--------------------------------------
  double fx = 0.0, fy = 0.0;
  double gx = 0.0, gy = 0.0;
  double sx = 0.0, sy = 0.0;
  double ax = 0.0, ay = 0.0;

  // TODO: Write something here to compute fx, fy, gx, gy, sx,
  //    sy, ax, and ay using the information in the View v.
  //    (All the math you need is recapped in the
  //    "4303 Projection Parallel.pdf" handout.)
  fx =  -(v->m_worldXMin);
  fy = -(v->m_worldYMin);
  
  gx = v->m_width * v->m_viewportXMin;
  gy = v->m_height * v->m_viewportYMin;
  
  sx = v->m_width *(v->m_viewportXMax - v->m_viewportXMin)/ (v->m_worldXMax - v->m_worldXMin);
  sy = v->m_height * (v->m_viewportYMax - v->m_viewportYMin)/ (v->m_worldYMax - v->m_worldYMin);
  
  ax = fx*sx+gx;
  ay = fy*sy+gy;

  printf( "\n#- Projection parameters ---------------\n" );
  printf( "(fx, fy) : ( %13.6f, %13.6f )\n", fx, fy );
  printf( "(gx, gy) : ( %13.6f, %13.6f )\n", gx, gy );
  printf( "(sx, sy) : ( %13.6f, %13.6f )\n", sx, sy );
  printf( "(ax, ay) : ( %13.6f, %13.6f )\n", ax, ay );
  printf( "#---------------------------------------\n" );

  //--------------------------------------
  printf( "\n#- Transformed vertices ----------------\n" );
  for ( int i=0; i<m->m_numVertices; i++ ) {
    double x = 0.0, y = 0.0;
    double pX = 0.0, pY = 0.0;
	
    // TODO: Write something here to get the x, y coordinates of
    //    vertex i of Model m and then compute the projected
    //    values pX and pY using x and y and the projecion
    //    parameters computed above.  (Again, all the math you
    //    need is in the handout.)
	
	x =  m->m_vertex[i].x;
	y =  m->m_vertex[i].y;
	
	pX = sx*x+ax;
	pY = sy*y+ay;
	
    printf( "  [%5d] ( %13.6f, %13.6f ) => ( %8.1f, %8.1f )\n",
      i, x, y, pX, pY );
  }
  printf( "#---------------------------------------\n" );

  //--------------------------------------
  printf( "\n#- Triangles to draw -------------------\n" );
  for ( int i=0; i<m->m_numFaces; i++ ) {
    double x = 0.0, y = 0.0;
    double v1pX = 0.0, v1pY = 0.0;
    double v2pX = 0.0, v2pY = 0.0;
    double v3pX = 0.0, v3pY = 0.0;
	
    // TODO: Write something here to get the x, y coordinates of
    //    each of the vertices of face i and compute the
    //    projected values v1pX, v1pY, v2pX, v2pY, v3pX, and v3pY
    //    using the projection parameters computed above.  (All
    //    the math you need is still in the handout.  It hasn't
    //    disappeared.)
	
	int v1 = m->m_face[i].v1;
	int v2 = m->m_face[i].v2;
	int v3 = m->m_face[i].v3;
	
	v1pX = sx*(m->m_vertex[v1].x)+ax;
	v1pY = sy*(m->m_vertex[v1].y)+ay;
	v2pX = sx*(m->m_vertex[v2].x)+ax;
	v2pY = sy*(m->m_vertex[v2].y)+ay;
	v3pX = sx*(m->m_vertex[v3].x)+ax;
	v3pY = sy*(m->m_vertex[v3].y)+ay;
	 
	
    printf( "  [%5d] ( %8.1f, %8.1f ), ( %8.1f, %8.1f ), ( %8.1f, %8.1f )\n",
      i, v1pX, v1pY, v2pX, v2pY, v3pX, v3pY );
  }
  printf( "#---------------------------------------\n" );

  //--------------------------------------
  freeModel( m );
  freeView( v );
}

//----------------------------------------------------------

