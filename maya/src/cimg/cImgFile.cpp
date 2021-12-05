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
#include "cImgFile.h"
#include "jMayaIds.h"

MTypeId cImgFile::id(k_cImgFile);

MObject cImgFile::aImageFilename;
MObject cImgFile::aResize;
MObject cImgFile::aResolution;
MObject cImgFile::aInterpolation;

MObject cImgFile::aOutput;

cImgFile::cImgFile() {}

cImgFile::~cImgFile() {}

void *cImgFile::creator()
{
	return new cImgFile();
}

MStatus cImgFile::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute(aImageFilename);

	aResize = nAttr.create("resize", "rsz", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aResize);

	aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolution);

	aInterpolation = eAttr.create("interpolation", "int", cImgData::kNearestNeighbor);
	eAttr.addField("noInterpolation", cImgData::kNoInterpolation);
	eAttr.addField("nearestNeighbor", cImgData::kNearestNeighbor);
	eAttr.addField("movingAverage", cImgData::kMovingAverage);
	eAttr.addField("linear", cImgData::kLinear);
	eAttr.addField("grid", cImgData::kGrid);
	eAttr.addField("cubic", cImgData::kCubic);
	eAttr.addField("lanczos", cImgData::kLanczos);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute(aInterpolation);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aImageFilename, aOutput);
	mser;
	st = attributeAffects(aResize, aOutput);
	mser;
	st = attributeAffects(aResolution, aOutput);
	mser;
	st = attributeAffects(aInterpolation, aOutput);
	mser;

	return MS::kSuccess;
}

MStatus cImgFile::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}

	MStatus st = MS::kSuccess;

	bool resize = data.inputValue(aResize).asBool();

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid, &st);
	mser;

	cImgData *newData = (cImgData *)fnOut.data(&st);
	mser;

	CImg<unsigned char> *image = newData->fImg;

	MString imageFilename = data.inputValue(aImageFilename).asString();
	image->assign(imageFilename.asChar());
	if (!(image->width() || image->height()))
	{
		return (MS::kUnknownParameter);
	}

	if (resize)
	{
		const int2 &resolution = data.inputValue(aResolution).asInt2();

		cImgData::ResizeInterpolation interp = cImgData::ResizeInterpolation(data.inputValue(
																					 aInterpolation)
																				 .asShort());

		int xres = resolution[0];
		int yres = resolution[1];
		if (xres > 4096)
		{
			xres = 4096;
		}
		if (yres > 4096)
		{
			yres = 4096;
		}
		image->resize(xres, yres, -100, -100, interp);
	}

	hOutput.set(newData);
	data.setClean(plug);
	return MS::kSuccess;
}
