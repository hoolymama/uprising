/**
 * @file imageMeasureCmd.h
 * @brief Maya command for image measurement operations.
 *
 * This file implements a Maya command for image analysis:
 * - Measures image properties along specified axes
 * - Supports slicing operations
 * - Configurable thresholding
 * - Optional result scaling
 */

#ifndef	__imageMeasureCmd_H__
#define	__imageMeasureCmd_H__

#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MPxCommand.h>

#include "cImgData.h"

/** @brief Flag for specifying measurement axis */
#define kAxisFlag "-a"
#define kAxisFlagL "-axis"

/** @brief Flag for specifying slice operation */
#define kSliceFlag "-s"
#define kSliceFlagL "-slice"

/** @brief Flag for specifying threshold value */
#define kThresholdFlag "-t"
#define kThresholdFlagL "-threshold"

/** @brief Flag for inverting measurement */
#define kInvertFlag "-i"
#define kInvertFlagL "-invert"


// #define kPercentageFlag "-p"
// #define kPercentageFlagL "-percentage"


/** @brief Flag for scaling results */
#define kScaleFlag "-sc"
#define kScaleFlagL "-scale"

/** @brief Flag for measuring extents */
#define kExtentsFlag "-e"
#define kExtentsFlagL "-extents"

/** @brief Flag for specifying input filename */
#define kFilenameFlag "-f"
#define kFilenameFlagL "-filename"


/**
 * @class imageMeasureCmd
 * @brief Maya command for image measurement operations.
 *
 * imageMeasureCmd provides image analysis functionality:
 * - Measures properties along specified axes
 * - Supports slicing for detailed analysis
 * - Configurable thresholding
 * - Optional result scaling and inversion
 */
class imageMeasureCmd : public MPxCommand
{
public:
	/**
	 * @brief Default constructor.
	 */
	imageMeasureCmd() {}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~imageMeasureCmd() {}

	/**
	 * @brief Executes the command.
	 * @param args Command arguments
	 * @return MStatus Success or failure status
	 */
	MStatus doIt( const MArgList &args );

	/**
	 * @brief Creates a new instance of the command.
	 * @return Pointer to new imageMeasureCmd instance
	 */
	static void *creator();

	/**
	 * @brief Creates command syntax.
	 * @return MSyntax object defining command syntax
	 */
	static MSyntax      newSyntax();

private:

};

#endif

