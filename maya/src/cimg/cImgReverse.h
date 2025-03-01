/**
 * @file cImgReverse.h
 * @brief Maya node for image value reversal.
 *
 * This file implements a Maya node for image reversal:
 * - Inverts pixel values
 * - Extends cImgProcess base functionality
 * - Simple but efficient operation
 * - Useful for creating negative images
 */

#ifndef _cImgReverse
#define _cImgReverse

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgReverse
 * @brief Maya node for image value reversal.
 *
 * cImgReverse extends cImgProcess with reversal capabilities:
 * - Inverts pixel values (255 - value)
 * - Simple but efficient operation
 * - Creates negative images
 * - Preserves alpha channel
 */
class cImgReverse : public cImgProcess
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgReverse();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgReverse();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgReverse instance
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
	 * @brief Processes the input image with value reversal.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output reversed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImg<unsigned char>& image,
						  CImg<unsigned char>& result);

private:
};

#endif
