#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFloatMatrix.h>

#include <maya/MFloatArray.h>

#include <maya/MPlugArray.h>

#include "errorMacros.h"
#include "cImgData.h"
#include "skGraphImage.h"
#include "jMayaIds.h"
#include "cImgFloatData.h"
#include "cImgUtils.h"

#include "skGraph.h"

MTypeId     skGraphImage::id( k_skGraphImage );

MObject 	skGraphImage::aSkeletonImage;
MObject skGraphImage::aPruneLength;


MObject 	skGraphImage::aMaxRadius;
MObject 	skGraphImage::aOutput;


skGraphImage::skGraphImage() {}

skGraphImage::~skGraphImage() {}

void *skGraphImage::creator()
{
	return new skGraphImage();
}

MStatus skGraphImage::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aSkeletonImage = tAttr.create("skeletonImage", "simg", cImgFloatData::id ) ;
	tAttr.setStorable(false);
	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute( aSkeletonImage ); mser;

	aPruneLength = nAttr.create("pruneLength", "prl", MFnNumericData::kInt);
	nAttr.setDefault( 2 );
	nAttr.setKeyable( true );
	st = addAttribute( aPruneLength ); mser


	aMaxRadius = nAttr.create( "maxRadius", "mxr", MFnNumericData::kInt);
	nAttr.setHidden(false);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	st = addAttribute(aMaxRadius);

	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);


	st = attributeAffects(aPruneLength, aOutput ); mser;
	st = attributeAffects(aSkeletonImage, aOutput ); mser;
	st = attributeAffects(aMaxRadius, aOutput ); mser;

	return MS::kSuccess;
}

// CImg<float> *skGraphImage::getImage(MDataBlock &data, MObject &attribute )
// {
// 	MStatus st;
// 	MDataHandle hImageData = data.inputValue(attribute, &st);
// 	if (st.error()) {	return 0;}
// 	MObject dImageData = hImageData.data();
// 	MFnPluginData fnImageData( dImageData , &st);
// 	if (st.error()) {	return 0;}
// 	cImgFloatData *imageData = (cImgFloatData *)fnImageData.data();
// 	return imageData->fImg;
// }



MStatus skGraphImage::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	int maxRadius = data.inputValue(aMaxRadius).asInt();


	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;
	cImgData *newData = (cImgData *)fnOut.data(&st); mser;
	CImg<unsigned char> *image = newData->fImg;


	int minBranchLength =  data.inputValue(aPruneLength).asInt();

	// pointer to skel image
	CImg<float>  *pImage = cImgUtils::getFloatImage(data, skGraphImage::aSkeletonImage );

	skGraph g(pImage);
	g.prune(minBranchLength);
	g.draw(*image);

	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}
