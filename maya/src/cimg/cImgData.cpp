// Author: Julian Mann

#include <maya/MDataHandle.h>
#include "jMayaIds.h"

#include "cImgData.h"

#define EPSILON 0.0000001

const double bigNum = 99999999999.0  ;

MTypeId cImgData::id( k_cImgData );

MString cImgData::typeName( "cImgData" );

cImgData::cImgData() {
	fImg = new  CImg<unsigned char> ;
}

cImgData::~cImgData() {
	if ( 0 != fImg ) {
		delete fImg;
		fImg = 0;
	}
}

void *cImgData::creator()
{
	return new cImgData;
}

void cImgData::copy(const MPxData &otherData)
{
	*fImg = *(((const cImgData &)otherData).fImg);
}

cImgData &cImgData::operator=(const cImgData &otherData ) {
	if (this != &otherData ) {
		*fImg = *(otherData.fImg);
	}
	return *this;
}





