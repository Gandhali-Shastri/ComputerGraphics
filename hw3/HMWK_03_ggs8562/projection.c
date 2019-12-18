// Dalio, Brian A.
// dalioba
// 2019-10-20
//----------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "projection.h"

//----------------------------------------------------------
Projection *allocProjection()
{
  Projection *p;

  p = (Projection *) calloc( 1, sizeof( Projection ) );
  if ( p == NULL ) {
    fprintf( stderr, "allocProjection: Unable to allocate Projection.\n" );
    exit( 1 );
  }

  return p;
}

//----------------------------------------------------------
Projection *computeProjection( View *v )
{
  Projection *p = allocProjection();

  // TODO: Compute the proper values of fx, fy, gx, gy, sx, sy,
  //       ax, and ay and store them in p->...
  //       Save the camera distance from the view in the
  //       projection.
  
	  double m_fx = 0.0, m_fy = 0.0;
	  double m_gx = 0.0, m_gy = 0.0;
	  double m_sx = 0.0, m_sy = 0.0;
	  double m_ax = 0.0, m_ay = 0.0;
	  
	  p->m_fx =  -(v->m_worldXMin);
	  p->m_fy = -(v->m_worldYMin);
	  
	  p->m_gx = v->m_width * v->m_viewportXMin;
	  p->m_gy = v->m_height * v->m_viewportYMin;
	  
	  p->m_sx = v->m_width *(v->m_viewportXMax - v->m_viewportXMin)/ (v->m_worldXMax - v->m_worldXMin);
	  p->m_sy = v->m_height * (v->m_viewportYMax - v->m_viewportYMin)/ (v->m_worldYMax - v->m_worldYMin);
	  
	  p->m_ax = p->m_fx * p->m_sx + p->m_gx;
	  p->m_ay = p->m_fy * p->m_sy + p->m_gy;
	  
	  p->m_cameraDistance = v->m_cameraDistance;

  return p;
}

//----------------------------------------------------------
void dumpProjection( Projection *p )
{
  printf( "#- Projection parameters ---------------\n" );
  printf( "(fx, fy) : ( %13.6f, %13.6f )\n", p->m_fx, p->m_fy );
  printf( "(gx, gy) : ( %13.6f, %13.6f )\n", p->m_gx, p->m_gy );
  printf( "(sx, sy) : ( %13.6f, %13.6f )\n", p->m_sx, p->m_sy );
  printf( "(ax, ay) : ( %13.6f, %13.6f )\n", p->m_ax, p->m_ay );
  printf( "Camera distance : %13.6f\n", p->m_cameraDistance );
  printf( "#---------------------------------------\n" );
}

//----------------------------------------------------------
void freeProjection( Projection *p )
{
  free( p );
}

//----------------------------------------------------------
void projectVertexList( Projection *p, Vertex *v, int numVertices )
{
  // TODO: Using the projection parameters in p, traverse the
  //       given list of vertices (there are numVertices of them)
  //       and project each vertex:
  //         1. If camera distance is not 0.0, first do a
  //            perspective adjustment.
  //         2. Once the vertex is adjusted for perspective,
  //            calculate its corresponding screen coordinates.
					
			for (int i = 0;i <numVertices; i++){
				
				if (p->m_cameraDistance == 0.0){
				
				v[i].x = p->m_sx*(v[i].x)+p->m_ax;
				v[i].y = p->m_sy*(v[i].y)+p->m_ay;			
			}
			else{
				v[i].x = p->m_sx * ((v[i].x) / (1- (v[i].z / p->m_cameraDistance))) + p->m_ax;
				v[i].y = p->m_sy * ((v[i].y) / (1- (v[i].z / p->m_cameraDistance))) + p->m_ay;
			}
		} 
  
}

//----------------------------------------------------------
#define DEGREES_TO_RADIANS(d) (M_PI*(d)/180.0)

void rotateVertexList( View *view, Vertex *vertex, int numVertices, Vertex center )
{
  // TODO: Using the Euler angles given in the view, traverse the
  //       given list of vertices (there are numVertices of them)
  //       and rotate each vertex.
  //
  //       Compute the r00 through r22 values and the ex, ey, ez
  //       values _before_ looping through the vertex list!

		double X =  DEGREES_TO_RADIANS(view->m_phi);
		double Y =  DEGREES_TO_RADIANS(view->m_theta);
		double Z =  DEGREES_TO_RADIANS(view->m_psi);
		
		double cosX = cos(X);	double sinX = sin(X);
		double cosY = cos(Y);	double sinY = sin(Y);
		double cosZ = cos(Z);	double sinZ = sin(Z);
		
		double r00 = cosZ * cosY;	
		double r01 = -cosY * sinZ;	
		double r02 = sinY;
		
		double r10 = cosX * sinZ + cosZ * sinX * sinY; 
		double r11 = cosX * cosZ - sinX* sinZ* sinY; 
		double r12 = -cosY * sinX; 
		
		double r20 = -cosY * cosZ * sinY + sinX * sinZ; 
		double r21 = cosX * sinZ* sinY + cosZ * sinX;
		double r22 = cosX * cosY; 
		
		double ex = -r00 * center.x - r01 * center.y - r02 * center.z + center.x;
		double ey = -r10 * center.x - r11 * center.y - r12 * center.z + center.y;
		double ez = -r20 * center.x - r21 * center.y - r22 * center.z + center.z;
		
		
		
		for (int i = 0;i <numVertices; i++){
			double xx = r00 * vertex[i].x + r01 * vertex[i].y + r02 * vertex[i].z + ex;
			double yy = r10 * vertex[i].x + r11 * vertex[i].y + r12 * vertex[i].z + ey;
			double zz = r20 * vertex[i].x + r21 * vertex[i].y + r22 * vertex[i].z + ez;
			
			vertex[i].x  = xx;
			vertex[i].y = yy;
			vertex[i].z = zz;
			
			//printf("%f\n",xx);
			//printf("%f\n",yy);
			//printf("%f\n",zz);
		}
		

}

//----------------------------------------------------------

