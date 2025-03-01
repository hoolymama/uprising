/**
 * @file cImgConstant.h
 * @brief Implements a Maya node for generating constant color images.
 *
 * This file defines the cImgConstant class, which creates images filled with a single color.
 * The node allows users to specify:
 * - The color to fill the image with (RGB values)
 * - The resolution of the output image (width x height)
 * The node is useful for creating solid color backgrounds, test patterns, or as a base
 * for more complex image processing networks.
 */

#ifndef CIMGCONSTANT_H
#define CIMGCONSTANT_H

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

/**
 * @class cImgConstant
 * @brief A Maya node that generates constant color images.
 *
 * The cImgConstant node creates images filled with a single user-specified color.
 * Features include:
 * - Configurable RGB color values
 * - Adjustable image resolution (width and height)
 * - Real-time preview in Maya viewport
 * - Automatic clamping of color values to valid range [0,1]
 * - Resolution limits for performance (1-8192 pixels)
 */
class cImgConstant : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgConstant();

	/**
	 * @brief Virtual destructor.
	 */
	virtual				~cImgConstant();

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new cImgConstant instance.
	 */
	static  void		*creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus indicating success or failure.
	 */
	static MStatus initialize();

	/**
	 * @brief Computes the output image based on input parameters.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus indicating success or failure
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/** @brief Unique Maya type ID for the node */
	static MTypeId	id;

	/** @brief Input attribute for the color (RGB values) */
	static MObject aColor;

	/** @brief Input attribute for image resolution (width x height) */
	static MObject aResolution;

	/** @brief Output attribute for the generated image */
	static MObject aOutput;

private:

};

#endif
