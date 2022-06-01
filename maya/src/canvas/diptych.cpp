

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MFnMatrixData.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnMatrixAttribute.h>

#include "diptych.h"

#include <jMayaIds.h>
#include <attrUtils.h>

#include "mayaMath.h"

#include "dotData.h"
#include "dotTree.h"
#include "cImgData.h"
#include "cImgUtils.h"
#include "nodeUtils.h"

MObject diptych::aBoardHeight;

MObject diptych::aGap;

MObject diptych::aApplyCrop;
MObject diptych::aCropCorner;
MObject diptych::aCropResolution;
MObject diptych::aImageResolution;

MObject diptych::aOutBoardOffset;
MObject diptych::aOutBoardSize;

MObject diptych::aOutSquareOffset;
MObject diptych::aOutSquareSize;

// MObject diptych::aOutCropFactor;
// MObject diptych::aOutCropOffsetFactor;

MObject diptych::aSquareColor;
MObject diptych::aBoardColor;
MObject diptych::aMirror;


MObject diptych::aOutSquareMatrix;


MTypeId diptych::id(k_diptych);
MString diptych::drawDbClassification("drawdb/geometry/diptych");
MString diptych::drawRegistrantId("diptychPlugin");

diptych::diptych() {}

diptych::~diptych() {}

void *diptych::creator()
{
	return new diptych();
}

MStatus diptych::initialize()
{
	MStatus st;
	MString method("diptych::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnMatrixAttribute mAttr;

	aBoardHeight = nAttr.create("boardHeight", "bdh", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aBoardHeight);

	aGap = nAttr.create("gap", "gap", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aGap);

	aApplyCrop = nAttr.create("applyCrop", "apc", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	st = addAttribute(aApplyCrop);

	aCropCorner = nAttr.create("cropCorner", "ccr", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aCropCorner);

	aCropResolution = nAttr.create("cropResolution", "crs", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aCropResolution);

	aImageResolution = nAttr.create("imageResolution", "imr", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aImageResolution);

	// OUT

	aOutBoardOffset = nAttr.create("outBoardOffset", "obo", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutBoardOffset);

	aOutBoardSize = nAttr.create("outBoardSize", "obs", MFnNumericData::k2Float);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutBoardSize);

	aOutSquareOffset = nAttr.create("outSquareOffset", "oso", MFnNumericData::k2Float);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutSquareOffset);

	aOutSquareSize = nAttr.create("outSquareSize", "oss", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);
	nAttr.setKeyable(false);
	st = addAttribute(aOutSquareSize);

  	aOutSquareMatrix = mAttr.create( "outSquareMatrix", "osm",  MFnMatrixAttribute::kFloat );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setReadable( true );
	addAttribute(aOutSquareMatrix);


	// aOutCropFactor = nAttr.create("outCropFactor", "ocf", MFnNumericData::kFloat);
	// nAttr.setHidden(false);
	// nAttr.setStorable(false);
	// nAttr.setReadable(true);
	// nAttr.setWritable(false);
	// nAttr.setKeyable(false);
	// st = addAttribute(aOutCropFactor);

	// aOutCropOffsetFactor = nAttr.create("outCropOffsetFactor", "oof", MFnNumericData::kFloat);
	// nAttr.setHidden(false);
	// nAttr.setStorable(false);
	// nAttr.setReadable(true);
	// nAttr.setWritable(false);
	// nAttr.setKeyable(false);
	// st = addAttribute(aOutCropOffsetFactor);

	// DRAW

	aSquareColor = nAttr.createColor("squareColor", "sc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aSquareColor);
	mser;

	aBoardColor = nAttr.createColor("boardColor", "bc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aBoardColor);
	mser;

	aMirror = nAttr.create("mirror", "mir", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aMirror);

	st = attributeAffects(aBoardHeight, aOutBoardOffset);
	st = attributeAffects(aGap, aOutBoardOffset);
	st = attributeAffects(aImageResolution, aOutBoardOffset);

	st = attributeAffects(aBoardHeight, aOutBoardSize);
	st = attributeAffects(aImageResolution, aOutBoardSize);

	st = attributeAffects(aBoardHeight, aOutSquareOffset);
	st = attributeAffects(aGap, aOutSquareOffset);
	st = attributeAffects(aImageResolution, aOutSquareOffset);
	st = attributeAffects(aApplyCrop, aOutSquareOffset);
	st = attributeAffects(aCropCorner, aOutSquareOffset);
	st = attributeAffects(aCropResolution, aOutSquareOffset);
	st = attributeAffects(aImageResolution, aOutSquareOffset);

	st = attributeAffects(aBoardHeight, aOutSquareSize);
	st = attributeAffects(aImageResolution, aOutSquareSize);
	st = attributeAffects(aApplyCrop, aOutSquareSize);
	st = attributeAffects(aCropCorner, aOutSquareSize);
	st = attributeAffects(aCropResolution, aOutSquareSize);
	st = attributeAffects(aImageResolution, aOutSquareSize);

	st = attributeAffects(aBoardHeight, aOutSquareMatrix);
	st = attributeAffects(aGap, aOutSquareMatrix);
	st = attributeAffects(aImageResolution, aOutSquareMatrix);
	st = attributeAffects(aApplyCrop, aOutSquareMatrix);
	st = attributeAffects(aCropCorner, aOutSquareMatrix);
	st = attributeAffects(aCropResolution, aOutSquareMatrix);
	st = attributeAffects(aImageResolution, aOutSquareMatrix);

 
 

	// st = attributeAffects(aApplyCrop, aOutCropFactor);
	// st = attributeAffects(aCropCorner, aOutCropFactor);
	// st = attributeAffects(aCropResolution, aOutCropFactor);
	// st = attributeAffects(aImageResolution, aOutCropFactor);

	// st = attributeAffects(aApplyCrop, aOutCropOffsetFactor);
	// st = attributeAffects(aCropCorner, aOutCropOffsetFactor);
	// st = attributeAffects(aCropResolution, aOutCropOffsetFactor);
	// st = attributeAffects(aImageResolution, aOutCropOffsetFactor);

	return (MS::kSuccess);
}

MStatus diptych::compute(const MPlug &plug, MDataBlock &data)
{

	MStatus st;
	if (!(
			plug == aOutBoardOffset ||
			plug == aOutBoardSize ||
			plug.parent() == aOutBoardSize ||
			plug == aOutSquareOffset ||
			plug.parent() == aOutSquareOffset ||
			plug == aOutSquareSize ||
			plug == aOutSquareMatrix
			// plug == aOutCropFactor ||
			// plug == aOutCropOffsetFactor ||
			// plug.parent() == aOutCropOffsetFactor
			))
	{
		return (MS::kUnknownParameter);
	}

	MMatrix wm = NodeUtils::firstWorldMatrix(thisMObject());

	float boardHeight = data.inputValue(aBoardHeight).asFloat();
	float gap = data.inputValue(aGap).asFloat();
	const int2 &imageResolution = data.inputValue(aImageResolution).asInt2();
	int xres = imageResolution[0];
	int yres = imageResolution[1];
	int fullSquareRes = std::max(xres, yres);

	float xAspect = (float(xres) / yres);

	float outBoardSizeX = boardHeight * xAspect / 2.0f;
	float outBoardSizeY = boardHeight / 2.0f;
	float outBoardOffset = ((boardHeight * xAspect) + gap) / 2.0f;

	float outSquareSize = outBoardSizeY;
	float outSquareOffsetX = outBoardOffset;
	float outSquareOffsetY = 0.0f;

	if (data.inputValue(aApplyCrop).asBool())
	{

		const int2 &cropCorner = data.inputValue(aCropCorner).asInt2();
		int cropResolution = data.inputValue(aCropResolution).asInt();

		float boardScale = boardHeight / yres;

		float letterBoxPad = (yres - xres) * 0.5;
		outSquareSize = (cropResolution * boardHeight * 0.5f) / yres;

		float halfGap = gap * 0.5;
		float halfCropRes = cropResolution * 0.5;

		outSquareOffsetX = halfGap + (boardScale * ((halfCropRes + cropCorner[0])));

		float yCenter = (halfCropRes + cropCorner[1]) * boardScale;

		outSquareOffsetY = outBoardSizeY - yCenter;
	}


	MFloatMatrix squareMat;
	squareMat.setToIdentity();
	squareMat[0][0] = outSquareSize; 
	squareMat[1][1] = outSquareSize;
	squareMat[3][0] = outSquareOffsetX;
	squareMat[3][1] = outSquareOffsetY;
	
	MDataHandle hOutSquareMatrix = data.outputValue(aOutSquareMatrix);
	hOutSquareMatrix.set(squareMat);
	hOutSquareMatrix.setClean();


	MDataHandle hBoardOffset = data.outputValue(aOutBoardOffset);

	hBoardOffset.set(outBoardOffset);

	hBoardOffset.setClean();

	MDataHandle hBoardSize = data.outputValue(aOutBoardSize);
	float2 &outBoardSize = hBoardSize.asFloat2();
	outBoardSize[0] = outBoardSizeX;
	outBoardSize[1] = outBoardSizeY;
	hBoardSize.setClean();

	MDataHandle hSquareOffset = data.outputValue(aOutSquareOffset);
	float2 &outSquareOffset = hSquareOffset.asFloat2();
	outSquareOffset[0] = outSquareOffsetX;
	outSquareOffset[1] = outSquareOffsetY;
	hSquareOffset.setClean();

	MDataHandle hSquareSize = data.outputValue(aOutSquareSize);
	hSquareSize.set(outSquareSize);
	hSquareSize.setClean();

	return MS::kSuccess;
}

bool diptych::isBounded() const
{
	return false;
}

MBoundingBox diptych::boundingBox() const
{
	return MBoundingBox(MPoint(-1.0, -1.0, 0.0), MPoint(1.0, 1.0, 0.0));
}

void diptych::postConstructor()
{
}
