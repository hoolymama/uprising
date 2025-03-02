/**
 * @file brushDrawData.h
 * @brief Defines the user data for brush drawing.
 *
 * This file implements the user data for brush drawing:
 * - Stores geometry data for brush visualization
 * - Provides points, normals, colors, and indices for drawing
 * - Supports cone representation for brush direction
 * - Used by brushDrawOverride for Viewport 2.0 drawing
 */

#ifndef brushDrawData_H
#define brushDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>

#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MColorArray.h>
#include <maya/MUintArray.h>

/**
 * @class brushDrawData
 * @brief User data for brush drawing in Viewport 2.0.
 *
 * brushDrawData stores geometry data for brush visualization:
 * - Extends MUserData for Viewport 2.0 drawing
 * - Stores points, normals, colors, and indices for drawing
 * - Provides cone representation for brush direction
 * - Used by brushDrawOverride for drawing brushes
 */
class brushDrawData : public MUserData
{
public:
	/**
	 * @brief Default constructor.
	 */
	brushDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~brushDrawData();
 
	/** @brief Vertex positions for drawing */
	MFloatPointArray points;
	/** @brief Normal vectors for drawing */
	MFloatVectorArray normals;
	/** @brief Colors for drawing */
	MColorArray colors;
	/** @brief Vertex indices for drawing */
	MUintArray indices;

	/** @brief Base position of the direction cone */
	MPoint coneBase;
	/** @brief Scale of the direction cone */
	double coneScale;
};

#endif

