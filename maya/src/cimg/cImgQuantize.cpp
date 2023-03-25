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
#include "cImgUtils.h"
#include "cImgData.h"
#include "cImgQuantize.h"
#include "jMayaIds.h"




MTypeId cImgQuantize::id(k_cImgQuantize);

MObject cImgQuantize::aLevels;

MObject cImgQuantize::aDither;
MObject cImgQuantize::aGreyscale;
MObject cImgQuantize::aInput;
MObject cImgQuantize::aOutput;

cImgQuantize::cImgQuantize() {}

cImgQuantize::~cImgQuantize() {}

void *cImgQuantize::creator()
{
	return new cImgQuantize();
}

MStatus cImgQuantize::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnEnumAttribute eAttr;

	aInput = tAttr.create("input", "in", cImgData::id);
	tAttr.setReadable(false);
	tAttr.setKeyable(true);
	st = addAttribute(aInput);
	mser;

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	aLevels = nAttr.create("levels", "lvs", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setMin(2);
	nAttr.setMax(256);
	st = addAttribute(aLevels);


	aDither = eAttr.create("dither", "dth", cImgQuantize::kNone);
	eAttr.addField("None", cImgQuantize::kNone);
	eAttr.addField("Floyd Steinberg", cImgQuantize::kFloydSteinberg); 
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aDither);


	aGreyscale = nAttr.create("greyscale", "gry", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	st = addAttribute(aGreyscale);

	attributeAffects(aInput, aOutput);

	attributeAffects(aLevels, aOutput);
	attributeAffects(aGreyscale, aOutput);
	attributeAffects(aDither, aOutput);

	return MS::kSuccess;
}

MStatus cImgQuantize::compute(const MPlug &plug, MDataBlock &data)
{

	MObject thisObj = thisMObject();

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}

	MStatus st = MS::kSuccess;


	// INPUT IMAGE ///////////////////////////////////////////////////////////
	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {
		return MS::kUnknownParameter;
	}

	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *inImage = imageData->fImg;
	/////////////////////////////////////////////////////////////////

	// OUTPUT IMAGE //////////////////////////////////////////////////////////
	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	CImg<unsigned char> *outImage = newData->fImg;

	if (! data.inputValue(state).asShort()) {
		process(data, *inImage, *outImage);
	}
	else {
		outImage->assign(*inImage);
	}
	/////////////////////////////////////////////////////////////////

	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}


int quantize(int value, int levels)
{
	float range = (levels-1) / 255.0f;
	float recip = 255.0f / (levels-1);
	int q = (int)(value * range + 0.5f);
	return (int)(q * recip + 0.5f);
}


MStatus cImgQuantize::process(MDataBlock &data, const CImg<unsigned char> &image,
                             CImg<unsigned char> &result)
{
	bool greyscale = data.inputValue(aGreyscale).asBool();
	int levels = data.inputValue(aLevels).asInt();
	cImgQuantize::Dither dither = (cImgQuantize::Dither)data.inputValue(aDither).asShort();

	if (levels < 2) {
		levels = 2;
	}

	// BW image

	int w = image.width();
	int h = image.height();

	CImg<int> dest;
	if (greyscale) {
		dest.assign(w, h, 1, 1);
		cimg_forXY(image, x, y)
		{
			int g = image(x, y, 0) * 0.2162 + image(x, y, 1) * 0.7152 + image(x, y, 2) * 0.0722;
			dest(x, y, 0) = g; 
		}
	} else {
		dest.assign(image);
	}

	int s = dest.spectrum();

	result.assign(w, h, 1, s);

	if (dither == cImgQuantize::kFloydSteinberg)
	{
		// Floyd Steinberg dithering
		cimg_forXYC(dest, x, y, c)
		{
			int oldpixel = dest(x, y, c);
			int newpixel = quantize(oldpixel, levels);
			int quant_error = oldpixel - newpixel;
			dest(x, y, c) = newpixel;

			if (x < w-1) {
				dest(x+1, y, c) = dest(x+1, y, c) + quant_error * 7 / 16;
			}
			if (x > 0 && y < h-1) {
				dest(x-1, y+1, c) = dest(x-1, y+1, c) + quant_error * 3 / 16;
			}
			if (y < h-1) {
				dest(x, y+1, c) = dest(x, y+1, c) + quant_error * 5 / 16;
			}
			if (x < w-1 && y < h-1) {
				dest(x+1, y+1, c) = dest(x+1, y+1, c) + quant_error * 1 / 16;
			}
		}
		cimg_forXYC(dest, x, y, c)
		{
			result(x,y,c) =  std::min(std::max(dest(x,y,c), 0), 255);
		}
	}
	else
	{
		// No dithering
		cimg_forXYC(dest, x, y, c)
		{
			int oldpixel = dest(x, y, c);
			int newpixel = quantize(oldpixel, levels);
			result(x, y, c) = newpixel;
		}
	}

	return MS::kSuccess;
}
