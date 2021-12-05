#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgGate.h"
#include "jMayaIds.h"

MTypeId cImgGate::id(k_cImgGate);

MObject cImgGate::aMinGate;
MObject cImgGate::aMaxGate;

MObject cImgGate::aDilate;
MObject cImgGate::aBlur;



cImgGate::cImgGate() {}

cImgGate::~cImgGate() {}

void *cImgGate::creator()
{
	return new cImgGate();
}

MStatus cImgGate::initialize()
{
	MStatus st;

	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;

	aMinGate = nAttr.create("minGate", "mng", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aMinGate);

	aMaxGate = nAttr.create("maxGate", "mxg", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1);
	st = addAttribute(aMaxGate);


	aDilate = nAttr.create("dilate", "dil", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	st = addAttribute(aDilate);

	aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	st = addAttribute(aBlur);

	attributeAffects(aBlur, aOutput);
	attributeAffects(aDilate, aOutput);

	attributeAffects(aMinGate, aOutput);
	attributeAffects(aMaxGate, aOutput);

	return MS::kSuccess;
}

MStatus cImgGate::process(MDataBlock &data, const CImg<unsigned char> &image,
						  CImg<unsigned char> &result)

{

	int minGate = data.inputValue(aMinGate).asInt();
	int maxGate = data.inputValue(aMaxGate).asInt();
	int dilate = data.inputValue(aDilate).asInt();
	int blur = data.inputValue(aBlur).asFloat();

	minGate = std::min(std::max(minGate, 0), 255);
	maxGate = std::min(std::max(maxGate, 0), 255);

	result = image.get_channel(0);

	// cerr << "minGate maxGate = " << minGate << " " << maxGate << endl;


	// cerr << "w: " << result.width() << endl;
	// cerr << "h: " << result.height() << endl;
	// cerr << "s: " << result.spectrum() << endl;
	// cerr << "d: " << result.depth() << endl;
 
	
	cimg_forXY(result, x, y)
	{
		unsigned char val = result(x, y);
		result(x, y) = (val >= minGate && val <= maxGate) ? 255 : 0;
	}

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
		cimg_forXY(result, x, y)
		{
			unsigned char val = result(x, y);
			if (val < 255 and val > 0) {
				result(x, y)  = val >= 128 ? 255 : 0;
			}
		}
	}


	return MS::kSuccess;
}
