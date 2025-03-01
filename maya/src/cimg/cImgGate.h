/**
 * @file cImgGate.h
 * @brief Maya node for image thresholding and morphological operations.
 *
 * This file implements a Maya node that provides advanced image gating functionality,
 * combining thresholding with morphological operations and blur. The node supports:
 * - Dual-threshold gating (min and max values)
 * - Morphological operations (dilation/erosion)
 * - Optional Gaussian blur with threshold re-application
 * - Real-time preview in Maya viewport
 */

#ifndef _cImgGate
#define _cImgGate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgGate
 * @brief Maya node for image thresholding and morphological operations.
 *
 * The cImgGate node provides advanced image processing capabilities:
 * - Dual-threshold gating to isolate pixel values within a specific range
 * - Morphological operations (dilation/erosion) for shape manipulation
 * - Optional Gaussian blur with threshold re-application
 * - Inherits from cImgProcess for common image processing functionality
 */
class cImgGate : public cImgProcess
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgGate();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgGate();

	/**
	 * @brief Indicates if this is an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new cImgGate instance
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
	 * @brief Processes the input image according to gate parameters.
	 * @param data The data block containing the node's data
	 * @param image The input image to process
	 * @param result The output image after processing
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(
		MDataBlock& data,
		const CImg<unsigned char>& image,
		CImg<unsigned char>& result);

	/** @brief Amount of dilation/erosion to apply (positive for dilation, negative for erosion) */
	static MObject aDilate;

	/** @brief Amount of Gaussian blur to apply */
	static MObject aBlur;

	/** @brief Minimum threshold value for gating (0-255) */
	static MObject aMinGate;

	/** @brief Maximum threshold value for gating (0-255) */
	static MObject aMaxGate;

private:
};

#endif
