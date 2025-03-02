/**
 * @file paintingDrawData.h
 * @brief Defines the PaintingDrawData class for storing drawing data for painting nodes.
 * 
 * This file contains the PaintingDrawData class which stores the data needed for drawing
 * painting nodes in Maya's Viewport 2.0. It includes display flags, colors, and geometric
 * data used by the paintingDrawOverride class during rendering.
 */

#ifndef PaintingDrawData_H
#define PaintingDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MStringArray.h>
#include "paintingGeom.h"
#include "enums.h"

/**
 * @class PaintingDrawData
 * @brief User data class for storing drawing data for painting nodes.
 * 
 * The PaintingDrawData class extends MUserData to store the data needed for drawing
 * painting nodes in Maya's Viewport 2.0. It includes:
 * - Display flags for controlling what information is shown
 * - Visual properties like colors, sizes, and thicknesses
 * - Geometric data for positioning and orientation
 * - Reference to the painting geometry being visualized
 */
class PaintingDrawData : public MUserData
{
public:
	/**
	 * @brief Default constructor.
	 */
	PaintingDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~PaintingDrawData();

	bool displayClusterPath;     /**< Flag indicating whether to display cluster paths. */
	bool displayPivots;          /**< Flag indicating whether to display pivots. */
	bool displayIds;             /**< Flag indicating whether to display IDs. */
	bool displaySegmentIds;      /**< Flag indicating whether to display segment IDs. */
	bool displayParentIds;       /**< Flag indicating whether to display parent IDs. */
	bool displayLayerIds;        /**< Flag indicating whether to display layer IDs. */
	bool displayBrushIds;        /**< Flag indicating whether to display brush IDs. */
	bool displayPaintIds;        /**< Flag indicating whether to display paint IDs. */
	bool displayRepeatIds;       /**< Flag indicating whether to display repeat IDs. */
	bool displayBrushModelIds;   /**< Flag indicating whether to display brush model IDs. */
	bool displayApproachTargets; /**< Flag indicating whether to display approach targets. */

	bool displayContactWidth;    /**< Flag indicating whether to display contact width. */

	float pointSize;             /**< Size of points for display. */
	float lineLength;            /**< Length of lines for display. */
	float lineThickness;         /**< Thickness of lines for display. */
	float stackGap;              /**< Gap between stacked elements. */
	float drawParam;             /**< Parameter for drawing control. */
	MColor wireColor;            /**< Color for wireframe display. */
	MColor clusterPathColor;     /**< Color for cluster path display. */
	
	
	float arrowheadSize;         /**< Size of arrowheads for display. */

	short displayTargets;        /**< Mode for displaying targets. */
	short displayTargetColors;   /**< Mode for displaying target colors. */

	MFloatVector drawingNormal;  /**< Normal vector for drawing orientation. */

	MFloatVector idDisplayOffset; /**< Offset for ID display positioning. */

	MFloatVectorArray stackOffsets; /**< Array of offsets for stacked elements. */

	paintingGeom *geom;          /**< Pointer to the painting geometry being visualized. */
};

#endif
 