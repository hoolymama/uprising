#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgDilate.h"
#include "jMayaIds.h"
#include "cImgUtils.h"

MTypeId cImgDilate::id(k_cImgDilate);

MObject cImgDilate::aDilateAmount;
MObject cImgDilate::aBlur;
MObject cImgDilate::aThreshold;
MObject cImgDilate::aMedian;
MObject cImgDilate::aBitwise;
cImgDilate::cImgDilate() {}

cImgDilate::~cImgDilate() {}

void *cImgDilate::creator()
{
	return new cImgDilate();
}

MStatus cImgDilate::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

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

	aBitwise = eAttr.create("bitwise", "btw", cImgDilate::kNone);
	eAttr.addField("None", cImgDilate::kNone);
	eAttr.addField("OR", cImgDilate::kOR);
	eAttr.addField("AND", cImgDilate::kAND);
	eAttr.addField("XOR", cImgDilate::kXOR);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	eAttr.setKeyable(true);
	st = addAttribute(aBitwise);

	attributeAffects(aDilateAmount, aOutput);
	attributeAffects(aBlur, aOutput);
	attributeAffects(aThreshold, aOutput);
	attributeAffects(aMedian, aOutput);
	attributeAffects(aBitwise, aOutput);

	return MS::kSuccess;
}

MStatus cImgDilate::process(MDataBlock &data,
							const CImg<unsigned char> &image,
							CImg<unsigned char> &result)

{

	int dilate = data.inputValue(aDilateAmount).asInt();
	float blur = data.inputValue(aBlur).asFloat();
	int thresh = data.inputValue(aThreshold).asInt();
	int median = data.inputValue(aMedian).asInt();
	cImgDilate::Bitwise bitwise = cImgDilate::Bitwise(data.inputValue(aBitwise).asShort());

	// cImgUtils::img_stats("cImgDilate start", &image);

	result.assign(image);

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

	switch (bitwise)
	{
	case cImgDilate::kOR:
		result |= image;
		break;
	case cImgDilate::kXOR:
		result ^= image;
		break;
	case cImgDilate::kAND:
		result &= image;
		break;
	default:
		break;
	}
	// cImgUtils::img_stats("cImgDilate end", &result);
	return MS::kSuccess;
}