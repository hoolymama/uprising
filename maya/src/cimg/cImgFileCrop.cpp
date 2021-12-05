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
#include "cImgFileCrop.h"
#include "jMayaIds.h"

MTypeId cImgFileCrop::id(k_cImgFileCrop);

MObject cImgFileCrop::aImageFilename;
MObject cImgFileCrop::aResize;
MObject cImgFileCrop::aResizeResolution;
MObject cImgFileCrop::aBoundary;
MObject cImgFileCrop::aApplyCrop;
MObject cImgFileCrop::aCropCorner;
MObject cImgFileCrop::aCropResolution;

MObject cImgFileCrop::aOutput;
MObject cImgFileCrop::aOutputCropFactor;
MObject cImgFileCrop::aOutputOffsetFactorX;
MObject cImgFileCrop::aOutputOffsetFactorY;


cImgFileCrop::cImgFileCrop() {}

cImgFileCrop::~cImgFileCrop() {}

void *cImgFileCrop::creator()
{
	return new cImgFileCrop();
}

MStatus cImgFileCrop::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;
	MFnEnumAttribute eAttr;
	

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute(aImageFilename);

	aBoundary = eAttr.create("boundary", "bnd", cImgFileCrop::kBoundaryDirichlet);
	eAttr.addField("dirichlet", cImgFileCrop::kBoundaryDirichlet);
	eAttr.addField("neumann", cImgFileCrop::kBoundaryNeumann);
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
	attributeAffects(aImageFilename, aOutput);
	attributeAffects(aResize, aOutput);
	attributeAffects(aResizeResolution, aOutput);
	attributeAffects(aApplyCrop, aOutput);

	attributeAffects(aImageFilename, aOutputCropFactor);
	attributeAffects(aResize, aOutputCropFactor);
	attributeAffects(aResizeResolution, aOutputCropFactor);
	attributeAffects(aApplyCrop, aOutputCropFactor);

	attributeAffects(aImageFilename, aOutputOffsetFactorX);
	attributeAffects(aResize, aOutputOffsetFactorX);
	attributeAffects(aResizeResolution, aOutputOffsetFactorX);
	attributeAffects(aApplyCrop, aOutputOffsetFactorX);

	attributeAffects(aImageFilename, aOutputOffsetFactorY);
	attributeAffects(aResize, aOutputOffsetFactorY);
	attributeAffects(aResizeResolution, aOutputOffsetFactorY);
	attributeAffects(aApplyCrop, aOutputOffsetFactorY);

	return MS::kSuccess;
}

MStatus cImgFileCrop::compute(const MPlug &plug, MDataBlock &data)
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
	


	MString imageFilename = data.inputValue(aImageFilename).asString();
	CImg<unsigned char> image(imageFilename.asChar());

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
		outimage->assign(image.get_crop(cropX, cropY,cropX+cropRes-1,cropY+cropRes-1, boundary ));


		float fSquareRes = float(squareRes);
		cropFactor = cropResX / fSquareRes;
		offsetFactorX = (cropX+xOffset) / fSquareRes;
		offsetFactorY = (cropY+yOffset) / fSquareRes;


	} else {
 
		outimage->assign(squareRes, squareRes, 1, 1, 0);
		if (xOffset > 0) {
			CImg<unsigned char> leftImg = image.get_crop(0,0,0,yres).resize(xOffset, yres, -100, -100, 1);
			CImg<unsigned char> rightImg = image.get_crop(xres-1,0, xres-1,yres).resize(xOffset, yres, -100, -100, 1);
			outimage->draw_image(0, 0, 0, 0, leftImg);
			outimage->draw_image(xOffset+xres, 0, 0, 0, rightImg);
		} else if (yOffset > 0) {
			CImg<unsigned char> topImg = image.get_crop(0,0,xres,0).resize(xres, yOffset, -100, -100, 1);
			CImg<unsigned char> bottomImg = image.get_crop(0,yres-1, xres,yres-1).resize(xres, yOffset, -100, -100, 1);
			outimage->draw_image(0, 0, 0, 0, topImg);
			outimage->draw_image(0, yOffset+yres, 0, 0, bottomImg);
		}
		outimage->draw_image(xOffset, yOffset, 0, 0, image);
	}

 





	// cerr << "cImgFileCrop: ------" << endl;
	// cerr << "w: " <<  outimage->width() << endl;
	// cerr << "h: " <<  outimage->height() << endl;
	// cerr << "s: " <<  outimage->spectrum() << endl;
	// cerr << "d: " <<  outimage->depth() << endl;


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


MStatus cImgFileCrop::setDependentsDirty(
	const MPlug &plugBeingDirtied,
	MPlugArray &affectedPlugs)
{
	MObject thisNode = thisMObject();
	const MString &affectorName = plugBeingDirtied.partialName();
	MPlug outPlug = MPlug(thisNode, cImgFileCrop::aOutput);
	if (affectorName == "ccr0" ||
		affectorName == "ccr1" ||
		affectorName == "ccr" ||
		affectorName == "crs")
	{
		bool apply;
		MPlug(thisNode, cImgFileCrop::aApplyCrop).getValue(apply);
		if (apply)
		{
			affectedPlugs.append(MPlug(thisNode, cImgFileCrop::aOutput));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileCrop::aOutputCropFactor));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileCrop::aOutputOffsetFactorX));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileCrop::aOutputOffsetFactorY));

		}
	}
	return MS::kSuccess;
}
