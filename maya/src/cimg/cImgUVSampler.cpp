#include <math.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnIntArrayData.h>

#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgUtils.h"
#include "cImgData.h"
#include "cImgUVSampler.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgUVSampler::id( k_cImgUVSampler );




MObject 	cImgUVSampler::aInputImage;
MObject 	cImgUVSampler::aInterpolation;
MObject cImgUVSampler::aChannel;
MObject 	cImgUVSampler::aSampleU;
MObject 	cImgUVSampler::aSampleV;

MObject 	cImgUVSampler::aRemapRamp;
MObject 	cImgUVSampler::aRemapRange;


// MObject 	cImgUVSampler::aOutColors;
MObject 	cImgUVSampler::aOutput;



cImgUVSampler::cImgUVSampler() {}

cImgUVSampler::~cImgUVSampler() {}

void *cImgUVSampler::creator()
{
	return new cImgUVSampler();
}

MStatus cImgUVSampler::initialize()
{
	MStatus st;

	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;
	MFnUnitAttribute uAttr;

	aInputImage = tAttr.create("inputImage", "ini", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aInputImage ); mser;

	aSampleU = tAttr.create("sampleU", "su", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( false);
	tAttr.setHidden( false );
	tAttr.setWritable(true);
	st = addAttribute( aSampleU );

	aSampleV = tAttr.create("sampleV", "sv", MFnData::kDoubleArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( false);
	tAttr.setHidden( false );
	tAttr.setWritable(true);
	st = addAttribute( aSampleV );

	aInterpolation = eAttr.create("interpolation", "itp");
	eAttr.addField("Nearest", cImgData::kNearest);
	eAttr.addField("Bilinear", cImgData::kBilinear);
	eAttr.addField("Bicubic", cImgData::kBicubic);
	eAttr.setKeyable(true);
	eAttr.setWritable(true);
	addAttribute(aInterpolation);

	aChannel = eAttr.create("channel", "ch", cImgData::kAverage);
	eAttr.addField( "red",   cImgData::kRed);
	eAttr.addField( "green",   cImgData::kGreen);
	eAttr.addField( "blue",   cImgData::kBlue);
	eAttr.addField( "average",   cImgData::kAverage);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aChannel );



	aRemapRamp  = MRampAttribute::createCurveRamp("remapRamp", "rrp");
	st = addAttribute( aRemapRamp ); mser;

	aRemapRange = nAttr.create("remapRange", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aRemapRange);

	aOutput = tAttr.create("outAlphas", "oa", MFnData::kIntArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutput );

	st = attributeAffects(aInputImage, aOutput);
	st = attributeAffects(aSampleU, aOutput);
	st = attributeAffects(aSampleV, aOutput);
	st = attributeAffects(aInterpolation, aOutput);
	st = attributeAffects(aChannel, aOutput);
	st = attributeAffects(aRemapRamp, aOutput);
	st = attributeAffects(aRemapRange, aOutput);

	return MS::kSuccess;
}

MStatus cImgUVSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (plug != aOutput)  { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;



	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *inImage = imageData->fImg;
	int w = inImage->width();
	int h = inImage->height();

	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());

	cImgData::Channel channel = cImgData::Channel(data.inputValue(
	                              aChannel).asShort());


	MDataHandle hSampleU = data.inputValue( aSampleU , &st);
	MObject dSampleU = hSampleU.data();
	const MDoubleArray sampleU = MFnDoubleArrayData( dSampleU ).array( &st );

	MDataHandle hSampleV = data.inputValue( aSampleV , &st);
	MObject dSampleV = hSampleV.data();
	const MDoubleArray sampleV = MFnDoubleArrayData( dSampleV ).array( &st );

	CImg<unsigned char> image;
	cImgUtils::getImageChannel(*inImage , channel, image);
	MFloatArray values;

	if (interp == cImgData::kNearest)
	{
		cImgUtils::sampleNearest( image, sampleU, sampleV, values);
	}
	else if (interp == cImgData::kBilinear)
	{
		cImgUtils::sampleLinear( image, sampleU, sampleV, values);
	}
	else
	{
		cImgUtils::sampleCubic ( image, sampleU, sampleV, values);
	}

	MObject thisObj = thisMObject();
	MRampAttribute remapRamp( thisObj, aRemapRamp ); mser;

	unsigned len = values.length();

	const int2 &range = data.inputValue( aRemapRange ).asInt2();
	int low = range[0];
	int high = range[1];

	int span = high - low;
	MIntArray output;
	for (int i = 0; i < len; ++i)
	{
		float result;
		remapRamp.getValueAtPosition( values[i], result, &st ); mser;
		int index = int(low +  (result * span) );
		index = std::max(low, std::min(index, high));
		output.append( index );
	}

	MDataHandle hOutput = data.outputValue(aOutput);
	MFnIntArrayData fnOutput;
	MObject dOutput = fnOutput.create(output);
	hOutput.set(dOutput);
	st = data.setClean( aOutput );

	return MS::kSuccess;

}
