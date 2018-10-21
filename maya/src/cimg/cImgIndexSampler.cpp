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

#include <maya/MFnIntArrayData.h>

#include <maya/MRampAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgUtils.h"
#include "cImgData.h"
#include "cImgIndexSampler.h"
#include "jMayaIds.h"
#include "textureSampling.h"

MTypeId     cImgIndexSampler::id( k_cImgIndexSampler );




MObject 	cImgIndexSampler::aInputImage;
MObject 	cImgIndexSampler::aInterpolation;
MObject 	cImgIndexSampler::aChannel;
MObject 	cImgIndexSampler::aSampleU;
MObject 	cImgIndexSampler::aSampleV;


MObject 	cImgIndexSampler::aIndexRamp;
MObject 	cImgIndexSampler::aNumIndices;


MObject 	cImgIndexSampler::aOutIndex;


cImgIndexSampler::cImgIndexSampler() {}

cImgIndexSampler::~cImgIndexSampler() {}

void *cImgIndexSampler::creator()
{
	return new cImgIndexSampler();
}

MStatus cImgIndexSampler::initialize()
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

	eAttr.setDefault( cImgData::kBilinear );
	eAttr.setKeyable(true);
	eAttr.setWritable(true);
	addAttribute(aInterpolation);

	aIndexRamp  = MRampAttribute::createCurveRamp("indexRamp", "idxr");
	st = addAttribute( aIndexRamp ); mser;

	aNumIndices = nAttr.create( "numIndices", "nmi", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(8);
	st = addAttribute(aNumIndices);

	aChannel = eAttr.create("channel", "ch", cImgData::kAverage);
	eAttr.addField( "red",   cImgData::kRed);
	eAttr.addField( "green",   cImgData::kGreen);
	eAttr.addField( "blue",   cImgData::kBlue);
	eAttr.addField( "average",   cImgData::kAverage);
	eAttr.setHidden(false);
	eAttr.setStorable(true);
	st = addAttribute( aChannel );


	aOutIndex = tAttr.create("outIndex", "oi", MFnData::kIntArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutIndex );

	st = attributeAffects(aInputImage, aOutIndex);
	st = attributeAffects(aSampleU, aOutIndex);
	st = attributeAffects(aSampleV, aOutIndex);
	st = attributeAffects(aInterpolation, aOutIndex);
	st = attributeAffects(aChannel, aOutIndex);
	st = attributeAffects(aIndexRamp, aOutIndex);
	st = attributeAffects(aNumIndices, aOutIndex);

	return MS::kSuccess;
}




MStatus cImgIndexSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    (plug == aOutIndex) 	   ) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	cImgData::Channel channel = cImgData::Channel(data.inputValue(
	                              aChannel).asShort());

	MIntArray indices;

	int numIndices =  std::max(1, data.inputValue(aNumIndices).asInt() );

	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *colimage = imageData->fImg;

	// int w = colimage->width();
	// int h = colimage->height();

	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());

	MDataHandle hSampleU = data.inputValue( aSampleU , &st);
	MObject dSampleU = hSampleU.data();
	const MDoubleArray sampleU = MFnDoubleArrayData( dSampleU ).array( &st );

	MDataHandle hSampleV = data.inputValue( aSampleV , &st);
	MObject dSampleV = hSampleV.data();
	const MDoubleArray sampleV = MFnDoubleArrayData( dSampleV ).array( &st );

	MFloatArray fVals;
	// get one channel image.
	CImg<unsigned char> image;
	cImgUtils::getImageChannel(*colimage , channel, image);

	if (interp == cImgData::kNearest)
	{
		cImgUtils::sampleNearest(image, sampleU, sampleV, fVals);
	}
	else if (interp == cImgData::kBilinear)
	{
		cImgUtils::sampleLinear(image, sampleU, sampleV, fVals);
	}
	else if (interp == cImgData::kBicubic)
	{
		cImgUtils::sampleCubic(image, sampleU, sampleV, fVals);
	}

	unsigned len = fVals.length();
	if (! (len && len == sampleU.length() )) {
		return MS::kUnknownParameter;
	}


	MObject thisObj = thisMObject();
	MRampAttribute indexRamp( thisObj, aIndexRamp ); mser;


	for (int i = 0; i < len; ++i)
	{
		float result;
		indexRamp.getValueAtPosition( fVals[i], result, &st ); mser;
		int index = int(result * numIndices);
		index = std::max(0, std::min(index, (numIndices - 1)));
		indices.append( index );
	}





	MDataHandle hOutIndex = data.outputValue(aOutIndex);
	MFnIntArrayData fnOutIndex;
	MObject dOutIndex = fnOutIndex.create(indices);
	hOutIndex.set(dOutIndex);
	st = data.setClean( aOutIndex);



	return MS::kSuccess;

}
