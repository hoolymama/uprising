// Author: Julian Mann

#include <maya/MDataHandle.h>
#include "jMayaIds.h"

#include "cImgFloatData.h"

#define EPSILON 0.0000001

const double bigNum = 99999999999.0  ;

MTypeId cImgFloatData::id( k_cImgFloatData );

MString cImgFloatData::typeName( "cImgFloatData" );

cImgFloatData::cImgFloatData() {
	fImg = new  CImg<float> ;
}

cImgFloatData::~cImgFloatData() {
	if ( 0 != fImg ) {
		delete fImg;
		fImg = 0;
	}
}

void *cImgFloatData::creator()
{
	return new cImgFloatData;
}

void cImgFloatData::copy(const MPxData &otherData)
{
	*fImg = *(((const cImgFloatData &)otherData).fImg);
}

cImgFloatData &cImgFloatData::operator=(const cImgFloatData &otherData ) {
	if (this != &otherData ) {
		*fImg = *(otherData.fImg);
	}
	return *this;
}





