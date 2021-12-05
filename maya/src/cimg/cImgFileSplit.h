
#ifndef _cImgFileSplit
#define _cImgFileSplit

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgFileSplit : public MPxNode
{
public:

	cImgFileSplit();

	virtual				~cImgFileSplit();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	void calculate_pallete(	
		MDataBlock&data,
		const CImg<unsigned char> &image,
		MColorArray &palette
		) const;

	virtual MStatus setDependentsDirty(
        const MPlug &plugBeingDirtied,
        MPlugArray &affectedPlugs );


	static  void		*creator();

	static MStatus initialize();
	static MObject aImageFilename;
	static MObject aResize;
	static MObject aResizeResolution;
	static MObject aMaxOutputs;

	static MObject aInputPalette;
	
	static MObject aApplyCrop;
	static MObject aCropCorner;
	static MObject aCropResolution;
	
	static MObject aOutputImage;
	static MObject aOutputColor; 
	static MObject aOutput;
	static MObject aOutputCount;
	static MObject aOutputCropFactor;
	static MObject aOutputOffsetFactorX;
	static MObject aOutputOffsetFactorY;
	
	static MTypeId	id;


private:

};

#endif
