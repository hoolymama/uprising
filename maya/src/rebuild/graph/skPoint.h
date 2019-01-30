#ifndef _skPoint_H
#define _skPoint_H

#include <map>
#include <vector>

// #include <maya/MVector.h>

class skPoint
{
public:

	skPoint(float xval, float yval, float rval):
		x(xval),
		y(yval),
		radius(rval)
	{	}

	float x;
	float y;
	// float param;
	float radius;


	static skPoint lerp(const skPoint &a, const skPoint &b, float w)
	{
		float iw = 1.0 - w;
		if (w == 1.0)
		{
			return skPoint(a);
		}
		return skPoint(
		         (a.x * w) + (b.x * iw),
		         (a.y * w) + (b.y * iw),
		         (a.radius * w) + (b.radius * iw)
		       );
	}

	// skPoint(int x, int y, int z = 0);
	// skPoint(const coord &rhs);
	// ~skPoint();

};


#endif