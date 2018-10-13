
#ifndef _cImgNode
#define _cImgNode

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgNode : public MPxNode
{
public:

	cImgNode();

	virtual				~cImgNode();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();
	static MObject aImageFilename;

	static MObject aSamplePoints;
	static MObject aOutput;

	static  MObject		aOutPoints;
	static  MObject		aOutDirections;


	static MTypeId	id;


private:

	cImgData *m_data;

};

#endif
