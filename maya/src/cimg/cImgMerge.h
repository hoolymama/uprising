/**
 * @file cImgMerge.h
 * @brief Abstract base class for image merging operations in Maya.
 *
 * This file defines the base class for nodes that combine multiple images:
 * - Handles multiple input images through an array attribute
 * - Provides common interface for different merging strategies
 * - Supports inversion of merged results
 */

#ifndef _cImgMerge
#define _cImgMerge

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

#include "cImgData.h"

/**
 * @class cImgMerge
 * @brief Abstract base class for image merging nodes.
 *
 * cImgMerge provides functionality for combining multiple images:
 * - Handles array of input images
 * - Defines interface for different merging strategies
 * - Supports result inversion
 * - Base class for operations like min/max, multiply, etc.
 */
class cImgMerge : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgMerge();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgMerge();

	/**
	 * @brief Computes the merged result from input images.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	/**
	 * @brief Indicates this is an abstract class.
	 * @return true as this is an abstract base class
	 */
	virtual bool isAbstractClass() const { return true; }

	/**
	 * @brief Pure virtual method for implementing specific merge operations.
	 * @param data The data block containing the node's data
	 * @param images List of input images to merge
	 * @param result The output merged image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock &data, const CImgList<unsigned char> &images,
		CImg<unsigned char> &result);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new instance
	 */
	static void *creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

protected:
	/** @brief Array of input images to merge */
	static MObject aInput;
	/** @brief Controls whether to invert the merged result */
	static MObject aInvert;
	/** @brief Output merged image */
	static MObject aOutput;
};

#endif
