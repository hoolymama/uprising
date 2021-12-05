
#ifndef _cImgConstant
#define _cImgConstant

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgConstant : public MPxNode
{
public:

	cImgConstant();

	virtual				~cImgConstant();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();
	static MObject aColor;
	static MObject aResolution;
	static MObject aOutput;

	static MTypeId	id;


private:

};

#endif
