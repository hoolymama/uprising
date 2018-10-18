
#ifndef _cImgData
#define _cImgData


#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include "CImg.h"

using namespace cimg_library;

class cImgData : public MPxData
{
public:
	cImgData();
	virtual	~cImgData();
	static	void		*creator();
	virtual	void		copy(const MPxData &);
	cImgData &operator=(const cImgData &);

	virtual	MTypeId		typeId() const {return id;}
	static	MTypeId		id;
	virtual	MString		name() const {return typeName;}
	static	MString		typeName;

	CImg<unsigned char> *fImg;


	enum Interpolation { kNearest, kBilinear, kBicubic };
	enum Channel { kAll, kRed, kGreen , kBlue, kAverage};


	enum ResizeInterpolation {
		kNoInterpolation,
		kNearestNeighbor ,
		kMovingAverage ,
		kLinear ,
		kGrid ,
		kCubic ,
		kLanczos
	};




};

#endif


