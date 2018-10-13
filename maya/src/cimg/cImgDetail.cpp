#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "cImgDetail.h"
#include "jMayaIds.h"

MTypeId     cImgDetail::id( k_cImgDetail );

// MObject 	cImgDetail::aImageFilename;
// MObject 	cImgDetail::aResize;
MObject 	cImgDetail::aBlur;

// MObject 	cImgDetail::aOutput;


// MObject 	cImgDetail::aInput;

cImgDetail::cImgDetail() {}

cImgDetail::~cImgDetail() {}

void *cImgDetail::creator()
{
	return new cImgDetail();
}

MStatus cImgDetail::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	// MFnTypedAttribute tAttr;
	// MFnUnitAttribute uAttr;
	// MFnEnumAttribute eAttr;

	inheritAttributesFrom("cImgProcess");

	aBlur = nAttr.create( "blur", "bvlr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aBlur);

	// aImageFilename = tAttr.create("imageFilename", "im", MFnData::kString);
	// tAttr.setStorable(true);
	// tAttr.setUsedAsFilename(true);
	// addAttribute( aImageFilename );


	// aResize = nAttr.create( "resize", "rsz", MFnNumericData::kBoolean);
	// nAttr.setHidden(false);
	// nAttr.setStorable(true);
	// nAttr.setReadable(true);
	// nAttr.setDefault(true);
	// st = addAttribute(aResize);

	// aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	// nAttr.setStorable(true);
	// nAttr.setKeyable(true);
	// st = addAttribute(aResolution);


	st = attributeAffects(aBlur, aOutput ); mser;
	// st = attributeAffects(aResolution, aOutput ); mser;

	return MS::kSuccess;
}


MStatus cImgDetail::process(MDataBlock &data, const CImg<unsigned char> &image,
                            CImg<unsigned char> &result)

{


	float blur = data.inputValue(aBlur).asFloat();

	CImgList<float> grad = image.get_gradient();


	result.assign(sqrt(grad[0].pow(2) + grad[1].pow(2)).normalize(0,
	              255).blur(blur).normalize(0, 255));

	return MS::kSuccess;

}



// MStatus cImgDetail::compute( const MPlug &plug, MDataBlock &data ) {

// 	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

// 	MStatus st = MS::kSuccess;

// 	MDataHandle hImageData = data.inputValue(aInput, &st); msert;
// 	MObject dImageData = hImageData.data();
// 	MFnPluginData fnImageData( dImageData , &st);
// 	if (! st.error()) {

// 		cImgData *imageData = (cImgData *)fnImageData.data();
// 		CImg<unsigned char> *image = imageData->fImg;

// 		// for now,, just invert the image
// 		((*image) *= -1) += 255;



// 		MDataHandle hOutput = data.outputValue(aOutput);
// 		MFnPluginData fnOut;
// 		MTypeId kdid(cImgData::id);
// 		MObject dOut = fnOut.create(kdid , &st ); mser;

// 		cImgData *newData = (cImgData *)fnOut.data(&st); mser;

// 		CImg<unsigned char> *outimage = newData->fImg;

// 		outimage->assign(*image);

// 		hOutput.set(newData);
// 		data.setClean( plug );
// 		return MS::kSuccess;
// 	}
// 	return MS::kUnknownParameter;

// 	// hOutput.set(newData);
// 	// data.setClean( plug );
// 	// return MS::kSuccess;
// }
