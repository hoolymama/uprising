
#ifndef _axisImg
#define _axisImg

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class axisImg : public MPxNode
{
public:

	axisImg();

	virtual				~axisImg();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();

	static MObject aSkeletonImage;
	static MObject aMaxRadius;

	static MObject aOutput;

	static MTypeId	id;


private:


	CImg<float> *getImage(MDataBlock &data, MObject &attribute );

};

#endif
