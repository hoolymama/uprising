/**
 * @file cImgGradient.h
 * @brief Maya node for computing image gradients.
 *
 * This file implements a Maya node that computes image gradients using the CImg library.
 * The node provides:
 * - Gradient computation in X and Y directions
 * - Configurable Gaussian pre-blur for noise reduction
 * - Output as floating-point vectors for accurate gradient representation
 * - Real-time preview in Maya viewport
 */

#ifndef _cImgGradient
#define _cImgGradient

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>


#include "cImgData.h"

/**
 * @class cImgGradient
 * @brief Maya node for computing image gradients.
 *
 * The cImgGradient node computes image gradients using the CImg library:
 * - Computes gradients in X and Y directions
 * - Optional Gaussian pre-blur for noise reduction
 * - Outputs floating-point vectors for accurate gradient representation
 * - Supports real-time preview in Maya viewport
 * - Useful for edge detection and feature extraction
 */
class cImgGradient : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgGradient();

	/**
	 * @brief Virtual destructor.
	 */
	virtual				~cImgGradient();

	/**
	 * @brief Computes the output gradient based on input parameters.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Processes the input image to compute gradients.
	 * @param data The data block containing the node's data
	 * @param image The input image to process
	 * @param result The output gradient image (floating-point)
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(
		MDataBlock &data, 
		const CImg<unsigned char> &image,
		CImg<float> &result);

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to the new cImgGradient instance
	 */
	static  void		*creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();


	/** @brief Unique Maya type ID for the node */
	static MTypeId	id;


protected:


	/** @brief Input image data */
	static MObject aInput;
	/** @brief Amount of Gaussian blur to apply before gradient computation */
	static MObject aBlur;
	/** @brief Output gradient data (floating-point vectors) */
	static MObject aOutput;

};

#endif
