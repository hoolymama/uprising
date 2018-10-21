#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgIndex.h"
#include "jMayaIds.h"

MTypeId     cImgIndex::id( k_cImgIndex );


MObject 	cImgIndex::aDithering;

MObject 	cImgIndex::aMapIndices;

// MObject cImgIndex::aSampleRangeMin;
// MObject cImgIndex::aSampleRangeMax;
// MObject cImgIndex::aSampleRange;
// MObject cImgIndex::aSampleRamp;
// MObject cImgIndex::aSamples;


cImgIndex::cImgIndex() {}

cImgIndex::~cImgIndex() {}

void *cImgIndex::creator()
{
	return new cImgIndex();
}

MStatus cImgIndex::initialize()
{
	MStatus st;



	inheritAttributesFrom("cImgProcess");

	MFnNumericAttribute nAttr;

	aDithering = nAttr.create( "dithering", "dth", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aDithering);


	aMapIndices = nAttr.create( "mapIndices", "mpi", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aMapIndices);



	// aSampleRamp  = MRampAttribute::createCurveRamp("sampleRamp", "srmp");
	// st = addAttribute( aSampleRamp ); mser;

	// aSampleRangeMin = nAttr.create( "sampleRangeMin", "srn", MFnNumericData::kDouble);
	// nAttr.setDefault(0.0);
	// aSampleRangeMax = nAttr.create( "sampleRangeMax", "srx", MFnNumericData::kDouble );
	// nAttr.setDefault(0.0);
	// aSampleRange = nAttr.create("sampleRange", "sr", aSampleRangeMin,  aSampleRangeMax);
	// nAttr.setHidden( false );
	// nAttr.setKeyable( true );
	// st = addAttribute(aSampleRange); mser;

	// aSamples = nAttr.create( "samples", "smp", MFnNumericData::kInt);
	// nAttr.setHidden(false);
	// nAttr.setStorable(true);
	// nAttr.setReadable(true);
	// nAttr.setKeyable(true);
	// nAttr.setDefault(8);
	// st = addAttribute(aSamples);

	// st = attributeAffects(aSampleRamp, aOutput );
	// // st = attributeAffects(aSampleRange, aOutput );
	// st = attributeAffects(aSamples, aOutput );


	st = attributeAffects(aDithering, aOutput );
	st = attributeAffects(aMapIndices, aOutput );

	return MS::kSuccess;
}


MStatus cImgIndex::process(MDataBlock &data, const CImg<unsigned char> &image,
                           CImg<unsigned char> &result)

{

	// MObject thisObj = thisMObject();
	// MRampAttribute rampAttr( thisObj, aSampleRamp ); mser;

	// int nSamples = data.inputValue(aSamples).asInt();
	// if (nSamples < 2) {
	// 	nSamples = 2;
	// }
	// float gap = 1.0f / nSamples - 1;


	// unsigned char *buffer = new unsigned char[nSamples];
	// for (int i = 0; i < nSamples; ++i)
	// {
	// 	// unsigned char *result = buffer + size_t(i);

	// 	float rpos = (gap * i);
	// 	float val;
	// 	rampAttr.getValueAtPosition( rpos, val, &st ); mser;
	// 	val = std::max(0.0f, std::min(val, 1.0f));
	// 	buffer[i] = unsigned char(val * 255);
	// }






	bool dithering = data.inputValue(aDithering).asBool();
	bool mapIndices = !data.inputValue(aMapIndices).asBool();




	int spectrum = image.spectrum();
	if (image.spectrum() == 1) {
		CImg<unsigned char> colormap(8, 1, 1, 1, 0, 32, 64, 96, 128, 160, 192, 224);
		result = image.get_index(colormap, dithering, mapIndices);
		return MS::kSuccess;
	}

	CImg<unsigned char> colormap(8, 1, 1, 3,
	                             0, 32, 64, 96, 128, 160, 192, 224,
	                             0,  0,  0,  0,   0,   0,   0,   0,
	                             0, 32, 64, 96, 128, 160, 192, 224);
	result = image.get_index(colormap, dithering, mapIndices);


	return MS::kSuccess;

}
