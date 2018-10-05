#include <maya/MIOStream.h>
#include <math.h>
#include <map>

#include <maya/MStatus.h>


#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MColor.h>
#include <maya/MFloatVector.h>


#include "mayaMath.h"
#include "errorMacros.h"
#include "jMayaIds.h"

#include "indexShader.h"

MObject indexShader::aColor;
MObject indexShader::aInputs;
MObject indexShader::aOutColor;
MObject indexShader::aOutAlpha;
MObject indexShader::aUVCoord;
MObject indexShader::aFilterSize;
MObject indexShader::aColorSpace;


MTypeId indexShader::id( k_indexShader );


indexShader::indexShader()
{
}

indexShader::~indexShader()
{
}


void *indexShader::creator()
{
	return new indexShader();
}
void indexShader::postConstructor( )
{
	setMPSafe(false);
}


MStatus indexShader::initialize()
{
	MStatus st;

	MFnNumericAttribute nAttr;

	MFnEnumAttribute eAttr;

	aInputs = nAttr.createColor("inputs", "ins" ) ;
	nAttr.setDefault(0.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setArray(true);
	nAttr.setIndexMatters(true);
	st = addAttribute( aInputs ); mser;

	aColor = nAttr.createColor( "color", "c" );
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	st = addAttribute( aColor ); mser;



	MObject child1 = nAttr.create( "uCoord", "u", MFnNumericData::kFloat);
	MObject child2 = nAttr.create( "vCoord", "v", MFnNumericData::kFloat);
	aUVCoord = nAttr.create( "uvCoord", "uv", child1, child2);
	nAttr.setStorable(false);
	nAttr.setHidden(true);
	nAttr.setReadable(true);
	nAttr.setWritable(false);


	child1 = nAttr.create( "uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	child2 = nAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	aFilterSize = nAttr.create("uvFilterSize", "fs", child1, child2);
	nAttr.setStorable(false);
	nAttr.setHidden(true);
	nAttr.setReadable(true);
	nAttr.setWritable(false);


	aColorSpace = eAttr.create("colorSpace", "cs", indexShader::kRGB);

	eAttr.addField("None", indexShader::kNone);
	eAttr.addField("RGB", indexShader::kRGB);
	eAttr.addField("hcl",  indexShader::kHCL);
	eAttr.setKeyable(true);
	eAttr.setStorable(true);
	eAttr.setHidden(false);
	st = addAttribute( aColorSpace ); mser;

	// aDefaultColor = nAttr.createColor( "defaultColor", "dc" );
	// nAttr.setDefault(1.0f, 0.0f, 0.0f);
	// nAttr.setKeyable(true);
	// nAttr.setStorable(true);
	// nAttr.setUsedAsColor(true);
	// nAttr.setReadable(true);
	// nAttr.setWritable(true);
	// st = addAttribute( aDefaultColor ); mser;



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


	attributeAffects (aInputs, aOutColor);
	attributeAffects (aColor, aOutColor);
	attributeAffects (aUVCoord, aOutColor);
	attributeAffects (aFilterSize, aOutColor);
	attributeAffects (aColorSpace, aOutColor);

	attributeAffects (aInputs, aOutAlpha);
	attributeAffects (aColor, aOutAlpha);
	attributeAffects (aUVCoord, aOutAlpha);
	attributeAffects (aFilterSize, aOutAlpha);
	attributeAffects (aColorSpace, aOutAlpha);
	return MS::kSuccess;
}


MStatus indexShader::compute( const MPlug      &plug,  MDataBlock &data )
{
	MStatus st;

	if ((plug != aOutColor) && (plug.parent() != aOutColor) && (plug != aOutAlpha)) {
		return MS::kUnknownParameter;
	}



	MFloatVector &color = data.inputValue(aColor).asFloatVector();
	float resultAlpha = 0.0f;
	MFloatVector resultColor = color;

	ColorSpace space = ColorSpace(data.inputValue(aColorSpace).asShort());
	if (space != indexShader::kNone) {


		MFloatVector colorInSpace = color;
		if (space == indexShader::kHCL) {
			colorInSpace = mayaMath::rgb2hcl(MColor(color.x, color.y, color.z));
		}



		float dist = 9999;

		// std::map<int, MFloatVector> indexedColors;
		// cerr << "-----------------------------: " << endl;
		MArrayDataHandle hInputs = data.inputValue(aInputs);
		unsigned nPlugs = hInputs.elementCount();
		int index = -1;
		for (int i = 0; i < nPlugs; i++, hInputs.next()) {
			index = hInputs.elementIndex(&st);
			if (st.error()) {
				continue;
			}
			MDataHandle hInput = hInputs.inputValue(&st);
			if (st.error()) {
				continue;
			}
			MFloatVector indexColor = hInput.asFloatVector();

			MFloatVector indexColorInSpace = indexColor;
			if (space == indexShader::kHCL) {
				indexColorInSpace =  mayaMath::rgb2hcl(MColor(indexColor.x, indexColor.y, indexColor.z));
			}


			float currDist = (colorInSpace - indexColorInSpace).length();
			// cerr << "RGB: " << color << " <> " << indexColor << ",  InSpace: " << colorInSpace <<" <> " <<		       indexColorInSpace << endl;
			if (currDist < dist) {
				dist = currDist;
				resultColor = indexColor;
				resultAlpha = float(index) + 0.5;
				// cerr << "RESULT: " <<  resultColor << "indexCol: " << indexColor <<  endl;
			}

		}

		resultAlpha = resultAlpha / (float(index) + 1.0);
	}

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