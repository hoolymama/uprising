#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MColorArray.h>
#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgCrop.h"
#include "jMayaIds.h"

MTypeId cImgCrop::id(k_cImgCrop);
 
MObject cImgCrop::aInput;
MObject cImgCrop::aResize;
MObject cImgCrop::aResizeResolution;
MObject cImgCrop::aBoundary;
MObject cImgCrop::aApplyCrop;
MObject cImgCrop::aCropCorner;
MObject cImgCrop::aCropResolution;

MObject cImgCrop::aOutput;
MObject cImgCrop::aOutputCropFactor;
MObject cImgCrop::aOutputOffsetFactorX;
MObject cImgCrop::aOutputOffsetFactorY;


cImgCrop::cImgCrop() {}

cImgCrop::~cImgCrop() {}

void *cImgCrop::creator()
{
	return new cImgCrop();
}

MStatus cImgCrop::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;
	MFnEnumAttribute eAttr;
	
	aInput = tAttr.create("input", "in", cImgData::id ) ;
	tAttr.setReadable(false);
	tAttr.setKeyable(true);
	st = addAttribute( aInput ); mser;

	aBoundary = eAttr.create("boundary", "bnd", cImgCrop::kBoundaryDirichlet);
	eAttr.addField("dirichlet", cImgCrop::kBoundaryDirichlet);
	eAttr.addField("neumann", cImgCrop::kBoundaryNeumann);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute(aBoundary);
	
	aResize = nAttr.create("resize", "rsz", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	st = addAttribute(aResize);

	aResizeResolution = nAttr.create("resizeResolution", "rrs", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResizeResolution);

	aApplyCrop = nAttr.create("applyCrop", "apc", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	st = addAttribute(aApplyCrop);

	aCropCorner = nAttr.create("cropCorner", "ccr", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aCropCorner);

	aCropResolution = nAttr.create("cropResolution", "crs", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aCropResolution);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	tAttr.setKeyable(false);
	addAttribute(aOutput);


	aOutputCropFactor = nAttr.create("outputCropFactor", "ocf", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputCropFactor);

	aOutputOffsetFactorX = nAttr.create("outputOffsetFactorX", "oofx", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputOffsetFactorX);

	aOutputOffsetFactorY= nAttr.create("outputOffsetFactorY", "oofy", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputOffsetFactorY);



	attributeAffects(aBoundary, aOutput);
	attributeAffects(aInput, aOutput);
	attributeAffects(aResize, aOutput);
	attributeAffects(aResizeResolution, aOutput);
	attributeAffects(aApplyCrop, aOutput);

	attributeAffects(aInput, aOutputCropFactor);
	attributeAffects(aResize, aOutputCropFactor);
	attributeAffects(aResizeResolution, aOutputCropFactor);
	attributeAffects(aApplyCrop, aOutputCropFactor);

	attributeAffects(aInput, aOutputOffsetFactorX);
	attributeAffects(aResize, aOutputOffsetFactorX);
	attributeAffects(aResizeResolution, aOutputOffsetFactorX);
	attributeAffects(aApplyCrop, aOutputOffsetFactorX);

	attributeAffects(aInput, aOutputOffsetFactorY);
	attributeAffects(aResize, aOutputOffsetFactorY);
	attributeAffects(aResizeResolution, aOutputOffsetFactorY);
	attributeAffects(aApplyCrop, aOutputOffsetFactorY);

	return MS::kSuccess;
}

MStatus cImgCrop::compute(const MPlug &plug, MDataBlock &data)
{

	MObject thisObj = thisMObject();

	if (!(
			(plug == aOutput) 
			|| (plug == aOutputCropFactor)
			|| (plug == aOutputOffsetFactorX)
			|| (plug == aOutputOffsetFactorY)
			))
	{
		return (MS::kUnknownParameter);
	}

	MPlug outputPlug(thisObj, aOutput);

	MStatus st = MS::kSuccess;
	bool resize = data.inputValue(aResize).asBool();
	bool applyCrop = data.inputValue(aApplyCrop).asBool();
	short boundary = data.inputValue(aBoundary).asShort();

	float cropFactor = 1.0f;
	float offsetFactorX = 0.0f;
	float offsetFactorY = 0.0f;
	

	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {
		return MS::kUnknownParameter;
	}
	cImgData *imageData = (cImgData *)fnImageData.data();
	// CImg<unsigned char> *inImage = imageData->fImg;
	CImg<unsigned char> image(*(imageData->fImg));

 
	int xres = image.width();
	int yres = image.height();
	int spectrum = image.spectrum();

	if (!(xres && yres))
	{
		return (MS::kUnknownParameter);
	}

	if (resize)
	{
		int resolution = data.inputValue(aResizeResolution).asInt();
		resolution = std::min(std::max(2, resolution), 4096);

		if (xres > yres)
		{
			yres = int((yres * resolution) / float(xres));
			xres = resolution;
		} else  {
			xres = int((xres * resolution) / float(yres));
			yres = resolution;
		}
		image.resize(xres, yres, -100, -100, 1);
	}


	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid, &st);
	mser;
	cImgData *newData = (cImgData *)fnOut.data(&st);
	CImg<unsigned char> *outimage = newData->fImg;

	int squareRes = std::max(xres, yres);
	int xOffset = (squareRes - xres) / 2;
	int yOffset = (squareRes - yres) / 2;


	if (applyCrop)
	{
		const int2 &cropCorner = data.inputValue(aCropCorner).asInt2();
		const int cropResolution = data.inputValue(aCropResolution).asInt();

		int cropX = std::max(cropCorner[0], 0);
		int cropY = std::max(cropCorner[1], 0);
		int cropResX = std::min(cropResolution, xres - cropX);
		int cropResY = std::min(cropResolution, yres - cropY);
		int cropRes = fmin(cropResX, cropResY);

		;
		outimage->assign(image.crop(cropX, cropY,cropX+cropRes-1,cropY+cropRes-1, boundary ));


		float fSquareRes = float(squareRes);
		cropFactor = cropResX / fSquareRes;
		offsetFactorX = (cropX+xOffset) / fSquareRes;
		offsetFactorY = (cropY+yOffset) / fSquareRes;


	} else {
		outimage->assign(squareRes, squareRes, 1, 1, 0);
		// if (xOffset > 0) {
		// 	CImg<unsigned char> leftImg = image.get_crop(0,0,0,yres).resize(xOffset, yres, -100, -100, 1);
		// 	CImg<unsigned char> rightImg = image.get_crop(xres-1,0, xres-1,yres).resize(xOffset, yres, -100, -100, 1);
		// 	outimage->draw_image(0, 0, 0, 0, leftImg);
		// 	outimage->draw_image(xOffset+xres, 0, 0, 0, rightImg);
		// } else if (yOffset > 0) {
		// 	CImg<unsigned char> topImg = image.get_crop(0,0,xres,0).resize(xres, yOffset, -100, -100, 1);
		// 	CImg<unsigned char> bottomImg = image.get_crop(0,yres-1, xres,yres-1).resize(xres, yOffset, -100, -100, 1);
		// 	outimage->draw_image(0, 0, 0, 0, topImg);
		// 	outimage->draw_image(0, yOffset+yres, 0, 0, bottomImg);
		// }
		outimage->draw_image(xOffset, yOffset, 0, 0, image);
	}

	MDataHandle hOutputCropFactor = data.outputValue(aOutputCropFactor);
	hOutputCropFactor.set(cropFactor);
	hOutputCropFactor.setClean();

	MDataHandle hOutputOffsetFactorX = data.outputValue(aOutputOffsetFactorX);
	hOutputOffsetFactorX.set(offsetFactorX);
	hOutputOffsetFactorX.setClean();

	MDataHandle hOutputOffsetFactorY = data.outputValue(aOutputOffsetFactorY);
	hOutputOffsetFactorY.set(offsetFactorY);
	hOutputOffsetFactorY.setClean();


	hOutput.set(newData);
	data.setClean(plug);
	return MS::kSuccess;
}


MStatus cImgCrop::setDependentsDirty(
	const MPlug &plugBeingDirtied,
	MPlugArray &affectedPlugs)
{
	MObject thisNode = thisMObject();
	const MString &affectorName = plugBeingDirtied.partialName();
	MPlug outPlug = MPlug(thisNode, cImgCrop::aOutput);
	if (affectorName == "ccr0" ||
		affectorName == "ccr1" ||
		affectorName == "ccr" ||
		affectorName == "crs")
	{
		bool apply;
		MPlug(thisNode, cImgCrop::aApplyCrop).getValue(apply);
		if (apply)
		{
			affectedPlugs.append(MPlug(thisNode, cImgCrop::aOutput));
			affectedPlugs.append(
				MPlug(thisNode, cImgCrop::aOutputCropFactor));
			affectedPlugs.append(
				MPlug(thisNode, cImgCrop::aOutputOffsetFactorX));
			affectedPlugs.append(
				MPlug(thisNode, cImgCrop::aOutputOffsetFactorY));

		}
	}
	return MS::kSuccess;
}
