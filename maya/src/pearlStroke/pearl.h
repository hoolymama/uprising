/**
 * @file pearl.h
 * @brief Defines the pearl class for representing points with radius in a pearl chain.
 * 
 * This file contains the pearl class which represents a point with an associated radius.
 * Pearls are the basic building blocks of pearl chains, which can be used to create
 * stroke-like structures in Maya. The class provides methods for interpolation and
 * extrapolation between pearls.
 */
#ifndef _pearl_H
#define _pearl_H

#include <map>
#include <vector>
#include <maya/MFloatPoint.h>

/**
 * @class pearl
 * @brief Represents a point with an associated radius.
 * 
 * The pearl class encapsulates a point in 3D space along with a radius value.
 * It provides methods for interpolation and extrapolation between pearls,
 * which are useful for creating smooth transitions in pearl chains.
 */
class pearl
{
public:
	/**
	 * @brief Default constructor.
	 * Creates a pearl with default point and radius values.
	 */
	pearl():
		point(),
		radius()
	{	}

	/**
	 * @brief Constructor with 2D position and radius.
	 * @param px X coordinate of the point.
	 * @param py Y coordinate of the point.
	 * @param rval Radius value.
	 */
	pearl(float px, float py, float rval):
		point(px,py,0.0),
		radius(rval)
	{	}

	/**
	 * @brief Constructor with 3D position and radius.
	 * @param pval Point position.
	 * @param rval Radius value.
	 */
	pearl(MFloatPoint pval, float rval):
		point(pval),
		radius(rval)
	{	}

	MFloatPoint point;  /**< Position of the pearl. */
	float radius;       /**< Radius of the pearl. */

	/**
	 * @brief Linearly interpolates between two pearls.
	 * @param a First pearl.
	 * @param b Second pearl.
	 * @param w Weight factor (0.0 to 1.0) where 0.0 is fully b and 1.0 is fully a.
	 * @return A new pearl interpolated between a and b.
	 */
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

	/**
	 * @brief Extrapolates a new pearl from two existing pearls.
	 * @param p0 First pearl (reference point).
	 * @param p1 Second pearl (direction point).
	 * @param dist Distance to extrapolate beyond p1.
	 * @return A new pearl extrapolated from p0 through p1.
	 */
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