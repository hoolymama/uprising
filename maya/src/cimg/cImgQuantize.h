/**
 * @file cImgQuantize.h
 * @brief Maya node for color quantization operations.
 *
 * This file implements a Maya node for color reduction:
 * - Multiple quantization methods (uniform, median cut, k-means)
 * - Optional dithering support
 * - Grayscale conversion option
 * - Configurable color levels
 */

#ifndef _cImgQuantize
#define _cImgQuantize

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>
#include <maya/MColor.h>
#include <math.h>

#include "cImgData.h"

/**
 * @class cImgQuantize
 * @brief Maya node for color quantization operations.
 *
 * cImgQuantize provides color reduction functionality:
 * - Multiple quantization algorithms
 * - Dithering support for better visual quality
 * - Grayscale conversion option
 * - Configurable output color levels
 */
class cImgQuantize : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgQuantize();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgQuantize();

	/**
	 * @brief Computes the quantized output image.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgQuantize instance
	 */
	static void *creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();

	/** @brief Input image data */
	static MObject aInput;
	/** @brief Output quantized image */
	static MObject aOutput;
	/** @brief Number of output color levels */
	static MObject aLevels; 	

	/** @brief Enable grayscale conversion */
	static MObject aGreyscale;

	/** @brief Quantization method selection */
	static MObject aQuantize;
	/** @brief Dithering method selection */
	static MObject aDither;

	/** @brief Unique Maya type ID for the node */
	static MTypeId id;

	/**
	 * @brief Processes the input image with quantization.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output quantized image
	 * @return MStatus Success or failure status
	 */
	MStatus process(MDataBlock &data, const CImg<unsigned char> &image,
					CImg<unsigned char> &result);

private:
	/**
	 * @brief Applies uniform quantization.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output quantized image
	 * @return MStatus Success or failure status
	 */
	MStatus quantizeUniform(MDataBlock &data, const CImg<unsigned char> &image,
							CImg<unsigned char> &result);

	/**
	 * @brief Applies k-means quantization.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output quantized image
	 * @return MStatus Success or failure status
	 */
	MStatus quantizeKMeans(MDataBlock &data, const CImg<unsigned char> &image,
						   CImg<unsigned char> &result);

	/** @brief Dithering methods */
	enum Dither
	{
		kNone,
		kFloydSteinberg
	};
	/** @brief Quantization methods */
	enum Quantize
	{
		kUniform,
		kMedianCut,
		kKMeans
	};
};

#endif