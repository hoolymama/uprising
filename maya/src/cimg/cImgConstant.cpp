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
#include "cImgConstant.h"
#include "jMayaIds.h"

MTypeId     cImgConstant::id( k_cImgConstant );

MObject 	cImgConstant::aColor;
MObject 	cImgConstant::aResolution;

MObject 	cImgConstant::aOutput;


cImgConstant::cImgConstant() {}

cImgConstant::~cImgConstant() {}

void *cImgConstant::creator()
{
	return new cImgConstant();
}

MStatus cImgConstant::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;

	aColor = nAttr.createColor( "color", "c" );
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	st = addAttribute( aColor ); mser;


	aResolution = nAttr.create("resolution", "rrg", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aResolution);



	aOutput = tAttr.create("output", "out", cImgData::id);
	tAttr.setReadable(true);
	tAttr.setStorable(false);
	addAttribute(aOutput);

	st = attributeAffects(aColor, aOutput ); mser;
	st = attributeAffects(aResolution, aOutput ); mser;


	return MS::kSuccess;
}

MStatus cImgConstant::compute( const MPlug &plug, MDataBlock &data ) {

	if ( plug != aOutput) { return ( MS::kUnknownParameter ); }

	MStatus st = MS::kSuccess;

	const int2 &res = data.inputValue( aResolution ).asInt2();

	int xres = std::max(std::min(res[0], 8192), 1)  ;
	int yres = std::max(std::min(res[1], 8192), 1)  ;


	MDataHandle hOutput = data.outputValue(aOutput);
	MFnPluginData fnOut;
	MTypeId kdid(cImgData::id);
	MObject dOut = fnOut.create(kdid , &st ); mser;

	cImgData *newData = (cImgData *)fnOut.data(&st); mser;

	CImg<unsigned char> *image = newData->fImg;


	MFloatVector &color = data.inputValue(aColor).asFloatVector();

	unsigned char cx = (unsigned char)(fmax(fmin(color.x, 1.0), 0.0) * 255);
	unsigned char cy = (unsigned char)(fmax(fmin(color.y, 1.0), 0.0) * 255);
	unsigned char cz = (unsigned char)(fmax(fmin(color.z, 1.0), 0.0) * 255);
	const unsigned char uc_col[3] = { cx, cy, cz};
	image->assign(xres, yres, 1, 3);
	image->draw_rectangle(0, 0, 0, xres, yres, 1, uc_col);


	hOutput.set(newData);
	data.setClean( plug );
	return MS::kSuccess;
}
