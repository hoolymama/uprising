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
#include "cImgExpr.h"
#include "jMayaIds.h"

MTypeId     cImgExpr::id( k_cImgExpr );

MObject 	cImgExpr::aExpression;
MObject 	cImgExpr::aResolution;
MObject 	cImgExpr::aOutput;
MObject 	cImgExpr::aNormalize;
MObject 	cImgExpr::aSetRange;



cImgExpr::cImgExpr() {}

cImgExpr::~cImgExpr() {}

void *cImgExpr::creator()
{
	return new cImgExpr();
}

MStatus cImgExpr::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	aExpression = tAttr.create("expression", "ex", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setUsedAsFilename(true);
	addAttribute( aExpression );

	aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolution);


	aNormalize = nAttr.create( "normalize", "nrm", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aNormalize); mser;

	aSetRange = nAttr.create("setRange", "rng", MFnNumericData::k2Float);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aSetRange);





	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);


	st = attributeAffects(aNormalize, aOutput ); mser;
	st = attributeAffects(aSetRange, aOutput ); mser;

	st = attributeAffects(aExpression, aOutput ); mser;
	st = attributeAffects(aResolution, aOutput ); mser;

	return MS::kSuccess;
}

MStatus cImgExpr::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;


	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	CImg<unsigned char> *image = newData->fImg;



	const float2 &range = data.inputValue( aSetRange ).asFloat2();
	float minVal = range[0];
	float maxVal = range[1];


	bool normalize = data.inputValue(aNormalize).asBool();

	const int2 &res = data.inputValue( aResolution ).asInt2();
	MString expr = data.inputValue(aExpression).asString();
	const CImg<float> float_img( res[0], res[0], 1, 1, expr.asChar(), true);

	if (normalize) {
		image->assign(float_img.get_normalize(minVal, maxVal));
	}
	else {
		image->assign(float_img);
	}



	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}
