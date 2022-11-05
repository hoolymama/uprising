
#ifndef _cImgFileCrop
#define _cImgFileCrop

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgFileCrop : public MPxNode
{
public:

	cImgFileCrop();

	virtual				~cImgFileCrop();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	void calculate_pallete(
	const CImg<unsigned char> &image, 
	int maxPaletteSize,
	MColorArray &palette) const;

	virtual MStatus setDependentsDirty(
        const MPlug &plugBeingDirtied,
        MPlugArray &affectedPlugs );


	static  void		*creator();

	static MStatus initialize();

	static MObject aImageFilename;
	static MObject aResize;
	static MObject aResizeResolution;
	static MObject aBoundary;
	static MObject aApplyCrop;
	static MObject aCropCorner;
	static MObject aCropResolution;
	static MObject aLetterbox;


	static MObject aOutput;
	static MObject aOutputCropFactor;
	static MObject aOutputOffsetFactorX;
	static MObject aOutputOffsetFactorY;



	static MTypeId	id;


private:

	enum Boundary { kBoundaryDirichlet, kBoundaryNeumann};
	enum Letterbox { kBlack, kWhite, kRepeat };


};

#endif
