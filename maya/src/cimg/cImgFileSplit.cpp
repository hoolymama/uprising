#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MColorArray.h>
#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgFileSplit.h"
#include "jMayaIds.h"

MTypeId cImgFileSplit::id(k_cImgFileSplit);

MObject cImgFileSplit::aImageFilename;
MObject cImgFileSplit::aResize;
MObject cImgFileSplit::aResizeResolution;
MObject cImgFileSplit::aMaxOutputs;
// MObject cImgFileSplit::aMakeSquare;
MObject cImgFileSplit::aApplyCrop;
MObject cImgFileSplit::aCropCorner;
MObject cImgFileSplit::aCropResolution;
// MObject cImgFileSplit::aExcludeColor;

MObject cImgFileSplit::aInputPalette;

MObject cImgFileSplit::aOutputImage;
MObject cImgFileSplit::aOutputColor;

MObject cImgFileSplit::aOutput;
MObject cImgFileSplit::aOutputCount;
MObject cImgFileSplit::aOutputCropFactor;
MObject cImgFileSplit::aOutputOffsetFactorX;
MObject cImgFileSplit::aOutputOffsetFactorY;


cImgFileSplit::cImgFileSplit() {}

cImgFileSplit::~cImgFileSplit() {}

void *cImgFileSplit::creator()
{
	return new cImgFileSplit();
}

MStatus cImgFileSplit::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnCompoundAttribute cAttr;

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute(aImageFilename);

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

	aMaxOutputs = nAttr.create("maxOutputs", "mxo", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(18);
	st = addAttribute(aMaxOutputs);

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

	aInputPalette = nAttr.createColor("inputPalette", "inp");
	nAttr.setReadable(false);
	nAttr.setKeyable(true);
	nAttr.setArray(true);
	nAttr.setDisconnectBehavior(MFnAttribute::kDelete);
	st = addAttribute(aInputPalette);

	aOutputImage = tAttr.create("outputImage", "outi", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	tAttr.setKeyable(false);

	aOutputColor = nAttr.createColor("outputColor", "outc");
	nAttr.setReadable(true);
	nAttr.setStorable(false);
	nAttr.setKeyable(false);

	aOutput = cAttr.create("output", "out");
	cAttr.setKeyable(true);
	cAttr.setReadable(true);
	cAttr.setStorable(false);
	cAttr.addChild(aOutputImage);
	cAttr.addChild(aOutputColor);
	cAttr.setArray(true);
	cAttr.setUsesArrayDataBuilder(true);
	addAttribute(aOutput);

	aOutputCount = nAttr.create("outputCount", "opc", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputCount);

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

	
	attributeAffects(aMaxOutputs, aOutputImage);
	attributeAffects(aImageFilename, aOutputImage);
	attributeAffects(aInputPalette, aOutputImage);
	attributeAffects(aResize, aOutputImage);
	attributeAffects(aResizeResolution, aOutputImage);
	attributeAffects(aApplyCrop, aOutputImage);

	attributeAffects(aMaxOutputs, aOutputColor);
	attributeAffects(aImageFilename, aOutputColor);
	attributeAffects(aInputPalette, aOutputColor);

	attributeAffects(aMaxOutputs, aOutputCount);
	attributeAffects(aImageFilename, aOutputCount);
	attributeAffects(aInputPalette, aOutputCount);

	attributeAffects(aMaxOutputs, aOutput);
	attributeAffects(aImageFilename, aOutput);
	attributeAffects(aInputPalette, aOutput);
	attributeAffects(aResize, aOutput);
	attributeAffects(aResizeResolution, aOutput);
	attributeAffects(aApplyCrop, aOutput);

	// attributeAffects(aMaxOutputs, aOutputCropFactor);
	attributeAffects(aImageFilename, aOutputCropFactor);
	attributeAffects(aResize, aOutputCropFactor);
	attributeAffects(aResizeResolution, aOutputCropFactor);
	attributeAffects(aApplyCrop, aOutputCropFactor);

	// attributeAffects(aMaxOutputs, aOutputOffsetFactor);
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

MStatus cImgFileSplit::compute(const MPlug &plug, MDataBlock &data)
{

	MObject thisObj = thisMObject();

	if (!(
			(plug == aOutput) 
			|| (plug == aOutputImage) 
			|| (plug == aOutputColor) 
			|| (plug.parent() == aOutputColor) 
			|| (plug == aOutputCount) 
			|| (plug == aOutputCropFactor)
			|| (plug == aOutputOffsetFactorX)
			|| (plug == aOutputOffsetFactorY)
			))
	{
		return (MS::kUnknownParameter);
	}

	cerr << "cImgFileSplit::compute() PLUG:"<<  plug.name() << endl;

	MPlug outputPlug(thisObj, aOutput);

	MStatus st = MS::kSuccess;
	bool resize = data.inputValue(aResize).asBool();
	bool applyCrop = data.inputValue(aApplyCrop).asBool();

	float cropFactor = 1.0f;
	float offsetFactorX = 0.0f;
	float offsetFactorY = 0.0f;
	

	MString imageFilename = data.inputValue(aImageFilename).asString();
	CImg<unsigned char> image(imageFilename.asChar());

	int xres = image.width();
	int yres = image.height();
	if (!(xres && yres))
	{
		return (MS::kUnknownParameter);
	}

	// resize the max axis to the resolution value
	// resize other axis accordingly
	// Very important that we use nearest neighbor, because  we don't want to introduce extra colors.
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


	MColorArray palette;
	calculate_pallete(data, image, palette);
	int paletteLength = palette.length();

	int fullSquareRes = std::max(xres, yres);

	MArrayDataHandle hOutputArray = data.outputArrayValue(aOutput, &st);
	mser;
	MArrayDataBuilder bOutput = hOutputArray.builder(&st);
	mser;
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);

	int xSquareOffset = (fullSquareRes - xres) / 2;
	int ySquareOffset = (fullSquareRes - yres) / 2;

	int spriteX = 0;
	int spriteY = 0;
	int spriteResX = xres;
	int spriteResY = yres;

	if (applyCrop)
	{
		const int2 &cropCorner = data.inputValue(aCropCorner).asInt2();
		const int cropResolution = data.inputValue(aCropResolution).asInt();

		spriteX = std::max(cropCorner[0], 0);
		spriteY = std::max(cropCorner[1], 0);
		spriteResX = std::min(cropResolution, xres - spriteX);
		spriteResY = std::min(cropResolution, yres - spriteY);
		spriteResX = fmin(spriteResX, spriteResY);
		spriteResY = fmin(spriteResX, spriteResY);

		float fFullSquareRes = float(fullSquareRes);

		cropFactor = spriteResX / fFullSquareRes;
		offsetFactorX = (spriteX+xSquareOffset) / fFullSquareRes;
		offsetFactorY = (spriteY+ySquareOffset) / fFullSquareRes;
		
	}

	for (int i = 0; i < paletteLength; i++)
	{

		const MColor &color = palette[i];

		MDataHandle hOutput = bOutput.addElement(i, &st);
		mser;

		MDataHandle hOutputImage = hOutput.child(aOutputImage);
		MDataHandle hOutputColor = hOutput.child(aOutputColor);

		MObject dOut = fnOut.create(kdid, &st);
		mser;
		cImgData *newData = (cImgData *)fnOut.data(&st);
		CImg<unsigned char> *outimage = newData->fImg;

		CImg<unsigned char> sprite(spriteResX, spriteResY, 1, 1, 0);

		cimg_forXY(sprite, x, y)
		{
			int imgX = x + spriteX;
			int imgY = y + spriteY;

			MColor pixelColor(
				MColor::kRGB,
				(image)(imgX, imgY, 0),
				(image)(imgX, imgY, 1),
				(image)(imgX, imgY, 2));
			if (pixelColor == color)
			{
				(sprite)(x, y) = 255;
			}
		}

		if (applyCrop)
		{
			outimage->assign(sprite);
		}
		else
		{
			outimage->assign(fullSquareRes, fullSquareRes, 1, 1, 0);
			outimage->draw_image(xSquareOffset + spriteX, ySquareOffset + spriteY, 0, 0, sprite);
		}

		hOutputImage.set(newData);

		float3 &outColor = hOutputColor.asFloat3();
		outColor[0] = color.r;
		outColor[1] = color.g;
		outColor[2] = color.b;

		MPlug outCompoundPlug = outputPlug.elementByLogicalIndex(i);
		MPlug outImgPlug = outCompoundPlug.child(aOutputImage);
		MPlug outColPlug = outCompoundPlug.child(aOutputColor);
		outImgPlug.setMDataHandle(hOutputImage);
		outColPlug.setMDataHandle(hOutputColor);
	}

	MDataHandle hOutputCount = data.outputValue(aOutputCount);
	hOutputCount.set(paletteLength);
	hOutputCount.setClean();

	MDataHandle hOutputCropFactor = data.outputValue(aOutputCropFactor);
	hOutputCropFactor.set(cropFactor);
	hOutputCropFactor.setClean();

	MDataHandle hOutputOffsetFactorX = data.outputValue(aOutputOffsetFactorX);
	hOutputOffsetFactorX.set(offsetFactorX);
	hOutputOffsetFactorX.setClean();

	MDataHandle hOutputOffsetFactorY = data.outputValue(aOutputOffsetFactorY);
	hOutputOffsetFactorY.set(offsetFactorY);
	hOutputOffsetFactorY.setClean();
	
	hOutputArray.set(bOutput);

	hOutputArray.setAllClean();

	return MS::kSuccess;
}

void cImgFileSplit::calculate_pallete(
	MDataBlock&data,
	const CImg<unsigned char> &image,
	MColorArray &palette
	) const
{
	MStatus st;
	int maxPaletteSize = data.inputValue(aMaxOutputs).asInt();
	maxPaletteSize = std::max(maxPaletteSize, 1);

	MArrayDataHandle hInputPalette = data.inputArrayValue(aInputPalette, &st);
	if (! st.error()) {
		unsigned nInputs = hInputPalette.elementCount();
		if (nInputs > 0) {
			for (unsigned i = 0; i < nInputs; i++, hInputPalette.next())
			{
				const MFloatVector& color = hInputPalette.inputValue().asFloatVector();
				palette.append(MColor(color.x, color.y,color.z));
			}
			return;
		}
	}
 
	int lastIndex = -1;
	bool started = false;
	cimg_forXY(image, x, y)
	{
		MColor color(
			MColor::kRGB,
			(image)(x, y, 0),
			(image)(x, y, 1),
			(image)(x, y, 2));

		if (!started)
		{
			palette.append(color);
			started = true;
			lastIndex = 0;
		}
		else
		{
			// Check the last index first as its the most likely.
			if (color == palette[lastIndex])
			{
				continue;
			}
			else
			{
				bool found = false;
				for (size_t i = 0; i < palette.length(); i++)
				{
					if (color == palette[i])
					{
						lastIndex = i;
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (palette.length() == maxPaletteSize)
					{
						break;
					}
					palette.append(color);
				}
			}
		}
	}
}

MStatus cImgFileSplit::setDependentsDirty(
	const MPlug &plugBeingDirtied,
	MPlugArray &affectedPlugs)
{

	MObject thisNode = thisMObject();
	const MString &affectorName = plugBeingDirtied.partialName();

	MPlug outArrayPlug = MPlug(thisNode, cImgFileSplit::aOutput);
	// Attribite affects for crop attributes only when
	// applyCrop is on.
	// cerr << "affectorName" << affectorName<< endl;
	if (affectorName == "ccr0" ||
		affectorName == "ccr1" ||
		affectorName == "ccr" ||
		affectorName == "crs")
	{
		bool apply;
		MPlug(thisNode, cImgFileSplit::aApplyCrop).getValue(apply);

		if (apply)
		{
			// cerr << "apply: " << apply << "adding plugs" << endl;
			for (unsigned i = 0; i < outArrayPlug.numElements(); i++)
			{
				MPlug elementPlug = outArrayPlug[i];
				affectedPlugs.append(elementPlug);
				affectedPlugs.append(elementPlug.child(cImgFileSplit::aOutputImage));
				affectedPlugs.append(elementPlug.child(cImgFileSplit::aOutputColor));
			}

			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputCropFactor));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputOffsetFactorX));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputOffsetFactorY));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputImage));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputCount));
			affectedPlugs.append(
				MPlug(thisNode, cImgFileSplit::aOutputColor));
		}
	}

	return MS::kSuccess;
}

 