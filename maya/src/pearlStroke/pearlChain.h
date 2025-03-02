/**
 * @file pearlChain.h
 * @brief Defines the pearlChain class for managing sequences of pearls.
 * 
 * This file contains the pearlChain class which provides functionality for managing
 * sequences of pearl objects. Pearl chains can be used to create stroke-like structures
 * in Maya, with each pearl representing a point with an associated radius.
 */
#ifndef _pearlChain_H
#define _pearlChain_H
#include <vector>

#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrix.h>



#include "pearl.h"

/**
 * @class pearlChain
 * @brief Manages a sequence of pearls.
 * 
 * The pearlChain class encapsulates a sequence of pearl objects, providing methods
 * for adding pearls, accessing pearls, interpolating between pearls, and converting
 * the chain to various Maya data structures for visualization and processing.
 */
class pearlChain
{
public:
	/**
	 * @brief Default constructor.
	 * Creates an empty pearl chain.
	 */
	pearlChain();

	/**
	 * @brief Destructor.
	 */
	~pearlChain();

	/**
	 * @brief Gets the number of pearls in the chain.
	 * @return The number of pearls.
	 */
	int size() const;

	/**
	 * @brief Adds a pearl to the chain.
	 * @param pt The pearl to add.
	 */
	void add(const pearl &pt);

	/**
	 * @brief Appends the pearl positions to a point array.
	 * @param points The point array to append to.
	 * @param wm World matrix to transform the points (default: identity).
	 */
	void appendPointsTo(MPointArray &points, const MMatrix &wm=MMatrix::identity) const;
	
	/**
	 * @brief Appends the pearl positions to a float point array.
	 * @param points The float point array to append to.
	 * @param wm World matrix to transform the points (default: identity).
	 */
	void appendPointsTo(MFloatPointArray &points, const MMatrix &wm=MMatrix::identity) const;
	
	/**
	 * @brief Appends the pearl positions to a vector array.
	 * @param positions The vector array to append to.
	 */
	void appendPointsTo(MVectorArray &positions) const;

	/**
	 * @brief Access operator for pearls in the chain.
	 * @param index Index of the pearl to access.
	 * @return Constant reference to the pearl at the specified index.
	 */
	const pearl &operator[](const int index) const;

	/**
	 * @brief Appends the pearl radii to a float array.
	 * @param radii The float array to append to.
	 * @param scale Scale factor to apply to the radii (default: 1.0).
	 */
	void appendRadiiTo(MFloatArray &radii, float scale=1.0f) const;
	
	/**
	 * @brief Appends the pearl radii to a double array.
	 * @param radii The double array to append to.
	 */
	void appendRadiiTo(MDoubleArray &radii) const;

	/**
	 * @brief Appends normalized parameters to a double array.
	 * @param params The double array to append to.
	 */
	void appendParamsTo(MDoubleArray &params) const;

	/**
	 * @brief Interpolates the chain to create a new chain with more points.
	 * @param step Number of interpolation steps.
	 * @param result Output chain to store the interpolated result.
	 */
	void interpolate(int step, pearlChain &result) const;

	/**
	 * @brief Gets the maximum radius in the chain.
	 * @return Constant reference to the maximum radius value.
	 */
	const float &maxRadius() const;
	
	/**
	 * @brief Gets the minimum radius in the chain.
	 * @return Constant reference to the minimum radius value.
	 */
	const float &minRadius() const;

	/**
	 * @brief Gets the pearls in this chain.
	 * @return Constant reference to the vector of pearls.
	 */
	const std::vector<pearl> &points() const;

private:
	std::vector<pearl> m_points;  /**< Collection of pearls in this chain. */

	float m_maxRadius;  /**< Maximum radius value in the chain. */
	float m_minRadius;  /**< Minimum radius value in the chain. */
};
#endif
