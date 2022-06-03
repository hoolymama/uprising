

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

MObject diptych::aPinOffset;

MObject diptych::aPin0;
MObject diptych::aPinX;
MObject diptych::aPinY;
MObject diptych::aPinPainting;


MObject diptych::aSquareColor;
MObject diptych::aBoardColor;
MObject diptych::aPinColor;
MObject diptych::aMirror;
MObject diptych::aPaintingLocatorSize;

MObject diptych::aDisplaySquareMatrix;
MObject diptych::aDisplayBoardMatrix;
MObject diptych::aDisplayPinMatrix;
MObject diptych::aDisplayPaintingMatrix;



MObject diptych::aOutSquareMatrix;
MObject diptych::aOutBoardMatrix;
MObject diptych::aOutPinMatrix;
MObject diptych::aOutPaintingMatrix;


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

	aPinOffset = nAttr.create("pinOffset", "pof", MFnNumericData::k2Float);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aPinOffset);


	aPin0 = nAttr.createPoint("pin0", "pn0");
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aPin0);

	aPinX = nAttr.createPoint("pinX", "pnx");
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aPinX);

	aPinY = nAttr.createPoint("pinY", "pny");
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	addAttribute(aPinY);

	aPinPainting = nAttr.create("pinPainting", "ppt", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	addAttribute(aPinPainting);

 


	// OUTPUTS
  	aOutSquareMatrix = mAttr.create( "squareMatrix", "sqmt",  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	addAttribute(aOutSquareMatrix);
	mAttr.setHidden( false );
	mAttr.setReadable( true );

  	aOutBoardMatrix = mAttr.create( "boardMatrix", "bdmt",  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setReadable( true );
	addAttribute(aOutBoardMatrix);

  	aOutPinMatrix = mAttr.create( "pinMatrix", "pnmt",  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setReadable( true );
	addAttribute(aOutPinMatrix);

  	aOutPaintingMatrix = mAttr.create( "paintingMatrix", "ptmt",  MFnMatrixAttribute::kDouble );
	mAttr.setStorable( false );
	mAttr.setHidden( false );
	mAttr.setReadable( true );
	addAttribute(aOutPaintingMatrix);


	// DRAW

	aSquareColor = nAttr.createColor("squareColor", "sc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aSquareColor);


	aBoardColor = nAttr.createColor("boardColor", "bc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aBoardColor);


	aPinColor = nAttr.createColor("pinColor", "pc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aPinColor);


	aMirror = nAttr.create("mirror", "mir", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aMirror);

	aPaintingLocatorSize = nAttr.create("paintingLocatorSize", "ps", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f);
	st = addAttribute(aPaintingLocatorSize);


	aDisplaySquareMatrix= nAttr.create("displaySquareMatrix", "dsq", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplaySquareMatrix);

	aDisplayBoardMatrix= nAttr.create("displayBoardMatrix", "dbo", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayBoardMatrix);

	aDisplayPinMatrix= nAttr.create("displayPinMatrix", "dpi", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayPinMatrix);

	aDisplayPaintingMatrix= nAttr.create("displayPaintingMatrix", "dpa", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayPaintingMatrix);





	st = attributeAffects(aBoardHeight, aOutBoardMatrix);
	st = attributeAffects(aGap, aOutBoardMatrix);
	st = attributeAffects(aImageResolution, aOutBoardMatrix);

	st = attributeAffects(aBoardHeight, aOutSquareMatrix);
	st = attributeAffects(aGap, aOutSquareMatrix);
	st = attributeAffects(aImageResolution, aOutSquareMatrix);
	st = attributeAffects(aApplyCrop, aOutSquareMatrix);
	st = attributeAffects(aCropCorner, aOutSquareMatrix);
	st = attributeAffects(aCropResolution, aOutSquareMatrix);
	st = attributeAffects(aImageResolution, aOutSquareMatrix);

	st = attributeAffects(aBoardHeight, aOutPinMatrix);
	st = attributeAffects(aGap, aOutPinMatrix);
	st = attributeAffects(aImageResolution, aOutPinMatrix);
	st = attributeAffects(aPinOffset, aOutPinMatrix);

	st = attributeAffects(aPin0 , aOutPaintingMatrix);
	st = attributeAffects(aPinX , aOutPaintingMatrix);
	st = attributeAffects(aPinY , aOutPaintingMatrix);
	st = attributeAffects(aPinPainting , aOutPaintingMatrix);




	return (MS::kSuccess);
}

MStatus diptych::compute(const MPlug &plug, MDataBlock &data)
{

	MStatus st;
	if (!(
			plug == aOutSquareMatrix ||
			plug == aOutBoardMatrix ||
			plug == aOutPinMatrix ||
			plug == aOutPaintingMatrix
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

	float halfGap = gap * 0.5;

	if (data.inputValue(aApplyCrop).asBool())
	{

		const int2 &cropCorner = data.inputValue(aCropCorner).asInt2();
		int cropResolution = data.inputValue(aCropResolution).asInt();

		float boardScale = boardHeight / yres;

		float letterBoxPad = (yres - xres) * 0.5;
		outSquareSize = (cropResolution * boardHeight * 0.5f) / yres;

		float halfCropRes = cropResolution * 0.5;

		outSquareOffsetX = halfGap + (boardScale * ((halfCropRes + cropCorner[0])));

		float yCenter = (halfCropRes + cropCorner[1]) * boardScale;

		outSquareOffsetY = outBoardSizeY - yCenter;
	}


	const float2 &pinOffset = data.inputValue(aPinOffset).asFloat2();
	float xPinOff = pinOffset[0];
	float yPinOff = pinOffset[1];


	MMatrix pinMat;
	pinMat.setToIdentity();
	pinMat[0][0] = double(outBoardSizeX + outBoardSizeX + xPinOff + halfGap); 
	pinMat[1][1] = double(outBoardSizeY + yPinOff);

	//// calculate painting matrix
	MMatrix paintingMat;
	paintingMat.setToIdentity();
	bool pinPainting = data.inputValue(aPinPainting).asBool();

	if (pinPainting)
	{
		MFloatVector pin0 = data.inputValue(aPin0).asFloatVector();
		MFloatVector pinX = data.inputValue(aPinX).asFloatVector();
		MFloatVector pinY = data.inputValue(aPinY).asFloatVector();

		MFloatVector nx = (pinX - pin0).normal();
		MFloatVector ny = (pinY - pin0).normal();

		if ( ! (nx.isEquivalent(MVector::zero) || ny.isEquivalent(MVector::zero) || nx.isParallel(ny)))
		{

			MMatrix cornerPinMatrix;

			MVector nz = (nx ^ ny).normal();
			nx = ny ^ nz;
			cornerPinMatrix[0][0] = nx.x;
			cornerPinMatrix[0][1] = nx.y;
			cornerPinMatrix[0][2] = nx.z;
			cornerPinMatrix[1][0] = ny.x;
			cornerPinMatrix[1][1] = ny.y;
			cornerPinMatrix[1][2] = ny.z;
			cornerPinMatrix[2][0] = nz.x;
			cornerPinMatrix[2][1] = nz.y;
			cornerPinMatrix[2][2] = nz.z;
			cornerPinMatrix[3][0] = pin0.x;
			cornerPinMatrix[3][1] = pin0.y;
			cornerPinMatrix[3][2] = pin0.z;	

			paintingMat[3][0] = pinMat[0][0];
			paintingMat[3][1] = pinMat[1][1];
			paintingMat *= cornerPinMatrix;
			
		}
	}


	MMatrix squareMat;
	squareMat.setToIdentity();
	squareMat[0][0] = double(outSquareSize); 
	squareMat[1][1] = double(outSquareSize);
	squareMat[3][0] = double(outSquareOffsetX);
	squareMat[3][1] = double(outSquareOffsetY);
	
	MMatrix boardMat;
	boardMat.setToIdentity();
	boardMat[0][0] = double(outBoardSizeX); 
	boardMat[1][1] = double(outBoardSizeY);
	boardMat[3][0] = double(outBoardOffset);


	MDataHandle hOutSquareMatrix = data.outputValue(aOutSquareMatrix);
	hOutSquareMatrix.set(squareMat);
	hOutSquareMatrix.setClean();

	MDataHandle hOutBoardMatrix = data.outputValue(aOutBoardMatrix);
	hOutBoardMatrix.set(boardMat);
	hOutBoardMatrix.setClean();

	MDataHandle hOutPinMatrix = data.outputValue(aOutPinMatrix);
	hOutPinMatrix.set(pinMat);
	hOutPinMatrix.setClean();

	MDataHandle hOutPaintingMatrix = data.outputValue(aOutPaintingMatrix);
	hOutPaintingMatrix.set(paintingMat);
	hOutPaintingMatrix.setClean();

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
