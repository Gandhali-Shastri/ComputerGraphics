// Dalio, Brian A.
// dalioba
// 2019-11-05
//----------------------------------------------------------
#include <stdio.h>

#include "bezier.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

void _computeCoefficients( double u, double v, double *c );

//----------------------------------------------------------
void generateBezierPoints( Model *model, int resolution )
{
  // TODO: Iterate through each patch, each value of u, and each
  //       value of v (so three nested loops in that order).
  //       Compute the 16 coefficients for the specific values of
  //       u and v.
  //       Iterate through the 16 coefficients multiplying each by
  //       the corresponding control point and accumulating the
  //       result in the current Bézier surface point.
  //       This is done for that point's x, y, and z coordinates.
	
	double u[resolution], v[resolution];
	double coefficients[16]; 
	double end = 1.0, start = 0.0;
    int partitions = resolution - 1; 
    double length = (end - start) / partitions; 
	int n = sizeof(u)/sizeof(u[0]);
	u[resolution-1] = end; v[resolution-1] = end;
	u[0] = start; v[0] = start;
	
    for (int i = 1; i < resolution - 1; i ++) {
        u[i] = (start + i * length);
		v[i] = (start + i * length);
    }

int z = 0;
for (int i = 0 ; i < model->m_numPatches; i++){
		for (int j = 0 ; j < n; j++){
			for (int k = 0 ; k < n; k++){
				_computeCoefficients( u[j], v[k], coefficients );	
                for( int l = 0; l < (sizeof(coefficients)/sizeof(coefficients[0])); l++ ) {
                    model->m_patchVertex[z].x += coefficients[l] * model->m_vertex[ model->m_patch[i][l] ].x;
                    model->m_patchVertex[z].y += coefficients[l] * model->m_vertex[ model->m_patch[i][l] ].y;
                    model->m_patchVertex[z].z += coefficients[l] * model->m_vertex[ model->m_patch[i][l] ].z;

                }			
                z++;

			}	
		}
	}
}

//----------------------------------------------------------
void generateBezierTriangles( Model *model, int resolution )
{
  // TODO: Iterate through each patch, each row of that patch, and
  //       and each column of that patch (so three nested loops in
  //       that order).
  //       Dump the two triangles that correspond to the row and
  //       column positions using the dumpTriangle() routine.
  //       Remember that you have to consider only the first
  //       through the penultimate row and the first through the
  //       penultimate column when generating a patch's triangles.
  //       Ensure that you generate the triangles in the correct
  //       order and use the vertices in the correct order.
  
  for (int i = 0 ; i < model->m_numPatches; i++){
		for (int row = 0 ; row < resolution-1; row++){		
			for (int col = 0 ; col < resolution-1; col++){
				int patch = i * resolution * resolution;
				int rowStart =  patch + row * resolution;
				int here = rowStart + col;
				int there = here + resolution;
			
				dumpTriangle(&model->m_patchVertex[here],&model->m_patchVertex[there],&model->m_patchVertex[there+1]);
				dumpTriangle(&model->m_patchVertex[there+1],&model->m_patchVertex[here+1],&model->m_patchVertex[here]);
				
			}
		}
  
  } 
	/*//int n = 0;
    for (int i = 0 ; i < model->m_numPatches; i++){
		for (int row = 0 ; row < resolution-1; row++){			
			for (int col = 0 ; col < resolution-1; col++){
				int v1t1 = (i * resolution * resolution) + (row * resolution) + col;
				int v2t1 = (i * resolution * resolution) + (row * resolution) + (col + 1);
				int v3t1 = (i * resolution * resolution) + ((row+1) * resolution) + (col + 1);
				
				int v1t2 = (i * resolution * resolution) + ((row+1) * resolution) + (col + 1);
				int v2t2 = (i * resolution * resolution) + ((row+1) * resolution) + (col);
				int v3t2 = (i * resolution * resolution) + (row * resolution) + (col);
			
				dumpTriangle(&model->m_patchVertex[v1t1],&model->m_patchVertex[v2t1],&model->m_patchVertex[v3t1]);
				dumpTriangle(&model->m_patchVertex[v1t2],&model->m_patchVertex[v2t2],&model->m_patchVertex[v3t2]);
				
			}
		}
  
  } */
}

//----------------------------------------------------------
void _computeCoefficients( double u, double v, double *coefficients )
{
  // TODO: For the given u and v values, generate the 16 Bernstein
  //       coefficients.  Ensure that you compute and store them
  //       in the proper order: c00, c01, c02, c03, c10, c11, c12,
  //       c13, c20, c21, c22, c23, c30, c31, c31, c32, c33.
  //double u = u, v=v;
  double u2 = u*u;		double v2 = v*v;
  double u3 = u2*u;		double v3 = v2*v; 
  
  double one_u = (1-u);			double one_v = (1-v);
  double one_u2 = one_u * one_u;		double one_v2 = one_v * one_v;
  double one_u3 = one_u2 * one_u;		double one_v3 = one_v2 * one_v;
  
  
   coefficients[0]= (one_u3) * (one_v3);			  	coefficients[1] = 3*v*(one_u3) * (one_v2);
   coefficients[2] = 3*v2*(one_u3) * (one_v);	  		coefficients[3] = v3*(one_u3);
  
   coefficients[4] = 3*u*(one_u2) * (one_v3);		  	coefficients[5] = 9*u*v*(one_u2) * (one_v2);
    coefficients[6] = 9*u*v2*(one_u2) * (one_v);	  	coefficients[7]= 3*u*v3*(one_u2);
  
    coefficients[8] = 3*u2*(one_u) * (one_v3);	  		coefficients[9]= 9*u2*v*(one_u) * (one_v2);
    coefficients[10] = 9*u2*v2*(one_u) * (one_v);	  	coefficients[11]= 3*u2*v3*(one_u);
  
    coefficients[12] = u3* (one_v3);				  	coefficients[13]= 3*u3*v*(one_v2);
    coefficients[14]= 3*u3*v2*(one_v);			  		coefficients[15]= u3*v3;

	/* for (int k = 0 ; k < (sizeof(coefficients)/sizeof(coefficients[0])); k++){
			
				printf("%d %f\n", k,coefficients[k]);
	} */
}

//----------------------------------------------------------

