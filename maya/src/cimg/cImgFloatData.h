
#ifndef _cImgFloatData
#define _cImgFloatData

// #define cimg_display 0

#include "cImgData.h"

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

class cImgFloatData : public MPxData
{
public:
	cImgFloatData();
	virtual	~cImgFloatData();
	static	void		*creator();
	virtual	void		copy(const MPxData &);
	cImgFloatData &operator=(const cImgFloatData &);

	virtual	MTypeId		typeId() const {return id;}
	static	MTypeId		id;
	virtual	MString		name() const {return typeName;}
	static	MString		typeName;

	CImg<float> *fImg;

};

#endif


