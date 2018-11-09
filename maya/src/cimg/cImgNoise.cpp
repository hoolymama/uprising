#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgNoise.h"
#include "jMayaIds.h"

MTypeId     cImgNoise::id( k_cImgNoise );



MObject cImgNoise::aScale;
MObject cImgNoise::aAmplitude;
MObject cImgNoise::aInterpolation;
MObject cImgNoise::aType;




cImgNoise::cImgNoise() {}

cImgNoise::~cImgNoise() {}

void *cImgNoise::creator()
{
	return new cImgNoise();
}

MStatus cImgNoise::initialize()
{
	MStatus st;



	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;




	enum NoiseType { kGaussian, kUniform, kSaltPepper, kPoisson, kRician};



	aScale = nAttr.create( "scale", "scl", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1);
	st = addAttribute(aScale);


	aAmplitude = nAttr.create( "amplitude", "amp", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(10);
	st = addAttribute(aAmplitude);

	aType = eAttr.create("type", "tp", cImgData::kGaussian);
	eAttr.addField( "gaussian",   cImgData::kGaussian);
	eAttr.addField( "uniform",   cImgData::kUniform);
	eAttr.addField( "saltPepper",   cImgData::kSaltPepper);
	eAttr.addField( "poisson",   cImgData::kPoisson);
	eAttr.addField( "rician",   cImgData::kRician);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aType );



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
	st = addAttribute( aInterpolation );


	st  = attributeAffects(aInterpolation, aOutput );

	st = attributeAffects(aScale, aOutput );
	st = attributeAffects(aAmplitude, aOutput );
	st = attributeAffects(aType, aOutput );


	return MS::kSuccess;
}


MStatus cImgNoise::process(MDataBlock &data, const CImg<unsigned char> &image,
                           CImg<unsigned char> &result)

{


	cImgData::ResizeInterpolation interp = cImgData::ResizeInterpolation(data.inputValue(
	    aInterpolation).asShort());

	float scale = data.inputValue(aScale).asFloat();
	if (scale < 1.0f) { scale = 1.0f; }
	float amp = data.inputValue(aAmplitude).asFloat();
	cImgData::NoiseType noiseType =  cImgData::NoiseType(data.inputValue(aType).asShort());

	int w = image.width();
	int h = image.height();
	int spectrum = image.spectrum();


	int noiseImgWidth = w / scale;
	int noiseImgHeight = h / scale;

	CImg<float> noiseImage = CImg<float>(noiseImgWidth, noiseImgHeight, 1, 1, 0.0).noise(amp,
	                         int(noiseType)).resize(w, h, -100, -100, interp);


	result = (image + noiseImage) ;

	return MS::kSuccess;

}
