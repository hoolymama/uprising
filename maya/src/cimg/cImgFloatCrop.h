
#ifndef _cImgFloatCrop
#define _cImgFloatCrop

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


 
class cImgFloatCrop : public MPxNode
{
public:

	cImgFloatCrop();

	virtual				~cImgFloatCrop();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	virtual MStatus setDependentsDirty(
        const MPlug &plugBeingDirtied,
        MPlugArray &affectedPlugs );

	static  void		*creator();

	static MStatus initialize();

	static MObject aInput;
	static MObject aResize;
	static MObject aResizeResolution;
	static MObject aBoundary;
	static MObject aApplyCrop;
	static MObject aCropCorner;
	static MObject aCropResolution;
	static MObject aOutput;
	static MObject aOutputCropFactor;
	static MObject aOutputOffsetFactorX;
	static MObject aOutputOffsetFactorY;

	static MTypeId	id;


private:

	enum Boundary { kBoundaryDirichlet, kBoundaryNeumann};


};

#endif
