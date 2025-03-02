/**
 * @file imagePointsDrawData.h
 * @brief Defines the imagePointsDrawData class for storing drawing data for image points.
 * 
 * This file contains the imagePointsDrawData class which extends MUserData to store
 * the data needed for drawing image points in Maya's Viewport 2.0. It includes points,
 * colors, radii, and display flags used by the imagePointsDrawOverride class during rendering.
 */


#ifndef imagePointsDrawData_H
#define imagePointsDrawData_H



#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
 
 
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>

/**
 * @class imagePointsDrawData
 * @brief User data class for storing drawing data for image points.
 * 
 * The imagePointsDrawData class extends MUserData to store the data needed for drawing
 * image points in Maya's Viewport 2.0. It includes:
 * - Points and colors for the points
 * - Line lists for connections between points
 * - Radii for the points
 * - Display flags for controlling what elements are drawn
 */
class imagePointsDrawData : public MUserData 
{


public:
    /**
     * @brief Default constructor.
     */
	imagePointsDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~imagePointsDrawData();

	MColor color;           /**< Color for the points. */
	MColor gradientColor;   /**< Color for the gradient lines. */
	
	MPointArray points;     /**< Positions of the points. */
	MPointArray lineList;   /**< Line segments for connections between points. */
	MDoubleArray radii;     /**< Radii of the points. */
	
	int pointSize;          /**< Size of points for display. */
	int lineThickness;      /**< Thickness of lines for display. */
	bool displayPoints;     /**< Flag indicating whether to display points. */
	bool displayOrder;      /**< Flag indicating whether to display order. */
	bool displayCircles;    /**< Flag indicating whether to display circles. */
 	float circleDisplaySize; /**< Size multiplier for circle display. */
	bool displayGradient;   /**< Flag indicating whether to display gradients. */
	float gradientLineLength; /**< Length of gradient lines. */
};

#endif
