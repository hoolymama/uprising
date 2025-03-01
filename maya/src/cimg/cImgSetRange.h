/**
 * @file cImgSetRange.h
 * @brief Maya node for remapping image color ranges.
 *
 * This file implements a Maya node for color range adjustment:
 * - Remaps color values to new ranges
 * - Extends cImgProcess base functionality
 * - Configurable min/max color values
 * - Preserves alpha channel
 */

#ifndef _cImgSetRange
#define _cImgSetRange

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgSetRange
 * @brief Maya node for remapping image color ranges.
 *
 * cImgSetRange extends cImgProcess with range remapping:
 * - Adjusts color values to new ranges
 * - Configurable minimum and maximum colors
 * - Linear interpolation between ranges
 * - Preserves alpha channel values
 */
class cImgSetRange : public cImgProcess
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgSetRange();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgSetRange();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgSetRange instance
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
	 * @brief Processes the input image with range remapping.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output remapped image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImg<unsigned char>& image,
							CImg<unsigned char>& result);

private:
	/** @brief Minimum output color values */
	static MObject aMinColor;
	/** @brief Maximum output color values */
	static MObject aMaxColor;
};

#endif
