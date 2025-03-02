/**
 * @file paletteOrder.h
 * @brief Defines the paletteOrder class for ordering colors in a palette.
 * 
 * This file contains the paletteOrder class which provides functionality for
 * ordering colors in a palette based on various methods, particularly using
 * the HSP (Hue, Saturation, Perceived brightness) color model.
 */

#ifndef paletteOrder_H
#define paletteOrder_H

#include <math.h> 
#include <maya/MPxNode.h>
#include <maya/MColor.h>

/**
 * @class paletteOrder
 * @brief A node for ordering colors in a palette.
 * 
 * The paletteOrder class extends MPxNode to create a specialized node that orders
 * colors in a palette based on various methods, particularly using the HSP
 * (Hue, Saturation, Perceived brightness) color model. It provides functionality for:
 * - Ordering colors by perceived brightness
 * - Supporting ascending and descending orders
 * - Applying offsets to the ordering
 */
class paletteOrder: public MPxNode {

public:
	/**
	 * @enum Method
	 * @brief Enumeration for color ordering methods.
	 * 
	 * Defines different methods for ordering colors in a palette.
	 */
	enum Method
	{
		kNone,            /**< No ordering. */
		kHSPAscending,    /**< Order by HSP value in ascending order. */
		kHSPDescending    /**< Order by HSP value in descending order. */
	};

	/**
	 * @brief Post-constructor setup for the node.
	 */
	virtual	void		postConstructor();
	
	/**
	 * @brief Default constructor.
	 */
	paletteOrder ();
	
	/**
	 * @brief Virtual destructor.
	 */
	virtual ~paletteOrder ();

	/**
	 * @brief Compute method for calculating the node's outputs.
	 * @param plug The plug to compute.
	 * @param data The data block containing the node's data.
	 * @return MStatus indicating success or failure.
	 */
	virtual MStatus compute (const MPlug &plug, MDataBlock &data);
	
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @return Pointer to a new paletteOrder object.
	 */
	static void *creator ();
	
	/**
	 * @brief Initialize attributes and connections for the node.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize ();
	
	/** @brief Unique type ID for this node type. */
	static MTypeId id;

	/**
	 * @brief Calculates the HSP (Hue, Saturation, Perceived brightness) value of a color.
	 * @param color The color to calculate the HSP value for.
	 * @return The HSP value of the color.
	 */
    static float calculateHSP(const MColor &color);

private:
	/** @brief Input colors attribute. */
	static MObject aInput;
	
	/** @brief Ordering method attribute. */
	static MObject aMethod;
	
	/** @brief Offset attribute. */
	static MObject aOffset;
	
	/** @brief Output colors attribute. */
	static MObject aOutput;
};

/**
 * @brief Calculates the HSP (Hue, Saturation, Perceived brightness) value of a color.
 * 
 * This inline function implements the HSP color model which takes into account the
 * perceived brightness of different color components. The formula used is:
 * HSP = 0.299*R^2 + 0.587*G^2 + 0.114*B^2
 * 
 * @param color The color to calculate the HSP value for.
 * @return The HSP value of the color.
 */
inline float paletteOrder::calculateHSP(const MColor &color)
{
	return 0.299*color.r*color.r + 0.587*color.g*color.g + 0.114*color.b*color.b;
}

#endif