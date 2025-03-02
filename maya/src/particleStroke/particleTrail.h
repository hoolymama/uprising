/**
 * @file particleTrail.h
 * @brief Defines the particleTrail class for storing particle trail data.
 * 
 * This file contains the particleTrail class which provides functionality for storing
 * and managing particle trail data, including positions and colors of particles along
 * a trail path.
 */

#ifndef _particleTrail_H
#define _particleTrail_H

#include <maya/MFloatPointArray.h>
#include <maya/MColorArray.h>

/**
 * @class particleTrail
 * @brief Stores position and color data for a particle trail.
 * 
 * The particleTrail class encapsulates the position and color data for a particle trail.
 * It provides methods for appending new positions and colors to the trail and for
 * accessing the stored data.
 */
class particleTrail {

public:
	/**
	 * @brief Default constructor.
	 * Creates an empty particle trail.
	 */
	particleTrail()
		: m_positions(), 
        m_colors() 
    {}

    /**
     * @brief Destructor.
     */
    ~particleTrail(){};

	/**
	 * @brief Appends a position and color to the trail.
	 * @param p The position to append.
	 * @param c The color to append.
	 */
	void append(const MFloatPoint &p, const MColor &c)
	{
		m_positions.append(p);
		m_colors.append(c);
	}

	/**
	 * @brief Gets the positions in this trail.
	 * @return Constant reference to the array of positions.
	 */
	const MFloatPointArray & positions() const
	{
		return m_positions;
	}

	/**
	 * @brief Gets the colors in this trail.
	 * @return Constant reference to the array of colors.
	 */
	const MColorArray & colors() const 
	{
		return m_colors;
	}
	

private:
	MFloatPointArray m_positions;  /**< Array of positions along the trail. */
	MColorArray m_colors;          /**< Array of colors along the trail. */
};
#endif
