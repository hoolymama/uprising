/**
 * @file cImgDetail.h
 * @brief Maya node for image detail enhancement and edge detection.
 *
 * This file implements a Maya node for detail and edge processing:
 * - Multiple edge detection methods (gradient, hysteresis)
 * - Detail magnitude calculation
 * - Configurable boundary handling
 * - Gradient blur control
 */

#ifndef _cImgDetail
#define _cImgDetail

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>

#include "cImgProcess.h"

/**
 * @class cImgDetail
 * @brief Maya node for image detail enhancement and edge detection.
 *
 * cImgDetail extends cImgProcess with detail enhancement capabilities:
 * - Multiple edge detection methods
 * - Hysteresis thresholding for edge detection
 * - Detail magnitude calculation
 * - Configurable gradient processing
 */
class cImgDetail : public cImgProcess
{
public:
	/**
	 * @brief Default constructor.
	 */
	cImgDetail();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgDetail();

	/**
	 * @brief Indicates this is not an abstract class.
	 * @return false as this class can be instantiated
	 */
	virtual bool isAbstractClass() const { return false; }

	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgDetail instance
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
	 * @brief Processes the input image for detail enhancement.
	 * @param data The data block containing the node's data
	 * @param image Input image to process
	 * @param result Output processed image
	 * @return MStatus Success or failure status
	 */
	virtual MStatus process(MDataBlock& data, const CImg<unsigned char>& image,
		CImg<unsigned char>& result);

	/** @brief Controls boundary expansion */
	static MObject aExpandBoundary;
	/** @brief Amount of blur to apply to gradient */
	static MObject aBlurGradient;
	/** @brief Low threshold for hysteresis */
	static MObject aLowThreshold;
	/** @brief High threshold for hysteresis */
	static MObject aHighThreshold;
	/** @brief Processing mode selection */
	static MObject aMode;
	/** @brief Median filter size */
	static MObject aMedian;

private:
	/**
	 * @brief Performs edge detection using hysteresis thresholding.
	 * @param img Input image
	 * @param gradientBlur Amount of gradient blur
	 * @param lowThreshold Low threshold value
	 * @param highThreshold High threshold value
	 * @param result Output edge image
	 */
	void findEdgesHysteresis(
		const CImg<unsigned char>& img,
		float gradientBlur,
		float lowThreshold,
		float highThreshold,
		CImg<unsigned char>& result);

	/**
	 * @brief Performs edge detection using gradient magnitude.
	 * @param img Input image
	 * @param gradientBlur Amount of gradient blur
	 * @param result Output edge image
	 */
	void findEdgesGradient(
		const CImg<unsigned char>& img,
		float gradientBlur,
		CImg<unsigned char>& result);

	/**
	 * @brief Calculates detail magnitude in the image.
	 * @param img Input image
	 * @param gradientBlur Amount of gradient blur
	 * @param lowThreshold Low threshold value
	 * @param highThreshold High threshold value
	 * @param result Output detail magnitude image
	 */
	void detailMagnitude(
		const CImg<unsigned char>& img,
		float gradientBlur,
		float lowThreshold,
		float highThreshold,
		CImg<unsigned char>& result);

	/** @brief Processing modes */
	enum Mode { 
		kGradient,      ///< Gradient-based edge detection
		kHysteresis,    ///< Hysteresis thresholding
		kDetailMagnitude ///< Detail magnitude calculation
	};
};

#endif
