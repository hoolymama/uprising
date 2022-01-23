#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatVector.h>


#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgSetRange.h"
#include "jMayaIds.h"

MTypeId cImgSetRange::id(k_cImgSetRange);



MObject cImgSetRange::aMinColor;

MObject cImgSetRange::aMaxColor;


cImgSetRange::cImgSetRange() {}

cImgSetRange::~cImgSetRange() {}

void *cImgSetRange::creator()
{
	return new cImgSetRange();
}

MStatus cImgSetRange::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;

	aMinColor= nAttr.createColor( "minColor", "mnc" );
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	st = addAttribute( aMinColor); mser;

	aMaxColor = nAttr.createColor( "maxColor", "mxc" );
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	st = addAttribute( aMaxColor ); mser;

	attributeAffects(aMinColor, aOutput);
	attributeAffects(aMaxColor, aOutput);
	

	return MS::kSuccess;
}

MStatus cImgSetRange::process(MDataBlock &data,
							 const CImg<unsigned char> &image,
							 CImg<unsigned char> &result)

{

	const MFloatVector &minColor = data.inputValue(aMinColor).asFloatVector();
	const MFloatVector &maxColor = data.inputValue(aMaxColor).asFloatVector();

	MFloatVector diff(maxColor - minColor);

	result.assign(image);

	cimg_forXYC(result, x, y, c)
	{
		const float & min = minColor[c];
		const float & range = diff[c];
		result(x, y, c) = (unsigned char)((((float(result(x, y, c)) / 255.0) * range) + min) * 255);
	}

	return MS::kSuccess;
}
 