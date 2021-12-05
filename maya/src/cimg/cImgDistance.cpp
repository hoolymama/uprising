
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgFloatData.h"
#include "cImgDistance.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgDistance::id(k_cImgDistance);

MObject cImgDistance::aInput;
// MObject cImgDistance::aBlur;
MObject cImgDistance::aOutput;

cImgDistance::cImgDistance() {}

cImgDistance::~cImgDistance() {}

void *cImgDistance::creator()
{
	return new cImgDistance();
}

MStatus cImgDistance::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	aInput = tAttr.create("input", "in", cImgData::id);
	tAttr.setReadable(false);
	tAttr.setKeyable(true);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute(aInput);
	mser;

	// aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	// nAttr.setHidden(false);
	// nAttr.setStorable(true);
	// nAttr.setReadable(true);
	// nAttr.setKeyable(true);
	// nAttr.setDefault(1.0f);
	// st = addAttribute(aBlur);

	aOutput = tAttr.create("output", "out", cImgFloatData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aInput, aOutput);
	mser;
	// st = attributeAffects(aBlur, aOutput);
	// mser;

	return MS::kSuccess;
}

MStatus cImgDistance::compute(const MPlug &plug, MDataBlock &data)
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

	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *inImage = imageData->fImg;

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgFloatData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgFloatData *newData = (cImgFloatData *)fnOut.data(&st); mser;

	CImg<float> *outImage = newData->fImg;



	if (! data.inputValue(state).asShort()) {
		process(data, *inImage, *outImage);
	}

	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;

}

MStatus cImgDistance::process(MDataBlock &data, const CImg<unsigned char> &image, CImg<float> &result)
{
	int w = image.width();
	int h = image.height();
	int c = image.spectrum();
	// const float black= 0.0f;
	if (c == 1) {
		result = image.get_distance(0);
	} else {
		result = image.get_channel(0).get_distance(0);
	}

	return MS::kSuccess;
}
