
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgFloatData.h"
#include "cImgGradient.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgGradient::id(k_cImgGradient);

MObject cImgGradient::aInput;
MObject cImgGradient::aBlur;
MObject cImgGradient::aOutput;

cImgGradient::cImgGradient() {}

cImgGradient::~cImgGradient() {}

void *cImgGradient::creator()
{
	return new cImgGradient();
}

MStatus cImgGradient::initialize()
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

	aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aBlur);

	aOutput = tAttr.create("output", "out", cImgFloatData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aInput, aOutput);
	mser;
	st = attributeAffects(aBlur, aOutput);
	mser;

	return MS::kSuccess;
}

MStatus cImgGradient::compute(const MPlug &plug, MDataBlock &data)
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

MStatus cImgGradient::process(MDataBlock &data, const CImg<unsigned char> &image, CImg<float> &result)
{
	int w = image.width();
	int h = image.height();
	float blur = data.inputValue(aBlur).asFloat();

	result.assign(w, h, 1, 3, 0.0f);

	// CImgList<> grad(image.get_normalize(0, 255).blur(blur).get_gradient());
	CImgList<> grad(image.get_blur(blur).get_gradient());


	// Leave edge pixels black




	cimg_forXY(result, x, y)
	{
		const float gradient[3] = {float(grad[0](x, y)), float(grad[1](x, y)), 0.0f};
		result.draw_point(x, y, gradient);
	}

	return MS::kSuccess;
}
