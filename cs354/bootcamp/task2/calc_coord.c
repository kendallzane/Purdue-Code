#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double slope(double x1, double y1, double x2, double y2)
{
	return (y2 - y1) / (x2 - x1);
}


double distance (double A, double B, double C, double x1, double y1)
{
	return ( abs( (A * x1) + (B * y1) + C) / sqrt( (A*A) + (B*B) ) );
}



