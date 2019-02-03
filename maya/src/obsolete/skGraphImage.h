
#ifndef _skGraphImage
#define _skGraphImage

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class skGraphImage : public MPxNode
{
public:

	skGraphImage();

	virtual				~skGraphImage();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();

	static MObject aSkeletonImage;
	static MObject aMaxRadius;
	static MObject aPruneLength;

	static MObject aOutput;

	static MTypeId	id;


private:


	// CImg<float> *getImage(MDataBlock &data, MObject &attribute );

};

#endif
