#include <vector>

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

#include "mayaMath.h"
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgFileSplit.h"
#include "jMayaIds.h"

MTypeId cImgFileSplit::id(k_cImgFileSplit);

MObject cImgFileSplit::aImageFilename;
// MObject cImgFileSplit::aResize;
// MObject cImgFileSplit::aResizeResolution;
MObject cImgFileSplit::aMaxOutputs;

MObject cImgFileSplit::aApplyCrop;
MObject cImgFileSplit::aCropCorner;
MObject cImgFileSplit::aCropResolution;

MObject cImgFileSplit::aInputPalette;

MObject cImgFileSplit::aOutputImage;
MObject cImgFileSplit::aOutputColor;

MObject cImgFileSplit::aOutput;
MObject cImgFileSplit::aOutputIndex;
MObject cImgFileSplit::aIndexScale;


MObject cImgFileSplit::aOutputCount;
MObject cImgFileSplit::aOutputCropFactor;
MObject cImgFileSplit::aOutputOffsetFactorX;
MObject cImgFileSplit::aOutputOffsetFactorY;

MObject cImgFileSplit::aXResolution;
MObject cImgFileSplit::aYResolution;
MObject cImgFileSplit::aSortMethod;

MObject cImgFileSplit::aHueRotate;
MObject cImgFileSplit::aSaturationRangeRemap;
MObject cImgFileSplit::aValueRangeRemap;


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
	MFnEnumAttribute eAttr;

	aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute(aImageFilename);

	// aResize = nAttr.create("resize", "rsz", MFnNumericData::kBoolean);
	// nAttr.setHidden(false);
	// nAttr.setStorable(true);
	// nAttr.setReadable(true);
	// nAttr.setKeyable(true);
	// nAttr.setDefault(false);
	// st = addAttribute(aResize);

	// aResizeResolution = nAttr.create("resizeResolution", "rrs", MFnNumericData::kInt);
	// nAttr.setHidden(false);
	// nAttr.setStorable(true);
	// nAttr.setReadable(true);
	// nAttr.setKeyable(true);
	// st = addAttribute(aResizeResolution);

	aMaxOutputs = nAttr.create("maxOutputs", "mxo", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(18);
	st = addAttribute(aMaxOutputs);

	aIndexScale = nAttr.create("indexScale", "ixs", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(18);
	st = addAttribute(aIndexScale);

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



	aSortMethod = eAttr.create("sortMethod", "mth", cImgFileSplit::kNone);
	eAttr.addField("None", cImgFileSplit::kNone);
	eAttr.addField("HSPAscending", cImgFileSplit::kHSPAscending);
	eAttr.addField("HSPDescending", cImgFileSplit::kHSPDescending);
	eAttr.setKeyable(true);
	eAttr.setHidden(false);
	st = addAttribute(aSortMethod);

	
	// color correction
	aHueRotate = nAttr.create("hueRotate", "hro", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	addAttribute(aHueRotate);


	aSaturationRangeRemap = nAttr.create("saturationRangeRemap", "srrp", MFnNumericData::k2Float);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f, 1.0f);
	addAttribute(aSaturationRangeRemap);
	
	aValueRangeRemap = nAttr.create("valueRangeRemap", "vrrp", MFnNumericData::k2Float);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f, 1.0f);
	addAttribute(aValueRangeRemap);
 



	aOutputIndex = tAttr.create("outputIndex", "outx", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	tAttr.setKeyable(false);
	addAttribute(aOutputIndex);


	aOutputImage = tAttr.create("outputImage", "outi", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	tAttr.setKeyable(false);

	aOutputColor = nAttr.createColor("outputColor", "outc");
	nAttr.setReadable(true);
	nAttr.setStorable(false);
	nAttr.setKeyable(false);

	aOutput = cAttr.create("output", "out");
	// cAttr.setKeyable(true);
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

	aOutputOffsetFactorY = nAttr.create("outputOffsetFactorY", "oofy", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutputOffsetFactorY);

	aXResolution = nAttr.create("xResolution", "xres", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	addAttribute(aXResolution);

	aYResolution = nAttr.create("yResolution", "yres", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	addAttribute(aYResolution);



	attributeAffects(aHueRotate,aOutputColor);
	attributeAffects(aSaturationRangeRemap,aOutputColor);
	attributeAffects(aValueRangeRemap,aOutputColor);
	attributeAffects(aMaxOutputs, aOutputColor);
	attributeAffects(aImageFilename, aOutputColor);
	attributeAffects(aInputPalette, aOutputColor);
	attributeAffects(aSortMethod, aOutputColor);

	attributeAffects(aMaxOutputs, aOutputImage);
	attributeAffects(aImageFilename, aOutputImage);
	attributeAffects(aInputPalette, aOutputImage);
	attributeAffects(aApplyCrop, aOutputImage);
	attributeAffects(aCropCorner, aOutputImage);
	attributeAffects(aCropResolution, aOutputImage);
	attributeAffects(aSortMethod, aOutputImage);


	attributeAffects(aMaxOutputs, aOutputIndex);
	attributeAffects(aImageFilename, aOutputIndex);
	attributeAffects(aInputPalette, aOutputIndex);
	attributeAffects(aApplyCrop, aOutputIndex);
	attributeAffects(aCropCorner, aOutputIndex);
	attributeAffects(aCropResolution, aOutputIndex);
	attributeAffects(aSortMethod, aOutputIndex);
	attributeAffects(aIndexScale, aOutputIndex);




	attributeAffects(aMaxOutputs, aOutputCount);
	attributeAffects(aImageFilename, aOutputCount);
	attributeAffects(aInputPalette, aOutputCount);

	attributeAffects(aImageFilename, aOutputCropFactor);
	attributeAffects(aApplyCrop, aOutputCropFactor);
	attributeAffects(aCropCorner, aOutputCropFactor);
	attributeAffects(aCropResolution, aOutputCropFactor);

	attributeAffects(aImageFilename, aOutputOffsetFactorX);

	attributeAffects(aApplyCrop, aOutputOffsetFactorX);
	attributeAffects(aCropCorner, aOutputOffsetFactorX);
	attributeAffects(aCropResolution, aOutputOffsetFactorX);

	attributeAffects(aImageFilename, aOutputOffsetFactorY);

	attributeAffects(aApplyCrop, aOutputOffsetFactorY);
	attributeAffects(aCropCorner, aOutputOffsetFactorY);
	attributeAffects(aCropResolution, aOutputOffsetFactorY);

	attributeAffects(aImageFilename, aXResolution);
	attributeAffects(aImageFilename, aYResolution);

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
			|| (plug == aOutputIndex) 
			|| (plug == aOutputCount) 
			|| (plug == aOutputCropFactor) 
			|| (plug == aOutputOffsetFactorX) 
			|| (plug == aOutputOffsetFactorY) 
			|| (plug == aXResolution) 
			|| (plug == aYResolution)))
	{
		return (MS::kUnknownParameter);
	}

	MPlug outputPlug(thisObj, aOutput);

	MStatus st = MS::kSuccess;
	// bool resize = data.inputValue(aResize).asBool();
	bool applyCrop = data.inputValue(aApplyCrop).asBool();

	int indexScale = data.inputValue(aIndexScale).asInt();
	indexScale = std::max(1, std::min(255,indexScale));

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

	int c = image.spectrum();
	if (c != 3)
	{
		image.resize(-100,-100,-100,3);
	}
	
	
	

	// PALETTE ///////////////////////////////////////////////////////////////
	MColorArray palette;
	calculate_pallete(data, image, palette);
	int paletteLength = palette.length();
	
	/////////////////////////////////////////////////////////////////

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
		offsetFactorX = (spriteX + xSquareOffset) / fFullSquareRes;
		offsetFactorY = (spriteY + ySquareOffset) / fFullSquareRes;
	}


	float hueRotate = data.inputValue(aHueRotate).asFloat();
	const float2 &saturationRangeRemap = data.inputValue(aSaturationRangeRemap).asFloat2();
	const float2 &valueRangeRemap = data.inputValue(aValueRangeRemap).asFloat2();
	bool doHue = hueRotate != 0.0f;
	bool doSaturation = (!(saturationRangeRemap[0] == 0.0f && saturationRangeRemap[1] == 1.0f));
	bool doValue = (!(valueRangeRemap[0] == 0.0f && valueRangeRemap[1] == 1.0f));




	CImg<unsigned char> indexSprite(spriteResX, spriteResY, 1, 1, 0);
	/////////
	
	for (int i = 0; i < paletteLength; i++)
	{

		MColor color = palette[i];

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
				sprite(x, y) = 255;
				indexSprite(x, y) = std::min((i*indexScale),255);
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

		 if (doHue || doSaturation || doValue)
		{
			MColor hsv;
			mayaMath::rgbToHsv(color, hsv);
			if (doHue)
			{
				hsv[0] =  fmod(hsv[0] + hueRotate +360.0f, 360.0f);
			}
			if (doSaturation)
			{
				hsv[1] =  saturationRangeRemap[0] + (hsv[1]  * (saturationRangeRemap[1] - saturationRangeRemap[0]));
			}
			if (doValue)
			{
				hsv[2] = valueRangeRemap[0] + (hsv[2]  * (valueRangeRemap[1] - valueRangeRemap[0]));
			}
		 
			mayaMath::hsvToRgb(hsv, color);
		}


		outColor[0] = color.r;
		outColor[1] = color.g;
		outColor[2] = color.b;

		MPlug outCompoundPlug = outputPlug.elementByLogicalIndex(i);
		MPlug outImgPlug = outCompoundPlug.child(aOutputImage);
		MPlug outColPlug = outCompoundPlug.child(aOutputColor);
		outImgPlug.setMDataHandle(hOutputImage);
		outColPlug.setMDataHandle(hOutputColor);
	}
	
	// Index image //////////////////////////////
	MDataHandle hOutputIndex = data.outputValue(aOutputIndex, &st);mser;
	MFnPluginData fnOutIndex;
	MObject dOutIndex = fnOutIndex.create(kdid, &st);
	mser;
	cImgData *newIndexData = (cImgData *)fnOutIndex.data(&st);
	CImg<unsigned char> *outIndex = newIndexData->fImg;
	if (applyCrop)
	{
		outIndex->assign(indexSprite);
	}
	else
	{
		outIndex->assign(fullSquareRes, fullSquareRes, 1, 1, 0);
		outIndex->draw_image(xSquareOffset + spriteX, ySquareOffset + spriteY, 0, 0, indexSprite);
	}
	hOutputIndex.set(newIndexData);
	hOutputIndex.setClean();
	////////////////////////////////////////
	



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
	

	MDataHandle hXResolution = data.outputValue(aXResolution);
	hXResolution.set(xres); 
	hXResolution.setClean();
	
	MDataHandle hYResolution = data.outputValue(aYResolution);
	hYResolution.set(yres);
	hYResolution.setClean();

	hOutputArray.set(bOutput);

	hOutputArray.setAllClean();


	return MS::kSuccess;
}

void cImgFileSplit::calculate_pallete(
	MDataBlock &data,
	const CImg<unsigned char> &image,
	MColorArray &palette) const
{
	MStatus st;
	int maxPaletteSize = data.inputValue(aMaxOutputs).asInt();
	maxPaletteSize = std::max(maxPaletteSize, 1);

	MArrayDataHandle hInputPalette = data.inputArrayValue(aInputPalette, &st);
	if (!st.error())
	{
		unsigned nInputs = hInputPalette.elementCount();
		if (nInputs > 0)
		{
			for (unsigned i = 0; i < nInputs; i++, hInputPalette.next())
			{
				const MFloatVector &color = hInputPalette.inputValue().asFloatVector();
				palette.append(MColor(color.x, color.y, color.z));
			}
			return;
		}
	}


	std::vector<MColor> colors;

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
			
			colors.push_back(color);
			// palette.append(color);
			started = true;
			lastIndex = 0;
		}
		else
		{
			// Check the last index first as its the most likely.
			if (color == colors[lastIndex])
			{
				continue;
			}
			else
			{
				
				bool found = false;
				for (unsigned i = 0; i < colors.size(); i++)
				{
					if (color == colors[i])
					{
						lastIndex = i;
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (colors.size() == maxPaletteSize)
					{
						break;
					}
					colors.push_back(color);
				}
			}
		}
	}
	
	cImgFileSplit::Method method = cImgFileSplit::Method(data.inputValue(aSortMethod).asShort());
	if (method != cImgFileSplit::kNone) {
		std::sort(colors.begin(), colors.end(), cImgFileSplit::compareHSP);
		if (method == cImgFileSplit::kHSPDescending) {
			std::reverse(colors.begin(), colors.end());
		}
	}
	
	std::vector<MColor>::const_iterator it = colors.begin();
	for (; it != colors.end(); it++)
	{
		palette.append(*it);
	}
}

// MStatus cImgFileSplit::setDependentsDirty(
// 	const MPlug &plugBeingDirtied,
// 	MPlugArray &affectedPlugs)
// {

// 	MObject thisNode = thisMObject();
// 	const MString &affectorName = plugBeingDirtied.partialName();

// 	MPlug outArrayPlug = MPlug(thisNode, cImgFileSplit::aOutput);
// 	// Attribite affects for crop attributes only when
// 	// applyCrop is on.
// 	// 
// 	if (affectorName == "ccr0" ||
// 		affectorName == "ccr1" ||
// 		affectorName == "ccr" ||
// 		affectorName == "crs")
// 	{
// 		bool apply;
// 		MPlug(thisNode, cImgFileSplit::aApplyCrop).getValue(apply);

// 		if (apply)
// 		{
// 			// 
// 			for (unsigned i = 0; i < outArrayPlug.numElements(); i++)
// 			{
// 				MPlug elementPlug = outArrayPlug[i];
// 				affectedPlugs.append(elementPlug);
// 				affectedPlugs.append(elementPlug.child(cImgFileSplit::aOutputImage));
// 				affectedPlugs.append(elementPlug.child(cImgFileSplit::aOutputColor));
// 			}

// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputCropFactor));
// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputOffsetFactorX));
// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputOffsetFactorY));
// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputImage));
// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputCount));
// 			affectedPlugs.append(
// 				MPlug(thisNode, cImgFileSplit::aOutputColor));
// 		}
// 	}

// 	return MS::kSuccess;
// }
