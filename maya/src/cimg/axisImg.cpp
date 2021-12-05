#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "axisImg.h"
#include "jMayaIds.h"
#include "cImgFloatData.h"

MTypeId axisImg::id(k_axisImg);

MObject axisImg::aSkeletonImage;
MObject axisImg::aMaxRadius;
MObject axisImg::aOutput;

axisImg::axisImg() {}

axisImg::~axisImg() {}

void *axisImg::creator()
{
	return new axisImg();
}

MStatus axisImg::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aSkeletonImage = tAttr.create("skeletonImage", "simg", cImgFloatData::id);
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute(aSkeletonImage);
	mser;

	aMaxRadius = nAttr.create("maxRadius", "mxr", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	st = addAttribute(aMaxRadius);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aSkeletonImage, aOutput);
	mser;
	st = attributeAffects(aMaxRadius, aOutput);
	mser;

	return MS::kSuccess;
}

CImg<float> *axisImg::getImage(MDataBlock &data, MObject &attribute)
{
	MStatus st;
	MDataHandle hImageData = data.inputValue(attribute, &st);
	if (st.error())
	{
		return 0;
	}
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData(dImageData, &st);
	if (st.error())
	{
		return 0;
	}
	cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
	return imageData->fImg;
}

MStatus axisImg::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}

	MStatus st = MS::kSuccess;

	int maxRadius = data.inputValue(aMaxRadius).asInt();

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid, &st);
	mser;
	cImgData *newData = (cImgData *)fnOut.data(&st);
	mser;
	CImg<unsigned char> *image = newData->fImg;

	// pointer to skel image
	CImg<float> *pImage = getImage(data, axisImg::aSkeletonImage);
	if (!pImage)
	{
		return MS::kUnknownParameter;
	}
	int w = pImage->width();
	int h = pImage->height();

	image->assign(w, h, 1, 1, 0);

	unsigned char color[] = {255};

	cimg_forXY(*pImage, x, y)
	{
		float radius = (*pImage)(x, y);
		if (radius < 0.0001)
		{
			continue;
		}
		if (radius > maxRadius)
		{
			radius = maxRadius;
		}
		image->draw_circle(x, y, int(radius), color);
	}

	hOutput.set(newData);
	data.setClean(plug);
	return MS::kSuccess;
}
