#ifndef _pearl_H
#define _pearl_H

#include <map>
#include <vector>
#include <maya/MFloatPoint.h>

/**
 * @brief A point in an image.
 * 
 */


class pearl
{
public:



	pearl():
		point(),
		radius()
	{	}

	pearl(float px, float py, float rval):
		point(px,py,0.0),
		radius(rval)
	{	}

	pearl(MFloatPoint pval, float rval):
		point(pval),
		radius(rval)
	{	}

	MFloatPoint point;
	float radius;


	static pearl lerp(const pearl &a, const pearl &b, float w)
	{
		float iw = 1.0 - w;
		if (w == 1.0)
		{
			return pearl(a);
		}
		if (w == 0.0)
		{
			return pearl(b);
		}
		return pearl(
		         MFloatPoint((a.point * w) + (b.point * iw)),
		         (a.radius * w) + (b.radius * iw)
		       );
	}


	static pearl extrapolate(const pearl &p0, const pearl &p1, float dist)
	{
		const float  epsilon = .0001;
		if (dist  < epsilon)
		{
			return pearl(p1);
		}

		MFloatPoint dp =  p1.point - p0.point;
		// float dy =  p1.y - p0.y;
		float denom = dp.distanceTo(MFloatPoint::origin);
		if (denom < epsilon) {
			return pearl(p1);
		}
		float recip = dist / denom;

		MFloatPoint p = p1.point + (dp * recip);

		return pearl(p , p1.radius);
	}

};


#endif