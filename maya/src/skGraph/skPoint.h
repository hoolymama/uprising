#ifndef _skPoint_H
#define _skPoint_H

#include <map>
#include <vector>


// #include <maya/MVector.h>
const float  epsilon = .0001;
class skPoint
{
public:




	skPoint():
		x(),
		y(),
		radius()
	{	}

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


	static skPoint extrapolate(const skPoint &p0, const skPoint &p1, float dist)
	{

		if (dist  < epsilon)
		{
			return skPoint(p1);
		}

		float dx =  p1.x - p0.x;
		float dy =  p1.y - p0.y;
		float denom = sqrt(dx * dx + dy * dy);
		if (denom < epsilon) {
			return skPoint(p1);
		}
		float recip = dist / denom;

		float x = p1.x + (dx * recip);
		float y = p1.y + (dy * recip);
		return skPoint(x, y, p1.radius);
	}





	// skPoint(int x, int y, int z = 0);
	// skPoint(const coord &rhs);
	// ~skPoint();

};


#endif