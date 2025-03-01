/**
 * @file cImgMinMax.h
 * @brief Maya node for minimum/maximum operations on multiple images.
 *
 * This file implements a Maya node for min/max operations:
 * - Combines multiple input images using min/max
 * - Supports per-pixel minimum and maximum operations
 * - Extends cImgMerge base functionality
 * - Optional result inversion
 */

#ifndef _cImgMinMax
#define _cImgMinMax

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"

/**
 * @class cImgMinMax
 * @brief Maya node for minimum/maximum operations on multiple images.
 *
 * cImgMinMax extends cImgMerge with min/max capabilities:
 * - Per-pixel minimum operation mode
 * - Per-pixel maximum operation mode
 * - Processes multiple input images
 * - Optional result inversion
 */
class cImgMinMax : public cImgMerge
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgMinMax();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgMinMax();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgMinMax instance
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
	 * @brief Processes input images with min/max operations.
	 * @param data The data block containing the node's data
	 * @param images List of input images to process
	 * @param result Output processed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImgList<unsigned char>& images,
		CImg<unsigned char>& result);

private:
	/** @brief Operation modes */
	enum Operation { 
		kMinimum,  ///< Take minimum value
		kMaximum   ///< Take maximum value
	};

	/** @brief Operation mode selection */
	static MObject aOperation;
};

#endif
