/**
 * @file cImgDistance.h
 * @brief Maya node for computing distance transforms.
 *
 * This file implements a Maya node for distance transform computation:
 * - Generates distance field from binary image
 * - Optional blur for smoothing
 * - Outputs floating-point distance values
 * - Useful for creating distance fields and masks
 */

#ifndef _cImgDistance
#define _cImgDistance

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

/**
 * @class cImgDistance
 * @brief Maya node for computing distance transforms.
 *
 * cImgDistance provides distance transform functionality:
 * - Converts binary image to distance field
 * - Configurable blur for smoothing
 * - Floating-point output precision
 * - Useful for creating masks and fields
 */
class cImgDistance : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgDistance();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgDistance();

	/**
	 * @brief Computes the distance transform.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	/**
	 * @brief Processes the input image to generate distance field.
	 * @param data The data block containing the node's data
	 * @param image Input binary image
	 * @param result Output distance field
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(
		MDataBlock& data,
		const CImg<unsigned char>& image,
		CImg<float>& result);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgDistance instance
	 */
	static void* creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

protected:
	/** @brief Input binary image */
	static MObject aInput;
	/** @brief Amount of blur to apply */
	static MObject aBlur;
	/** @brief Output distance field */
	static MObject aOutput;
};

#endif
