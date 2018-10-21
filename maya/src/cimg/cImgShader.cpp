#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MStatus.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <maya/MFloatVector.h>

#include "errorMacros.h"
#include "jMayaIds.h"

#include "cImgData.h"

#include "cImgShader.h"




MObject cImgShader::aImageData;
MObject cImgShader::aInterpolation;
MObject cImgShader::aUVCoord;
MObject cImgShader::aDefaultColor 	;
MObject cImgShader::aOutColor;
MObject cImgShader::aOutAlpha;


MTypeId cImgShader::id( k_cImgShader );


cImgShader::cImgShader()
{
}

cImgShader::~cImgShader()
{
}


void *cImgShader::creator()
{
	return new cImgShader();
}
void cImgShader::postConstructor( )
{
	setMPSafe(false);
}


MStatus cImgShader::initialize()
{
	MStatus st;




	MString method("cImgShader::initialize");
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnEnumAttribute eAttr;

	aImageData = tAttr.create("imageData", "imgd", cImgData::id ) ;
	tAttr.setStorable(false);
	st = addAttribute( aImageData ); mser;


	MObject c1 = nAttr.create("uCoord", "u", MFnNumericData::kFloat);
	MObject c2 = nAttr.create("vCoord", "v", MFnNumericData::kFloat);
	aUVCoord = nAttr.create("uvCoord", "uv", c1, c2);
	nAttr.setKeyable(true);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setHidden(true);
	addAttribute(aUVCoord);



	aInterpolation = eAttr.create("interpolation", "itp");
	eAttr.addField("Nearest", cImgData::kNearest);
	eAttr.addField("Bilinear", cImgData::kBilinear);
	eAttr.addField("Bicubic", cImgData::kBicubic);

	eAttr.setDefault( cImgData::kBilinear );
	eAttr.setKeyable(true);
	eAttr.setWritable(true);
	addAttribute(aInterpolation);


	aDefaultColor = nAttr.createColor( "defaultColor", "dc" );
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	st = addAttribute( aDefaultColor ); mser;

	aOutColor = nAttr.createColor( "outColor", "oc" );
	nAttr.setWritable( false );
	nAttr.setKeyable( false );
	nAttr.setReadable( true);
	nAttr.setStorable( false);
	st = addAttribute( aOutColor ); mser;


	aOutAlpha = nAttr.create( "outAlpha", "oa", MFnNumericData::kFloat);
	nAttr.setKeyable(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	addAttribute(aOutAlpha);



	attributeAffects (aInterpolation, aOutColor);
	attributeAffects (aUVCoord, aOutColor);
	attributeAffects (aImageData, aOutColor);
	attributeAffects (aDefaultColor, aOutColor);

	attributeAffects (aInterpolation, aOutAlpha);
	attributeAffects (aUVCoord, aOutAlpha);
	attributeAffects (aImageData, aOutAlpha);
	attributeAffects (aDefaultColor, aOutAlpha);

	return MS::kSuccess;
}


MStatus cImgShader::compute( const MPlug  &plug,  MDataBlock &data )
{
	MStatus st;


	if (!
	    ((plug == aOutColor) ||
	     (plug.parent() == aOutColor) ||
	     (plug == aOutAlpha))) 	{ return MS::kUnknownParameter; }

	cImgData::Interpolation interp = cImgData::Interpolation(data.inputValue(
	                                   aInterpolation).asShort());


	MFloatVector &defaultColor = data.inputValue(aDefaultColor).asFloatVector();

	MFloatVector resultColor = defaultColor;
	float resultAlpha = 0.0f;

	float2 &uv = data.inputValue(aUVCoord).asFloat2();

	MDataHandle hImageData = data.inputValue(aImageData, &st); msert;
	MObject dImageData = hImageData.data();
	MFnPluginData fnImageData( dImageData , &st);

	float valX = defaultColor.x;
	float valY = defaultColor.y;
	float valZ = defaultColor.z;

	if (! st.error()) {

		cImgData *imageData = (cImgData *)fnImageData.data();
		CImg<unsigned char> *image = imageData->fImg;
		float w = image->width();
		float h = image->height();
		int channels = image->spectrum();

		if (w > 0 && h > 0) {
			float u = uv[0] * w;
			float v = (1.0 - uv[1]) * h;

			if (interp ==  cImgData::kNearest) {
				valX = image->atXY(int(u),  int(v), 0, 0);
				if (channels == 3) {
					valY = image->atXY(int(u),  int(v), 0, 1);
					valZ = image->atXY(int(u),  int(v), 0, 2);
				}
			}
			else if (interp ==  cImgData::kBilinear) {
				valX = image->linear_atXY(u, v, 0, 0);
				if (channels == 3) {
					valY = image->linear_atXY(int(u),  int(v), 0, 1);
					valZ = image->linear_atXY(int(u),  int(v), 0, 2);
				}
			}
			else {   // bicubic
				valX = image->cubic_atXY(u, v, 0, 0);
				if (channels == 3) {
					valY = image->cubic_atXY(int(u),  int(v), 0, 1);
					valZ = image->cubic_atXY(int(u),  int(v), 0, 2);
				}
			}
			if (channels != 3) {
				valY = valX;
				valZ = valX;
			}
			valX = valX / 255.0;
			valY = valY / 255.0;
			valZ = valZ / 255.0;
			resultColor =  MFloatVector(valX, valY, valZ);
		}
	}


	resultAlpha = (valX + valY + valZ ) * 0.33333333;

	///////////////////////////////////////////////////////////////
	MDataHandle outAlphaHandle = data.outputValue( aOutAlpha );
	float &outAlpha = outAlphaHandle.asFloat();
	outAlpha = resultAlpha;
	outAlphaHandle.setClean();

	MDataHandle outColorHandle = data.outputValue( aOutColor );
	MFloatVector &outColor = outColorHandle.asFloatVector();
	outColor = resultColor;
	outColorHandle.setClean();

	return MS::kSuccess;
}