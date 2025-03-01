/**
 * @file cImgDilate.h
 * @brief Maya node for morphological dilation operations.
 *
 * This file implements a Maya node for morphological operations:
 * - Configurable dilation amount
 * - Optional blur and threshold controls
 * - Median filtering support
 * - Bitwise operation modes
 */

#ifndef _cImgDilate
#define _cImgDilate

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgDilate
 * @brief Maya node for morphological dilation operations.
 *
 * cImgDilate extends cImgProcess with dilation capabilities:
 * - Configurable dilation radius
 * - Pre/post blur operations
 * - Threshold control
 * - Multiple bitwise operation modes
 */
class cImgDilate : public cImgProcess
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgDilate();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgDilate();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgDilate instance
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
	 * @brief Processes the input image with dilation.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output processed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImg<unsigned char>& image,
							CImg<unsigned char>& result);

	/** @brief Amount of dilation to apply */
	static MObject aDilateAmount;
	/** @brief Amount of blur to apply */
	static MObject aBlur;
	/** @brief Threshold value for operations */
	static MObject aThreshold;
	/** @brief Median filter size */
	static MObject aMedian;
	/** @brief Bitwise operation mode */
	static MObject aBitwise;

private:
	/** @brief Bitwise operation modes */
	enum Bitwise { 
		kNone,  ///< No bitwise operation
		kOR,    ///< Bitwise OR
		kAND,   ///< Bitwise AND
		kXOR    ///< Bitwise XOR
	};
};

#endif
