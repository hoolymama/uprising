
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnPluginData.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgMerge.h"
#include "cImgUtils.h"
#include "jMayaIds.h"

MTypeId cImgMerge::id(k_cImgMerge);

MObject cImgMerge::aInput;
MObject cImgMerge::aInvert;
MObject cImgMerge::aOutput;

cImgMerge::cImgMerge() {}

cImgMerge::~cImgMerge() {}

void *cImgMerge::creator()
{
	return new cImgMerge();
}

MStatus cImgMerge::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;

	aInput = tAttr.create("input", "in", cImgData::id);
	tAttr.setReadable(false);
	tAttr.setKeyable(true);
	tAttr.setArray(true);
	tAttr.setDisconnectBehavior(MFnAttribute::kDelete);

	st = addAttribute(aInput);
	mser;

	aInvert = nAttr.create("invert", "inv", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aInvert);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aInput, aOutput);
	mser;
	st = attributeAffects(aInvert, aOutput);
	mser;

	return MS::kSuccess;
}

MStatus cImgMerge::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}
	MStatus st;

	CImgList<unsigned char> images;

	MArrayDataHandle hInputs = data.inputValue(aInput, &st);
	msert;
	unsigned nInputs = hInputs.elementCount();
	for (unsigned i = 0; i < nInputs; i++, hInputs.next())
	{
		int index = hInputs.elementIndex(&st);
		MDataHandle hImageData = hInputs.inputValue(&st);
		if (st.error())
		{
			continue;
		}
		MObject dImageData = hImageData.data();
		MFnPluginData fnImageData(dImageData, &st);
		if (st.error())
		{
			continue;
		}
		cImgData *imageData = (cImgData *)fnImageData.data();
		CImg<unsigned char> *inImage = imageData->fImg;
		images.push_back(*inImage);
	}

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid, &st);
	mser;
	cImgData *newData = (cImgData *)fnOut.data(&st);
	mser;
	CImg<unsigned char> *outImage = newData->fImg;

	if (!data.inputValue(state).asShort())
	{
		st = process(data, images, *outImage);

		if (outImage && data.inputValue(aInvert).asBool())
		{
			cImgUtils::invert(*outImage);
		}
	}

	hOutput.set(newData);
	data.setClean(plug);
	return MS::kSuccess;
}

MStatus cImgMerge::process(MDataBlock &data, const CImgList<unsigned char> &images,
						   CImg<unsigned char> &result)
{
	return MS::kUnknownParameter;
}
