#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgMergeDilate.h"
#include "jMayaIds.h"
#include "cImgUtils.h"

MTypeId cImgMergeDilate::id(k_cImgMergeDilate);

MObject cImgMergeDilate::aDilateAmount;
MObject cImgMergeDilate::aBlur;
MObject cImgMergeDilate::aThreshold;
MObject cImgMergeDilate::aMedian;
MObject cImgMergeDilate::aOperation;

cImgMergeDilate::cImgMergeDilate() {}

cImgMergeDilate::~cImgMergeDilate() {}

void *cImgMergeDilate::creator()
{
	return new cImgMergeDilate();
}

MStatus cImgMergeDilate::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgMerge");

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aOperation = eAttr.create("operation", "op", cImgMergeDilate::kMinimum);
	eAttr.addField( "minimum",   cImgMergeDilate::kMinimum);
	eAttr.addField( "maximum",   cImgMergeDilate::kMaximum);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute( aOperation );


	aDilateAmount = nAttr.create("amount", "amt", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aDilateAmount);

	aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	st = addAttribute(aBlur);

	aThreshold = nAttr.create("threshold", "thr", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aThreshold);

	aMedian = nAttr.create("median", "med", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aMedian);


	attributeAffects(aDilateAmount, aOutput);
	attributeAffects(aBlur, aOutput);
	attributeAffects(aThreshold, aOutput);
	attributeAffects(aMedian, aOutput);

	attributeAffects(aOperation, aOutput);

	return MS::kSuccess;
}

MStatus cImgMergeDilate::process(MDataBlock &data,
                            const CImgList<unsigned char> &images,
                            CImg<unsigned char> &result)
{ 
	if (!images.size())
	{
		return MS::kUnknownParameter;
	}

	cImgMergeDilate::Operation op = cImgMergeDilate::Operation(data.inputValue(aOperation).asShort());

	int w = images.front().width();
	int h = images.front().height();
	int s = images.front().spectrum();

	unsigned char startVal = 0;
	if (images.size() == 1) {
		result.assign(images.front());
	}
	else {
		CImgList<unsigned char>::const_iterator citer;
		if (op == cImgMergeDilate::kMinimum)
		{
			result.assign(w, h, 1, s, 255);
			for (citer = images.begin(); citer != images.end(); citer++)
			{
				result.min( citer->get_resize(w, h));
			}
		}
		else {
			result.assign(w, h, 1, s, 0);
			for (citer = images.begin(); citer != images.end(); citer++)
			{
				result.max( citer->get_resize(w, h));
			}
		}
	}

	int dilate = data.inputValue(aDilateAmount).asInt();
	float blur = data.inputValue(aBlur).asFloat();
	int thresh = data.inputValue(aThreshold).asInt();
	int median = data.inputValue(aMedian).asInt();


	if (dilate < 0)
	{
		result.erode(-dilate);
	}
	else if (dilate > 0)
	{
		result.dilate(dilate);
	}

	if (blur > 0.0f)
	{
		result.blur(blur, true, true);
	}

	if (median)
	{
		result.blur_median(median);
	}

	if (thresh > -1)
	{
		cimg_forXYC(result, x, y, c)
		{
			if (result(x, y, c) > thresh)
			{
				result(x, y, c) = 255;
			}
			else
			{
				result(x, y, c) = 0;
			}
		}
	}


	return MS::kSuccess;
}