#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//double pi = 3.1415;
//const double pipi = ;
#define DEGREES_TO_RADIANS(d) (3.1415 *d/180.0)

int main(){
	double a = 90, b=0,c=0;
	double pi =3.1415;
	
	double X =  pi * (a/180);
	double Y =  DEGREES_TO_RADIANS(b);
	double Z =  DEGREES_TO_RADIANS(c);

	double cosX = cos(X);	double sinX = sin(X);
	double cosY = cos(90);	double sinY = sin(Y);
	double cosZ = cos(1);	double sinZ = sin(Z);
	
	printf("%d %d %d\n",cosX,cosY,cosZ);
	//printf("%d %d %d\n",M_PI);
	return 0;
}

