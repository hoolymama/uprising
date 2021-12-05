
#ifndef _cImgData
#define _cImgData

#define cimg_display 0

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

#include <queue>
#define cimg_plugin "plugins/skeleton.h"

#define cimg_display 0
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


	enum SampleInterpolation { kNearest, kBilinear, kBicubic };
	enum Channel { kAll, kRed, kGreen , kBlue, kAverage};

	enum NoiseType { kGaussian, kUniform, kSaltPepper, kPoisson, kRician};


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


