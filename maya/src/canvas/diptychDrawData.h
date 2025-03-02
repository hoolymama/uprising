#ifndef diptychDrawData_H
#define diptychDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MColor.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

/**
 * @file diptychDrawData.h
 * @brief Defines the diptychDrawData class for storing drawing data for diptych nodes.
 * 
 * This file contains the diptychDrawData class which stores the data needed for drawing
 * diptych nodes in Maya's Viewport 2.0. It includes colors, display flags, points, and
 * transformation data used by the diptychDrawOverride class during rendering.
 */

/**
 * @class diptychDrawData
 * @brief User data class for storing drawing data for diptych nodes.
 * 
 * The diptychDrawData class extends MUserData to store the data needed for drawing
 * diptych nodes in Maya's Viewport 2.0. It includes:
 * - Colors for different diptych elements
 * - Display flags for controlling visibility of components
 * - Points and vectors for geometric representation
 * - Scale values for sizing elements
 */
class diptychDrawData : public MUserData
{

public:
	/**
	 * @brief Default constructor.
	 */
	diptychDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~diptychDrawData();
 
	/** @brief Color for the board element. */
	MColor boardColor;
	
	/** @brief Color for the square element. */
	MColor squareColor;
	
	/** @brief Color for the pin element. */
	MColor pinColor;
	
	/** @brief Color for the probe elements. */
	MColor probeColor;
	
	/** @brief Flag indicating whether to mirror the display. */
	bool mirror;
	// float paintingLocatorSize;

	/** @brief Flag indicating whether to display the square matrix. */
	bool displaySquareMatrix;
	
	/** @brief Flag indicating whether to display the board matrix. */
	bool displayBoardMatrix;
	
	/** @brief Flag indicating whether to display the pin matrix. */
	bool displayPinMatrix;
	
	/** @brief Flag indicating whether to display the painting matrix. */
	bool displayPaintingMatrix;
	
	/** @brief Flag indicating whether to display the probes. */
	bool displayProbes;

	/** @brief Array of probe points for display. */
	MPointArray probePoints;
	
	/** @brief Size of probe points for display. */
	float probePointSize;

	/** @brief Up vector for orientation. */
	MVector up;
	
	/** @brief Normal vector for orientation. */
	MVector normal;
	
	/** @brief Origin point for positioning. */
	MPoint origin;

	/** @brief Center point of the square element. */
	MPoint squareCenter;
	
	/** @brief Mirrored center point of the square element. */
	MPoint squareCenterMirror;

	/** @brief X-axis scale factor for the square element. */
	double squareScaleX;
	
	/** @brief Y-axis scale factor for the square element. */
	double squareScaleY;

	/** @brief Center point of the board element. */
	MPoint boardCenter;
	
	/** @brief Mirrored center point of the board element. */
	MPoint boardCenterMirror;

	/** @brief X-axis scale factor for the board element. */
	double boardScaleX;
	
	/** @brief Y-axis scale factor for the board element. */
	double boardScaleY;
	
	/** @brief X-axis scale factor for the pin element. */
	double pinScaleX;
	
	/** @brief Y-axis scale factor for the pin element. */
	double pinScaleY;

	/** @brief X-axis point for the painting element. */
	MPoint paintingXAxis;
	
	/** @brief Y-axis point for the painting element. */
	MPoint paintingYAxis;
	
	/** @brief Z-axis point for the painting element. */
	MPoint paintingZAxis;
};

#endif

