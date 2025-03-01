/**
 * @file cImgFileSplit.h
 * @brief Maya node for splitting images based on color analysis.
 *
 * This file implements a Maya node for image splitting operations:
 * - Splits images based on color analysis and sorting
 * - Supports multiple color sorting methods
 * - Handles palette-based image processing
 * - Provides color transformation options
 */

#ifndef _cImgFileSplit
#define _cImgFileSplit

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>
#include <maya/MColor.h>
#include <math.h> 

#include "cImgData.h"

/**
 * @class cImgFileSplit
 * @brief Maya node for color-based image splitting.
 *
 * cImgFileSplit provides image splitting functionality:
 * - Color analysis and sorting
 * - Palette-based processing
 * - Multiple output generation
 * - Color space transformations
 */
class cImgFileSplit : public MPxNode
{
public:
	/**
	 * @brief Method enumeration for color sorting.
	 */
	enum Method
	{
		kNone,
		kHSPAscending,
		kHSPDescending
	};

	/**
	 * @brief Default constructor.
	 */
	cImgFileSplit();

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~cImgFileSplit();

	/**
	 * @brief Computes the split image outputs.
	 * @param plug The plug to compute
	 * @param data The data block containing the node's data
	 * @return MStatus Success or failure status
	 */
	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	/**
	 * @brief Calculates color palette from input image.
	 * @param data Maya data block
	 * @param image Source image
	 * @param palette Output color palette
	 */
	void calculate_pallete(	
		MDataBlock&data,
		const CImg<unsigned char> &image,
		MColorArray &palette
		) const;

	// virtual MStatus setDependentsDirty(
    //     const MPlug &plugBeingDirtied,
    //     MPlugArray &affectedPlugs );


	/**
	 * @brief Creates a new instance of the node.
	 * @return Pointer to new cImgFileSplit instance
	 */
	static  void		*creator();

	/**
	 * @brief Initializes the node attributes.
	 * @return MStatus Success or failure status
	 */
	static MStatus initialize();
	static MObject aImageFilename;
	// static MObject aResize;
	// static MObject aResizeResolution;
	static MObject aMaxOutputs;

	static MObject aInputPalette;
	
	static MObject aApplyCrop;
	static MObject aCropCorner;
	static MObject aCropResolution;
	
	static MObject aXResolution;
	static MObject aYResolution;

	static MObject aSortMethod;

	static MObject aOutputImage;
	static MObject aOutputColor;
	static MObject aOutput;
	static MObject aOutputIndex;
	static MObject aIndexScale;
	
    static MObject aHueRotate;
    static MObject aSaturationRangeRemap;
    static MObject aValueRangeRemap;

	static MObject aOutputCount;
	static MObject aOutputCropFactor;
	static MObject aOutputOffsetFactorX;
	static MObject aOutputOffsetFactorY;
	
	static MTypeId	id;

	/**
	 * @brief Compares two colors based on HSP value.
	 * @param c1 First color
	 * @param c2 Second color
	 * @return true if c1's HSP value is less than c2's
	 */
	static bool compareHSP(const MColor &c1, const MColor &c2);

	/**
	 * @brief Calculates HSP (Highly Saturated Pixels) value for a color.
	 * @param color Input color
	 * @return HSP value
	 */
	static float calculateHSP(const MColor &color);

private:

};


inline float cImgFileSplit::calculateHSP(const MColor &color)
{
	// return  sqrt( 0.299*color.r*color.r + 0.587*color.g*color.g + 0.114*color.b*color.b );
	// Don't need sqrt for a sort
	return  0.299*color.r*color.r + 0.587*color.g*color.g + 0.114*color.b*color.b ;
	
}


inline bool cImgFileSplit::compareHSP(const MColor &c1, const MColor &c2)
{
	return cImgFileSplit::calculateHSP(c1) < cImgFileSplit::calculateHSP(c2);
}

#endif
