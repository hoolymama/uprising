#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgReverse.h"
#include "jMayaIds.h"

MTypeId cImgReverse::id(k_cImgReverse);

cImgReverse::cImgReverse() {}

cImgReverse::~cImgReverse() {}

void *cImgReverse::creator()
{
	return new cImgReverse();
}

MStatus cImgReverse::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	return MS::kSuccess;
}

MStatus cImgReverse::process(MDataBlock &data,
							 const CImg<unsigned char> &image,
							 CImg<unsigned char> &result)

{
	result.assign(((image - 255) * -1));

	return MS::kSuccess;
}