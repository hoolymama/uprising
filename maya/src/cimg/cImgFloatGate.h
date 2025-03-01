/**
 * @file cImgFloatGate.h
 * @brief Maya node for floating-point image thresholding.
 *
 * This file implements a Maya node for floating-point image thresholding:
 * - Processes floating-point image data
 * - Applies minimum and maximum thresholds
 * - Converts float values to unsigned char output
 * - Useful for processing gradient and displacement maps
 */

#ifndef _cImgGateFloat
#define _cImgGateFloat

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

/**
 * @class cImgFloatGate
 * @brief Maya node for floating-point image thresholding.
 *
 * cImgFloatGate provides floating-point image thresholding:
 * - Processes CImg<float> input images
 * - Applies configurable min/max thresholds
 * - Outputs thresholded result as unsigned char
 * - Useful for processing gradient and displacement data
 */
class cImgFloatGate : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgFloatGate();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgFloatGate();

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgFloatGate instance
	 */
	static void* creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

	/**
	 * @brief Computes the thresholded output image.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	/**
	 * @brief Processes the input float image.
	 * @param data The data block containing the node's data
	 * @param image Input float image
	 * @param result Output thresholded image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(
		MDataBlock& data,
		const CImg<float>& image,
		CImg<unsigned char>& result);

	/** @brief Minimum threshold value */
	static MObject aMinGate;
	/** @brief Maximum threshold value */
	static MObject aMaxGate;
	/** @brief Input float image */
	static MObject aInput;
	/** @brief Output thresholded image */
	static MObject aOutput;

private:
};

#endif
