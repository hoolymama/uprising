
#ifndef _cImgIndexSampler
#define _cImgIndexSampler

#include <maya/MPxNode.h>

#include <maya/MFloatPointArray.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgIndexSampler : public MPxNode
{
public:

	cImgIndexSampler();

	virtual				~cImgIndexSampler();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();

	static MObject aInputImage;
	// static MObject aInterpolation;
	// static MObject aChannel;

	static MObject aSampleU;
	static MObject aSampleV;

	// static MObject aIndexRamp;
	// static MObject aNumIndices;

	static MObject aOutIndex;

	static MTypeId	id;




private:


	// MStatus getImageChannel(MDataBlock &data,
	//                         cImgData::Channel channel, CImg<unsigned char> &result);



};

#endif
