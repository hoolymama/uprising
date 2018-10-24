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
MObject 	cImgIndexSampler::aSampleU;
MObject 	cImgIndexSampler::aSampleV;
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

	aOutIndex = tAttr.create("outIndex", "oi", MFnData::kIntArray, &st);
	tAttr.setStorable( false);
	tAttr.setReadable( true);
	st = addAttribute( aOutIndex );

	st = attributeAffects(aInputImage, aOutIndex);
	st = attributeAffects(aSampleU, aOutIndex);
	st = attributeAffects(aSampleV, aOutIndex);


	return MS::kSuccess;
}




MStatus cImgIndexSampler::compute( const MPlug &plug, MDataBlock &data ) {

	if (!
	    (plug == aOutIndex) 	   ) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	MDataHandle hImageData = data.inputValue(aInputImage, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st); msert;
	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *image = imageData->fImg;

	MDataHandle hSampleU = data.inputValue( aSampleU , &st);
	MObject dSampleU = hSampleU.data();
	const MDoubleArray sampleU = MFnDoubleArrayData( dSampleU ).array( &st );

	MDataHandle hSampleV = data.inputValue( aSampleV , &st);
	MObject dSampleV = hSampleV.data();
	const MDoubleArray sampleV = MFnDoubleArrayData( dSampleV ).array( &st );

	MIntArray indices;
	cImgUtils::sampleNearest( *image, sampleU, sampleV, indices);

	MDataHandle hOutIndex = data.outputValue(aOutIndex);
	MFnIntArrayData fnOutIndex;
	MObject dOutIndex = fnOutIndex.create(indices);
	hOutIndex.set(dOutIndex);
	st = data.setClean( aOutIndex);

	return MS::kSuccess;

}
