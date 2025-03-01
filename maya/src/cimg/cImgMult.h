/**
 * @file cImgMult.h
 * @brief Maya node for multiplying multiple images.
 *
 * This file implements a Maya node for image multiplication:
 * - Multiplies pixel values from multiple input images
 * - Extends cImgMerge base functionality
 * - Optional result inversion
 * - Useful for masking and compositing
 */

#ifndef _cImgMult
#define _cImgMult

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgMerge.h"

/**
 * @class cImgMult
 * @brief Maya node for multiplying multiple images.
 *
 * cImgMult extends cImgMerge with multiplication capabilities:
 * - Per-pixel multiplication of input images
 * - Handles multiple input images
 * - Optional result inversion
 * - Useful for masking and compositing
 */
class cImgMult : public cImgMerge
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgMult();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgMult();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgMult instance
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
	 * @brief Processes input images with multiplication.
	 * @param data The data block containing the node's data
	 * @param images List of input images to multiply
	 * @param result Output multiplied image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImgList<unsigned char>& images,
						  CImg<unsigned char>& result);

private:
};

#endif
