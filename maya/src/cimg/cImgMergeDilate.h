/**
 * @file cImgMergeDilate.h
 * @brief Maya node for merging and dilating multiple images.
 *
 * This file implements a Maya node for combined merge and dilate operations:
 * - Merges multiple input images
 * - Applies morphological dilation
 * - Supports blur and threshold operations
 * - Provides multiple operation modes
 */

#ifndef _cImgMergeDilate
#define _cImgMergeDilate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"

/**
 * @class cImgMergeDilate
 * @brief Maya node for merging and dilating multiple images.
 *
 * cImgMergeDilate extends cImgMerge with dilation capabilities:
 * - Merges multiple input images
 * - Applies configurable dilation
 * - Supports blur and threshold operations
 * - Provides minimum/maximum operation modes
 */
class cImgMergeDilate : public cImgMerge
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgMergeDilate();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgMergeDilate();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgMergeDilate instance
	 */
	static void *creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

	/**
	 * @brief Processes the input images with merge and dilate operations.
	 * @param data The data block containing the node's data
	 * @param images List of input images to process
	 * @param result Output processed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
	                        CImg<unsigned char> &result);

private:
	/** @brief Amount of dilation to apply */
	static MObject aDilateAmount;
	/** @brief Amount of blur to apply */
	static MObject aBlur;
	/** @brief Threshold value for operations */
	static MObject aThreshold;
	/** @brief Median filter size */
	static MObject aMedian;
	/** @brief Operation mode selection */
	static MObject aOperation;

	/** @brief Operation modes for merging */
	enum Operation { kMinimum, kMaximum};


};

#endif
