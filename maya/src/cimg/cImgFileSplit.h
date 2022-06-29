
#ifndef _cImgFileSplit
#define _cImgFileSplit

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>
#include <maya/MColor.h>
#include <math.h> 

#include "cImgData.h"

class cImgFileSplit : public MPxNode
{
public:

	enum Method
	{
		kNone,
		kHSPAscending,
		kHSPDescending
	};

	cImgFileSplit();

	virtual				~cImgFileSplit();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	void calculate_pallete(	
		MDataBlock&data,
		const CImg<unsigned char> &image,
		MColorArray &palette
		) const;

	// virtual MStatus setDependentsDirty(
    //     const MPlug &plugBeingDirtied,
    //     MPlugArray &affectedPlugs );


	static  void		*creator();

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

	static bool compareHSP(const MColor &c1, const MColor &c2);

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
