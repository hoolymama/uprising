/**
 * @file pearlNodeDrawData.h
 * @brief Defines the pearlNodeDrawData class for storing drawing data for pearl nodes.
 * 
 * This file contains the pearlNodeDrawData class which stores the data needed for drawing
 * pearl nodes in Maya's Viewport 2.0. It includes points, colors, radii, and display flags
 * used by the pearlNodeDrawOverride class during rendering.
 */

#ifndef pearlNodeDrawData_H
#define pearlNodeDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MColorArray.h>
#include <maya/MIntArray.h>

/**
 * @class pearlNodeDrawData
 * @brief User data class for storing drawing data for pearl nodes.
 * 
 * The pearlNodeDrawData class extends MUserData to store the data needed for drawing
 * pearl nodes in Maya's Viewport 2.0. It includes:
 * - Points and colors for the pearls
 * - Radii for the pearls
 * - Count information for organizing the pearls
 * - Display flags for controlling what elements are drawn
 */
class pearlNodeDrawData : public MUserData
{
public:
	/**
	 * @brief Default constructor.
	 */
	pearlNodeDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~pearlNodeDrawData();

	float pointSize;      /**< Size of points for display. */
	MColorArray colors;   /**< Colors for the pearls. */
	MPointArray points;   /**< Positions of the pearls. */
	MDoubleArray radius;  /**< Radii of the pearls. */
	MIntArray counts;     /**< Count information for organizing the pearls. */
	bool drawEdges;       /**< Flag indicating whether to draw edges. */
	bool drawPoints;      /**< Flag indicating whether to draw points. */
	bool drawCircles;     /**< Flag indicating whether to draw circles. */
};

#endif
