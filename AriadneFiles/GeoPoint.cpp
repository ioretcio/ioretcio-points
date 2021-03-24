#include "GeoPoint.h"
#include <math.h>
const int RADIUS_OF_TERRA = 6378137;
GeoPoint getOffset(GeoPoint start, double xmOffset, double ymOffset)
{
	GeoPoint result;
	result.Lat = xmOffset / RADIUS_OF_TERRA;
	result.Long = ymOffset/(RADIUS_OF_TERRA*cos(M_PI*start.Lat/180));
	return result;
}