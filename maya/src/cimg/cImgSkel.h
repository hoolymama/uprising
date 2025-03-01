/**
 * @file cImgSkel.h
 * @brief Maya node for image skeletonization.
 *
 * This file implements a Maya node for skeletonization:
 * - Generates skeleton/medial axis from binary image
 * - Multiple threshold controls
 * - Optional curve generation
 * - Configurable processing parameters
 */

#ifndef _cImgSkel
#define _cImgSkel

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

/**
 * @class cImgSkel
 * @brief Maya node for image skeletonization.
 *
 * cImgSkel provides skeletonization functionality:
 * - Medial axis transform
 * - Multiple threshold controls
 * - Optional curve generation
 * - Configurable processing parameters
 */
class cImgSkel : public MPxNode
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgSkel();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgSkel();

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgSkel instance
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
	 * @brief Computes the skeletonized output.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

private:
	/** @brief Input image data */
	static MObject aInput;

	/** @brief Main threshold value */
	static MObject aThreshold;
	/** @brief Soft threshold value */
	static MObject aSoftThreshold;
	/** @brief Strict threshold value */
	static MObject aStrictThreshold;

	/** @brief Median filter size */
	static MObject aMedian;
	/** @brief Enable/disable inversion */
	static MObject aInvert;
	/** @brief Skeleton threshold value */
	static MObject aSkeletonThreshold;
	/** @brief Enable/disable curve creation */
	static MObject aCreateCurve;
	/** @brief Correction factor */
	static MObject aCorrection;
	/** @brief Discrete step size */
	static MObject aDiscreteStep;

	/** @brief Output skeleton data */
	static MObject aOutSkeleton;
	/** @brief Output binary image */
	static MObject aOutBinaryImage;

	/**
	 * @brief Processes the input image to generate skeleton.
	 * @param data The data block containing the node's data
	 * @param inimage Input binary image
	 * @param outSkeleton Output skeleton data
	 * @param outBinary Output binary image
	 * @return MStatus Success or failure status
	 */
	MStatus process(
		MDataBlock& data,
		const CImg<unsigned char>& inimage,
		CImg<float>& outSkeleton,
		CImg<unsigned char>& outBinary);
};

#endif
