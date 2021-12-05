// #include  <queue>
#define cimg_plugin "plugins/skeleton.h"

#include <maya/MFnPluginData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnEnumAttribute.h>
#include "errorMacros.h"
#include "cImgData.h"
#include "cImgFloatData.h"

#include "cImgSkel.h"
#include "jMayaIds.h"



// #include "CImg.h"
// using namespace cimg_library;
// #include "plugins/skeleton.h"


MTypeId     cImgSkel::id( k_cImgSkel );



// MObject cImgSkel::aScale;
// MObject cImgSkel::aAmplitude;
// MObject cImgSkel::aInterpolation;
// MObject cImgSkel::aType;
MObject cImgSkel::aInput;

MObject cImgSkel::aThreshold;
MObject cImgSkel::aSoftThreshold;
MObject cImgSkel::aStrictThreshold;

MObject cImgSkel::aMedian;
MObject cImgSkel::aInvert;
MObject cImgSkel::aSkeletonThreshold;
MObject cImgSkel::aCreateCurve;
MObject cImgSkel::aCorrection;
MObject cImgSkel::aDiscreteStep;



MObject cImgSkel::aOutBinaryImage;
// MObject cImgSkel::aOutDistanceImage;
// MObject cImgSkel::aOutFluxImage;
MObject cImgSkel::aOutSkeleton;



cImgSkel::cImgSkel() {}

cImgSkel::~cImgSkel() {}

void *cImgSkel::creator()
{
	return new cImgSkel();
}

MStatus cImgSkel::initialize()
{
	MStatus st;

	// inheritAttributesFrom("cImgProcess");
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	// MFnEnumAttribute eAttr;

	aInput = tAttr.create("input", "in", cImgData::id ) ;
	tAttr.setReadable(false);
	st = addAttribute( aInput ); mser;


	aSoftThreshold = nAttr.create( "softThreshold", "sft", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aSoftThreshold);

	aStrictThreshold = nAttr.create( "strictThreshold", "stt", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aStrictThreshold);


	aThreshold = nAttr.create( "threshold", "thr", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1);
	st = addAttribute(aThreshold);


	aMedian = nAttr.create( "median", "med", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1);
	st = addAttribute(aMedian);

	aInvert = nAttr.create( "invert", "inv", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aInvert);

	aSkeletonThreshold = nAttr.create( "skeletonThreshold", "sthr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(0.3f);
	st = addAttribute(aSkeletonThreshold);

	aCreateCurve = nAttr.create( "createCurve", "ccv", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aCreateCurve);



	aCorrection = nAttr.create( "correction", "crc", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(false);
	st = addAttribute(aCorrection);

	aDiscreteStep = nAttr.create( "discreteStep", "dst", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aDiscreteStep);





	aOutBinaryImage = tAttr.create("outBinaryImage", "obi", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutBinaryImage);


	// aOutDistanceImage = tAttr.create("outDistanceImage", "odi", cImgData::id);
	// tAttr.setReadable(true);
	// tAttr.setStorable(false);
	// addAttribute(aOutDistanceImage);

	// aOutFluxImage = tAttr.create("outFluxImage", "ofi", cImgData::id);
	// tAttr.setReadable(true);
	// tAttr.setStorable(false);
	// addAttribute(aOutFluxImage);



	aOutSkeleton = tAttr.create("outSkeleton", "osk", cImgFloatData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutSkeleton);



	st = attributeAffects( aThreshold, aOutBinaryImage );
	st = attributeAffects( aSoftThreshold, aOutBinaryImage );
	st = attributeAffects( aStrictThreshold, aOutBinaryImage );
	st = attributeAffects(aInput, aOutBinaryImage );
	st = attributeAffects(aMedian, aOutBinaryImage );
	st = attributeAffects(aInvert, aOutBinaryImage );
	st = attributeAffects(aSkeletonThreshold, aOutBinaryImage );
	st = attributeAffects(aCreateCurve, aOutBinaryImage );
	st = attributeAffects(aCorrection, aOutBinaryImage );
	st = attributeAffects(aDiscreteStep, aOutBinaryImage );


	// st = attributeAffects( aThreshold, aOutDistanceImage );
	// st = attributeAffects( aSoftThreshold, aOutDistanceImage );
	// st = attributeAffects( aStrictThreshold, aOutDistanceImage );
	// st = attributeAffects(aInput, aOutDistanceImage );
	// st = attributeAffects(aMedian, aOutDistanceImage );
	// st = attributeAffects(aInvert, aOutDistanceImage );
	// st = attributeAffects(aSkeletonThreshold, aOutDistanceImage );
	// st = attributeAffects(aCreateCurve, aOutDistanceImage );
	// st = attributeAffects(aCorrection, aOutDistanceImage );

	// st = attributeAffects(aDiscreteStep , aOutDistanceImage );
	// st = attributeAffects( aThreshold, aOutFluxImage );
	// st = attributeAffects( aSoftThreshold, aOutFluxImage );
	// st = attributeAffects( aStrictThreshold, aOutFluxImage );
	// st = attributeAffects(aInput, aOutFluxImage );
	// st = attributeAffects(aMedian, aOutFluxImage );
	// st = attributeAffects(aInvert, aOutFluxImage );
	// st = attributeAffects(aSkeletonThreshold, aOutFluxImage );
	// st = attributeAffects(aCreateCurve, aOutFluxImage );
	// st = attributeAffects(aCorrection, aOutFluxImage );
	// st = attributeAffects(aDiscreteStep, aOutFluxImage );



	st = attributeAffects( aThreshold, aOutSkeleton );
	st = attributeAffects( aSoftThreshold, aOutSkeleton );
	st = attributeAffects( aStrictThreshold, aOutSkeleton );
	st = attributeAffects(aInput, aOutSkeleton );
	st = attributeAffects(aMedian, aOutSkeleton );
	st = attributeAffects(aInvert, aOutSkeleton );
	st = attributeAffects(aSkeletonThreshold, aOutSkeleton );
	st = attributeAffects(aCreateCurve, aOutSkeleton );
	st = attributeAffects(aCorrection, aOutSkeleton );
	st = attributeAffects(aDiscreteStep, aOutSkeleton );




	return MS::kSuccess;
}



MStatus cImgSkel::process(
  MDataBlock &data,
  const CImg<unsigned char> &inimage,
  CImg<float> &outSkeleton,
  CImg<unsigned char> &outBinary
  // ,
  // CImg<unsigned char> &outDistance,
  // CImg<unsigned char> &outFlux
)

{
	int threshold = data.inputValue(aThreshold).asInt();
	bool invert = data.inputValue(aInvert).asBool();
	int median = data.inputValue(aMedian).asInt();


	bool softThreshold = data.inputValue(aSoftThreshold).asBool();
	bool strictThreshold = data.inputValue(aStrictThreshold).asBool();


	bool createCurve = data.inputValue(aCreateCurve).asBool();
	float skeletonThreshold = data.inputValue(aSkeletonThreshold).asFloat();
	bool correction = data.inputValue(aCorrection).asBool();
	float dlt2 = data.inputValue(aDiscreteStep).asFloat();
	float dlt1 = 2.0;


	// Make image, with 2 values { 0,1 }).
	CImg<unsigned char> image = inimage.get_norm().threshold(threshold, softThreshold,
	                            strictThreshold).normalize(0.0f, 1.0f).round();
	if (median) { image.blur_median(median); }
	if (invert) { (image -= 1) *= -1; }

	outBinary.assign(image * 255);



	// Compute distance map.
	CImg<float> distance = image.get_distance(0);

	// outDistance.assign(distance);

	// Compute the gradient of the distance function, and the flux (divergence) of the gradient field.
	const CImgList<float> grad = distance.get_gradient("xyz");
	CImg<float> flux = image.get_flux(grad, 1, 1);

	// Use Torsello correction of the flux if necessary.
	if (correction) {
		CImg<float>
		logdensity = image.get_logdensity(distance, grad, flux, dlt1),
		nflux = image.get_corrected_flux(logdensity, grad, flux, dlt2);
		flux = nflux;
	}

	// outFlux.assign(flux * 255);
	// Compute the skeleton and multiply by distance to give the medial axias transform (MAT).
	CImg<bool> mask = image.get_skeleton(flux, distance, createCurve, skeletonThreshold);
	int w = distance.width();
	int h = distance.height();
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (! mask(x, y)) {
				distance(x, y) = 0.0;
			}
		}
	}


	outSkeleton.assign(distance) ;




	// outSkeleton = outSkeleton * distance;



	return MS::kSuccess;

}


MStatus cImgSkel::compute( const MPlug &plug, MDataBlock &data ) {

	if (!  (plug == aOutSkeleton
	        || plug == aOutBinaryImage
	        // || plug == aOutDistanceImage
	        // || plug == aOutFluxImage
	       )		) { return ( MS::kUnknownParameter ); }


	MStatus st;

	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);
	if (st.error()) {
		return MS::kUnknownParameter;
	}

	cImgData *imageData = (cImgData *)fnImageData.data();
	CImg<unsigned char> *inImage = imageData->fImg;

	MTypeId kdid(cImgData::id);
	MTypeId kdidf(cImgFloatData::id);

	MFnPluginData fnOutSkeleton,  fnOutBinary;
	MObject dOutSkeleton = fnOutSkeleton.create(kdidf , &st ); mser;
	MObject dOutBinary = fnOutBinary.create(kdid , &st ); mser;

	// MObject dOutDistance = fnOutDistance.create(kdid , &st ); mser;
	// MObject dOutFlux = fnOutFlux.create(kdid , &st ); mser;


	cImgFloatData *newOutSkeletonData = (cImgFloatData *)fnOutSkeleton.data(&st); mser;
	CImg<float> *outSkeletonImage = newOutSkeletonData->fImg;

	cImgData *newOutBinaryData = (cImgData *)fnOutBinary.data(&st); mser;
	CImg<unsigned char> *outBinaryImage = newOutBinaryData->fImg;

	// cImgData *newOutDistanceData = (cImgData *)fnOutDistance.data(&st); mser;
	// CImg<unsigned char> *outDistanceImage = newOutDistanceData->fImg;

	// cImgData *newOutFluxData = (cImgData *)fnOutFlux.data(&st); mser;
	// CImg<unsigned char> *outFluxImage = newOutFluxData->fImg;


	process(data, *inImage, *outSkeletonImage, *outBinaryImage
	        /*		, *outDistanceImage
	        		, *outFluxImage */
	       );


	MDataHandle hOutput = data.outputValue(aOutSkeleton);
	hOutput.set(newOutSkeletonData);
	hOutput.setClean();

	MDataHandle hOutBinary = data.outputValue(aOutBinaryImage);
	hOutBinary.set(newOutBinaryData);
	hOutBinary.setClean();

	// MDataHandle hOutDistance = data.outputValue(aOutDistanceImage);
	// hOutDistance.set(newOutDistanceData);
	// hOutDistance.setClean();

	// MDataHandle hOutFlux = data.outputValue(aOutFluxImage);
	// hOutFlux.set(newOutFluxData);
	// hOutFlux.setClean();

	return MS::kSuccess;
}

