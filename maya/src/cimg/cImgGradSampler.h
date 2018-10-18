
#ifndef _cImgGradSampler
#define _cImgGradSampler

#include <maya/MPxNode.h>

#include <maya/MFloatPointArray.h>
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
	static MObject aInterpolation;
	static MObject aSamplePoints;
	static MObject aProjectionMatrix;
	static MObject aChannel;
	static MObject aBlur;
	static MObject aRotation;



	static MObject aStrokeLength;
	static MObject aPointDensity;

	static MObject aOutPoints;
	static MObject aOutDirections;


	static MTypeId	id;

	// enum Channel { kAll, kRed, kGreen , kBlue, kAverage};



private:


	MStatus getImageChannel(MDataBlock &data,
	                        cImgData::Channel channel, CImg<unsigned char> &result);

	MFloatMatrix  getFullProjection(float angle,
	                                const MFloatMatrix &projection);


	int  calcFlowPoints(const MFloatPoint &point, const CImgList<float> &grad,
	                    const MFloatMatrix &mat, const MFloatMatrix &imat, int count, float gap,
	                    MFloatPointArray &resultPoints, cImgData::Interpolation interp = cImgData::kBilinear);

	// cImgData *m_data;

};

#endif
