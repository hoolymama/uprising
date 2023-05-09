
#include <string>
#include <regex>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgFileCrop.h"
#include "jMayaIds.h"

MTypeId cImgFileCrop::id(k_cImgFileCrop);

MObject cImgFileCrop::aImageFilename;
MObject cImgFileCrop::aImageFrameNumber;
MObject cImgFileCrop::aUseImageSequence;
MObject cImgFileCrop::aPadding;

MObject cImgFileCrop::aComputedImageFilename;

MObject cImgFileCrop::aResize;
MObject cImgFileCrop::aResizeResolution;
MObject cImgFileCrop::aBoundary;
MObject cImgFileCrop::aApplyCrop;
MObject cImgFileCrop::aCropCorner;
MObject cImgFileCrop::aCropResolution;
MObject cImgFileCrop::aLetterbox;

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
	MFnEnumAttribute eAttr;

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setUsedAsFilename(true);
	addAttribute(aImageFilename);

	aImageFrameNumber = nAttr.create("imageFrameNumber", "ifn", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setHidden(false);
	nAttr.setDefault(1);
	addAttribute(aImageFrameNumber);

	aPadding = nAttr.create("padding", "pad", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setHidden(false);
	nAttr.setDefault(3);
	nAttr.setMax(7);
	nAttr.setMin(1);

	addAttribute(aPadding);

	aUseImageSequence = nAttr.create("useImageSequence", "uis", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setHidden(false);
	nAttr.setDefault(false);
	addAttribute(aUseImageSequence);

	aComputedImageFilename = tAttr.create("computedImageFilename", "cim", MFnData::kString);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	addAttribute(aComputedImageFilename);

	aBoundary = eAttr.create("boundary", "bnd", cImgFileCrop::kBoundaryDirichlet);
	eAttr.addField("dirichlet", cImgFileCrop::kBoundaryDirichlet);
	eAttr.addField("neumann", cImgFileCrop::kBoundaryNeumann);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	eAttr.setStorable(true);
	st = addAttribute(aBoundary);

	aLetterbox = eAttr.create("letterbox", "lbx", cImgFileCrop::kBlack);
	eAttr.addField("black", cImgFileCrop::kBlack);
	eAttr.addField("white", cImgFileCrop::kWhite);
	eAttr.addField("repeat", cImgFileCrop::kRepeat);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	eAttr.setKeyable(true);

	st = addAttribute(aLetterbox);

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

	aOutputOffsetFactorY = nAttr.create("outputOffsetFactorY", "oofy", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputOffsetFactorY);

	attributeAffects(aImageFilename, aOutput);
	attributeAffects(aUseImageSequence, aOutput);
	attributeAffects(aImageFrameNumber, aOutput);
	attributeAffects(aPadding, aOutput);
	attributeAffects(aBoundary, aOutput);
	attributeAffects(aResize, aOutput);
	attributeAffects(aResizeResolution, aOutput);
	attributeAffects(aApplyCrop, aOutput);
	attributeAffects(aLetterbox, aOutput);

	attributeAffects(aImageFilename, aOutputCropFactor);
	attributeAffects(aUseImageSequence, aOutputCropFactor);
	attributeAffects(aImageFrameNumber, aOutputCropFactor);
	attributeAffects(aPadding, aOutputCropFactor);
	attributeAffects(aResize, aOutputCropFactor);
	attributeAffects(aResizeResolution, aOutputCropFactor);
	attributeAffects(aApplyCrop, aOutputCropFactor);
	attributeAffects(aImageFrameNumber, aOutputCropFactor);
	attributeAffects(aPadding, aOutputCropFactor);

	attributeAffects(aImageFilename, aOutputOffsetFactorX);
	attributeAffects(aUseImageSequence, aOutputOffsetFactorX);
	attributeAffects(aImageFrameNumber, aOutputOffsetFactorX);
	attributeAffects(aPadding, aOutputOffsetFactorX);
	attributeAffects(aResize, aOutputOffsetFactorX);
	attributeAffects(aResizeResolution, aOutputOffsetFactorX);
	attributeAffects(aApplyCrop, aOutputOffsetFactorX);
	attributeAffects(aImageFrameNumber, aOutputOffsetFactorX);
	attributeAffects(aPadding, aOutputOffsetFactorX);

	attributeAffects(aImageFilename, aOutputOffsetFactorY);
	attributeAffects(aUseImageSequence, aOutputOffsetFactorY);
	attributeAffects(aImageFrameNumber, aOutputOffsetFactorY);
	attributeAffects(aPadding, aOutputOffsetFactorY);
	attributeAffects(aResize, aOutputOffsetFactorY);
	attributeAffects(aResizeResolution, aOutputOffsetFactorY);
	attributeAffects(aApplyCrop, aOutputOffsetFactorY);
	attributeAffects(aImageFrameNumber, aOutputOffsetFactorY);
	attributeAffects(aPadding, aOutputOffsetFactorY);

	attributeAffects(aImageFilename, aComputedImageFilename);
	attributeAffects(aUseImageSequence, aComputedImageFilename);
	attributeAffects(aImageFrameNumber, aComputedImageFilename);
	attributeAffects(aPadding, aComputedImageFilename);

	// attributeAffects(aResize, aComputedImageFilename);
	// attributeAffects(aResizeResolution, aComputedImageFilename);
	// attributeAffects(aApplyCrop, aComputedImageFilename);

	return MS::kSuccess;
}

MStatus cImgFileCrop::compute(const MPlug &plug, MDataBlock &data)
{

	MObject thisObj = thisMObject();

	if (!(
			(plug == aOutput) ||
			(plug == aOutputCropFactor) ||
			(plug == aOutputOffsetFactorX) ||
			(plug == aOutputOffsetFactorY) ||
			(plug == aComputedImageFilename)))
	{
		return (MS::kUnknownParameter);
	}

	MPlug outputPlug(thisObj, aOutput);

	MStatus st = MS::kSuccess;
	int frameNumber = data.inputValue(aImageFrameNumber).asInt();
	int padding = data.inputValue(aPadding).asInt();

	// Find hashes for frame number
	const std::string imageFilename = std::string(data.inputValue(aImageFilename).asString().asChar());
	const std::regex base_regex("^(.*)\\.(\\d+)\\.([^\\.]+)$");
	std::smatch base_match;
	MString filename;

	if (std::regex_match(imageFilename, base_match, base_regex) && data.inputValue(aUseImageSequence).asBool())
	{
		int frameNumber = data.inputValue(aImageFrameNumber).asInt();
		std::string frameNumberString = std::to_string(frameNumber);
		std::string paddingString = std::string(padding - frameNumberString.length(), '0');
		std::string paddedFrameNumberString = paddingString + frameNumberString;

		std::string paddedFrameNumberFilename = base_match[1].str() + "." + paddedFrameNumberString + "." + base_match[3].str();

		filename = MString(paddedFrameNumberFilename.c_str());
	}
	else
	{
		filename = MString(imageFilename.c_str());
	}

	if (filename.length() == 0)
	{
		return (MS::kUnknownParameter);
	}

	MDataHandle hComputedImageFilename = data.outputValue(aComputedImageFilename);
	hComputedImageFilename.set(filename);
	hComputedImageFilename.setClean();

	if (plug == aComputedImageFilename)
	{
		// bail out early so we don't incur massive cost when we only want to see the filename sequence result
		return MS::kSuccess;
	}

	CImg<unsigned char> image(filename.asChar());

	int xres = image.width();
	int yres = image.height();
	int spectrum = image.spectrum();

	bool resize = data.inputValue(aResize).asBool();
	bool applyCrop = data.inputValue(aApplyCrop).asBool();
	short boundary = data.inputValue(aBoundary).asShort();
	cImgFileCrop::Letterbox letterbox = (cImgFileCrop::Letterbox)data.inputValue(aLetterbox).asShort();

	float cropFactor = 1.0f;
	float offsetFactorX = 0.0f;
	float offsetFactorY = 0.0f;

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
		}
		else
		{
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
		outimage->assign(image.get_crop(cropX, cropY, cropX + cropRes - 1, cropY + cropRes - 1, boundary));

		float fSquareRes = float(squareRes);
		cropFactor = cropResX / fSquareRes;
		offsetFactorX = (cropX + xOffset) / fSquareRes;
		offsetFactorY = (cropY + yOffset) / fSquareRes;
	}
	else
	{
		int bg = (letterbox == Letterbox::kWhite) ? 255 : 0;
		outimage->assign(squareRes, squareRes, 1, 1, bg);

		if (letterbox == Letterbox::kRepeat)
		{
			if (xOffset > 0)
			{
				CImg<unsigned char> leftImg = image.get_crop(0, 0, 0, yres).resize(xOffset, yres, -100, -100, 1);
				CImg<unsigned char> rightImg = image.get_crop(xres - 1, 0, xres - 1, yres).resize(xOffset, yres, -100, -100, 1);
				outimage->draw_image(0, 0, 0, 0, leftImg);
				outimage->draw_image(xOffset + xres, 0, 0, 0, rightImg);
			}
			else if (yOffset > 0)
			{
				CImg<unsigned char> topImg = image.get_crop(0, 0, xres, 0).resize(xres, yOffset, -100, -100, 1);
				CImg<unsigned char> bottomImg = image.get_crop(0, yres - 1, xres, yres - 1).resize(xres, yOffset, -100, -100, 1);
				outimage->draw_image(0, 0, 0, 0, topImg);
				outimage->draw_image(0, yOffset + yres, 0, 0, bottomImg);
			}
		}

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
