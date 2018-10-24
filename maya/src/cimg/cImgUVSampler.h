
#ifndef _cImgUVSampler
#define _cImgUVSampler

#include <maya/MPxNode.h>

#include <maya/MFloatPointArray.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgUVSampler : public MPxNode
{
public:

	cImgUVSampler();

	virtual				~cImgUVSampler();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();

	static MObject aInputImage;
	static MObject aInterpolation;
	static MObject aChannel;
	static MObject aSampleU;
	static MObject aSampleV;

	static MObject aRemapRamp;
	static MObject aRemapRange;


	static MObject aOutput;

	static MTypeId	id;

private:


};

#endif
