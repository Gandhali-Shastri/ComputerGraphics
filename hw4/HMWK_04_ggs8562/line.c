// Shastri, Gandhali G.
// ggs8562
// 2019-10-30
//----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "line.h"

//----------------------------------------------------------
#define INSIDE (0)
#define LEFT   (1)
#define RIGHT  (2)
#define BELOW  (4)
#define ABOVE  (8)

#define EPSILON (1e-13)

int _regionCode( double x, double y, View *v );

//----------------------------------------------------------

int clipLine( View *v, Line *l )
{
  // TODO: Clip the line l against the portal limits
  //       in the view using the Cohen-Sutherland
  //       method as described in class.
  //       If p1 or p2 of the line is updated, ensure
  //       that you change it in the Line structure.
  //       Return whether the line should be drawn
  //       at all (0 for no, 1 for yes).
  
  int p1Out = 0, p2Out = 0, anOutCode = 0;
  double x,y;
  double x1 = l->p1X, y1 = l->p1Y;
  double x2 = l->p2X, y2 = l->p2Y;
  
  double xMin = v->m_portalXMin;
  double xMax = v->m_portalXMax;
  double yMin = v->m_portalYMin;
  double yMax = v->m_portalYMax;
 
  p1Out = _regionCode(x1,y1,v);
  p2Out = _regionCode(x2,y2,v);

  if (p1Out == INSIDE && p2Out == INSIDE){
		  return 1;
	  }
  if((p1Out & p2Out) != INSIDE){
		  return 0;
	}
  
  if (p1Out == INSIDE)
	anOutCode = p2Out;
  else 
	anOutCode = p1Out;

  if ( anOutCode & ABOVE ){
	//Move point along the line down to Y max.
	x = x1 + ( x2 - x1 )*( yMax - y1 )/( y2 - y1 );
	y = yMax;
  }
  else if ( anOutCode & BELOW ){
		//Move point along the line up to Y min.
		x = x1 + ( x2 - x1 )*( yMin - y1 )/( y2 - y1 );
		y = yMin;
	  }
  else if ( anOutCode & RIGHT ){
		//Move it along the line over to X max.
		x = xMax;
		y = y1 + ( y2 - y1 )*( xMax - x1 )/( x2 - x1 );
  }
  else if( anOutCode & LEFT ){
		//Move it along the line over to X min.
		x = xMin;
		y = y1 + ( y2 - y1 )*( xMin - x1 )/( x2 - x1 );
  }
  
  if (anOutCode == p1Out){
		l->p1X = x;
		l->p1Y = y;
		//p1Out = _regionCode(l->p1X,l->p1Y,v);
	}
 else {
		l->p2X = x;
		l->p2Y = y;
		//p2Out = _regionCode(l->p2X,l->p2X,v);
}
return clipLine(v,l);
}

//----------------------------------------------------------
void dumpLine( Line *l )
{
  printf( "l %8.1f %8.1f %8.1f %8.1f\n",
    l->p1X, l->p1Y, l->p2X, l->p2Y );
}

//----------------------------------------------------------
int _regionCode( double x, double y, View *v )
{
  // TODO: Compute the region code for the point x, y
  //       by comparing against the portal limits in
  //       the view.  (Don't forget to use EPSILON
  //       comparisons!)
  //       Return the computed code.
  int code = INSIDE;
  double xMin = v->m_portalXMin;
  double xMax = v->m_portalXMax;
  double yMin = v->m_portalYMin;
  double yMax = v->m_portalYMax;
	  
	if ( (xMin - x) > EPSILON )
		code = code | LEFT;
	  if ( (x - xMax) > EPSILON )
		code = code | RIGHT;
	  if ( (yMin - y) > EPSILON)
		code = code | BELOW;
	  if ( (y - yMax) > EPSILON)
		code = code | ABOVE; 
		
		return code;
		
	/*if ( x < xMin )
	code = code | LEFT;
	
	else if ( x > xMax )
	code = code | RIGHT;
	
	if ( y < yMin )
	code = code | BELOW;
	
	else if ( y > yMax )
	code = code | ABOVE;
	  
	  return code;*/
  
  
  
}

//----------------------------------------------------------

