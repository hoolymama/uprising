#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnPluginData.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgFloatData.h"

#include "cImgFloatGate.h"
#include "jMayaIds.h"

MTypeId cImgFloatGate::id(k_cImgFloatGate);

MObject cImgFloatGate::aMinGate;
MObject cImgFloatGate::aMaxGate;
MObject cImgFloatGate::aInput;
MObject cImgFloatGate::aOutput;
 

cImgFloatGate::cImgFloatGate() {}

cImgFloatGate::~cImgFloatGate() {}

void *cImgFloatGate::creator()
{
	return new cImgFloatGate();
}

MStatus cImgFloatGate::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	

	aMinGate = nAttr.create("minGate", "mng", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(10.0f);
	st = addAttribute(aMinGate);

	aMaxGate = nAttr.create("maxGate", "mxg", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(20.0f);
	st = addAttribute(aMaxGate);

	aInput = tAttr.create("input", "in", cImgFloatData::id);
	tAttr.setReadable(false);
	tAttr.setKeyable(true);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute(aInput);
	mser;

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);


	attributeAffects(aInput, aOutput);
	attributeAffects(aMinGate, aOutput);
	attributeAffects(aMaxGate, aOutput);

	return MS::kSuccess;
}

MStatus cImgFloatGate::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}
	MStatus st;


	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {
		return MS::kUnknownParameter;
	}

	cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
	CImg<float> *inImage = imageData->fImg;

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	CImg<unsigned char> *outImage = newData->fImg;

	if (! data.inputValue(state).asShort()) {
		process(data, *inImage, *outImage);
	}

	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;

}

MStatus cImgFloatGate::process(MDataBlock &data, const CImg<float> &image,
						  CImg<unsigned char> &result)

{

	int w = image.width();
	int h = image.height();
	result.assign(w, h, 1, 1, 0);

	float minGate = data.inputValue(aMinGate).asFloat();
	float maxGate = data.inputValue(aMaxGate).asFloat();
 
	minGate = std::max(minGate, 0.0f);
	maxGate = std::max(maxGate, 0.0f);

 
 
	cimg_forXY(result, x, y)
	{
		float val = image(x, y);
		result(x, y) = (val >= minGate && val <= maxGate) ? 255 : 0;
	}
 
	return MS::kSuccess;
}
