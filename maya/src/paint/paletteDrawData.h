/**
 * @file paletteDrawData.h
 * @brief Defines the PaletteDrawData class for storing drawing data for palette nodes.
 * 
 * This file contains the PaletteDrawData class which stores the data needed for drawing
 * palette nodes in Maya's Viewport 2.0. It includes palette data, display options,
 * and positioning information used by the PaletteDrawOverride class during rendering.
 */

#ifndef PaletteDrawData_H
#define PaletteDrawData_H

#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawRegistry.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MMatrix.h>
#include <maya/MColor.h>
#include <maya/MStringArray.h>
#include "paintingGeom.h"
#include "paint.h"

/**
 * @class PaletteDrawData
 * @brief User data class for storing drawing data for palette nodes.
 * 
 * The PaletteDrawData class extends MUserData to store the data needed for drawing
 * palette nodes in Maya's Viewport 2.0. It includes:
 * - Palette data (map of Paint objects)
 * - Position and size information
 * - Display flags for controlling what information is shown
 */
class PaletteDrawData : public MUserData
{
public:
	/**
	 * @brief Default constructor.
	 */
	PaletteDrawData();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~PaletteDrawData();

	/** @brief Map of Paint objects indexed by ID. */
	std::map<int, Paint> palette;
	
	/** @brief X position of the palette in the viewport. */
	float xPosition;
	
	/** @brief Y position of the palette in the viewport. */
	float yPosition;
	
	/** @brief Width of the palette in the viewport. */
	float width;
	
	/** @brief Height of the palette in the viewport. */
	float height;
	
	/** @brief Flag indicating whether to display paint IDs. */
	bool displayId;
	
	/** @brief Flag indicating whether to display paint names. */
	bool displayName;
	
	/** @brief Flag indicating whether to display RGB values. */
	bool displayRGB;
};

#endif
 