#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgNoise.h"
#include "jMayaIds.h"
#include "simplexNoise.h"

MTypeId cImgNoise::id(k_cImgNoise);

// MObject cImgNoise::aColorNoise;
MObject cImgNoise::aResolution;
MObject cImgNoise::aFrequency;
MObject cImgNoise::aAmplitude;
MObject cImgNoise::aLacunarity;
MObject cImgNoise::aPersistence;
MObject cImgNoise::aOctaves;
MObject cImgNoise::aBlur;

MObject cImgNoise::aOutput;

cImgNoise::cImgNoise() {}

cImgNoise::~cImgNoise() {}

void *cImgNoise::creator()
{
	return new cImgNoise();
}

MStatus cImgNoise::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;
	MFnTypedAttribute tAttr;
	// enum NoiseType { kGaussian, kUniform, kSaltPepper, kPoisson, kRician};

	aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolution);

	aFrequency = nAttr.create("frequency", "fre", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aFrequency);

	aAmplitude = nAttr.create("amplitude", "amp", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aAmplitude);

	aLacunarity = nAttr.create("lacunarity", "lac", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(2.0f);
	st = addAttribute(aLacunarity);

	aPersistence = nAttr.create("persistence", "per", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(0.5f);
	st = addAttribute(aPersistence);

	aOctaves = nAttr.create("octaves", "oct", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(3);
	st = addAttribute(aOctaves);

	aBlur = nAttr.create("blur", "blr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	st = addAttribute(aBlur);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aResolution, aOutput);
	mser;
	st = attributeAffects(aFrequency, aOutput);
	mser;
	st = attributeAffects(aAmplitude, aOutput);
	mser;
	st = attributeAffects(aLacunarity, aOutput);
	mser;
	st = attributeAffects(aPersistence, aOutput);
	mser;
	st = attributeAffects(aOctaves, aOutput);
	mser;
	attributeAffects(aBlur, aOutput);
	mser;

	return MS::kSuccess;
}

MStatus cImgNoise::compute(const MPlug &plug, MDataBlock &data)
{

	if (plug != aOutput)
	{
		return (MS::kUnknownParameter);
	}
	MStatus st;

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid, &st);
	mser;

	cImgData *newData = (cImgData *)fnOut.data(&st);
	mser;

	CImg<unsigned char> *image = newData->fImg;

	const int2 &resolution = data.inputValue(aResolution).asInt2();
	int w = resolution[0];
	int h = resolution[1];

	image->assign(w, h, 1, 1);

	float amp = data.inputValue(aAmplitude).asFloat();
	float fre = data.inputValue(aFrequency).asFloat();
	float lac = data.inputValue(aLacunarity).asFloat();
	float per = data.inputValue(aPersistence).asFloat();
	int oct = data.inputValue(aOctaves).asInt();
	float blur = data.inputValue(aBlur).asFloat();

	if (oct < 1)
	{
		oct = 1;
	}

	float wrecip = 1.0f / w;
	float hrecip = 1.0f / h;

	SimplexNoise noise(fre, amp, lac, per);

	cimg_forXY(*image, x, y)
	{
		float fx = (x + 0.5) * wrecip;
		float fy = (y + 0.5) * hrecip;
		float val = fmax(fmin(noise.fractal(oct, fx, fy), 1.0f), 0.0f);
		(*image)(x, y) = val * 255;
	}

	if (blur > 0.0f)
	{
		image->blur(blur, true, true);
	}

	hOutput.set(newData);
	data.setClean(plug);
	return MS::kSuccess;
}
