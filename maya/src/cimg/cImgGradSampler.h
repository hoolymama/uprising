
#ifndef _cImgGradSampler
#define _cImgGradSampler

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MTime.h>


#include "cImgData.h"

class cImgGradSampler : public MPxNode
{
public:

	cImgGradSampler();

	virtual				~cImgGradSampler();

	virtual MStatus		compute( const MPlug &plug, MDataBlock &data );

	static  void		*creator();

	static MStatus initialize();

	static MObject aInputImage;
	static MObject aSamplePoints;
	static MObject aProjectionMatrix;
	static MObject aChannel;
	static MObject aDirection;

	static  MObject		aOutPoints;
	static  MObject		aOutDirections;


	static MTypeId	id;

	enum Channel { kAll, kRed, kGreen , kBlue, kAverage};

	enum Direction { kDown, kUp, kLeft , kRight};


private:

	cImgData *m_data;

};

#endif
