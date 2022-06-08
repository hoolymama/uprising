

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
#include <maya/MFnMeshData.h>

#include <maya/MFnMesh.h>

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

MObject diptych::aNumProbes;
MObject diptych::aProbeBorder;
MObject diptych::aProbePointSize;
MObject diptych::aDisplayProbes;
MObject diptych::aDisplacement;

// DISPLAY
MObject diptych::aSquareColor;
MObject diptych::aBoardColor;
MObject diptych::aPinColor;
MObject diptych::aProbeColor;

MObject diptych::aMirror;
MObject diptych::aPaintingLocatorSize;

MObject diptych::aDisplaySquareMatrix;
MObject diptych::aDisplayBoardMatrix;
MObject diptych::aDisplayPinMatrix;
MObject diptych::aDisplayPaintingMatrix;

MObject diptych::aOutMesh;

MObject diptych::aOutSquareMatrix;
MObject diptych::aOutBoardMatrix;
MObject diptych::aOutPinMatrix;
MObject diptych::aOutPaintingMatrix;

MObject diptych::aOutProbePoints;


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

	aOutSquareMatrix = mAttr.create("squareMatrix", "sqmt", MFnMatrixAttribute::kDouble);
	mAttr.setStorable(false);
	addAttribute(aOutSquareMatrix);
	mAttr.setHidden(false);
	mAttr.setReadable(true);

	aOutBoardMatrix = mAttr.create("boardMatrix", "bdmt", MFnMatrixAttribute::kDouble);
	mAttr.setStorable(false);
	mAttr.setHidden(false);
	mAttr.setReadable(true);
	addAttribute(aOutBoardMatrix);

	aOutPinMatrix = mAttr.create("pinMatrix", "pnmt", MFnMatrixAttribute::kDouble);
	mAttr.setStorable(false);
	mAttr.setHidden(false);
	mAttr.setReadable(true);
	addAttribute(aOutPinMatrix);

	aOutPaintingMatrix = mAttr.create("paintingMatrix", "ptmt", MFnMatrixAttribute::kDouble);
	mAttr.setStorable(false);
	mAttr.setHidden(false);
	mAttr.setReadable(true);
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

	// PROBES
	aNumProbes = nAttr.create("numProbes", "npr", MFnNumericData::k2Int);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(3, 4);
	st = addAttribute(aNumProbes);

	aProbeBorder = nAttr.create("probeBorder", "pbr", MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0);
	st = addAttribute(aProbeBorder);

	aProbeColor = nAttr.createColor("probeColor", "pbc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aProbeColor);

	aDisplayProbes = nAttr.create("displayProbes", "dpr", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(false);
	st = addAttribute(aDisplayProbes);

	aProbePointSize = nAttr.create("probePointSize", "pps", MFnNumericData::kFloat);
	nAttr.setDefault(2.0f);
	nAttr.setKeyable(true);
	st = addAttribute(aProbePointSize);
	mser;

	aDisplacement = nAttr.create("displacement", "disp", MFnNumericData::kDouble);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	nAttr.setArray(true);
	st = addAttribute(aDisplacement);

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

	aDisplaySquareMatrix = nAttr.create("displaySquareMatrix", "dsq", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplaySquareMatrix);

	aDisplayBoardMatrix = nAttr.create("displayBoardMatrix", "dbo", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayBoardMatrix);

	aDisplayPinMatrix = nAttr.create("displayPinMatrix", "dpi", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayPinMatrix);

	aDisplayPaintingMatrix = nAttr.create("displayPaintingMatrix", "dpa", MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(true);
	st = addAttribute(aDisplayPaintingMatrix);

	aOutMesh = tAttr.create("outMesh", "om", MFnData::kMesh);
	tAttr.setStorable(false);
	tAttr.setWritable(false);
	addAttribute(aOutMesh);

	aOutProbePoints = tAttr.create("outProbePoints", "opts", MFnData::kVectorArray);
	tAttr.setStorable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutProbePoints);


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

	st = attributeAffects(aPin0, aOutPaintingMatrix);
	st = attributeAffects(aPinX, aOutPaintingMatrix);
	st = attributeAffects(aPinY, aOutPaintingMatrix);
	st = attributeAffects(aPinPainting, aOutPaintingMatrix);

	st = attributeAffects(aNumProbes, aOutMesh);
	st = attributeAffects(aProbeBorder, aOutMesh);
	st = attributeAffects(aBoardHeight, aOutMesh);
	st = attributeAffects(aGap, aOutMesh);
	st = attributeAffects(aImageResolution, aOutMesh);
	st = attributeAffects(aDisplacement, aOutMesh);

	st = attributeAffects(aNumProbes, aOutProbePoints);
	st = attributeAffects(aBoardHeight, aOutProbePoints);
	st = attributeAffects(aGap, aOutProbePoints);
	st = attributeAffects(aImageResolution, aOutProbePoints);
	st = attributeAffects(aPinPainting, aOutProbePoints);
	st = attributeAffects(aPin0, aOutProbePoints);
	st = attributeAffects(aPinX, aOutProbePoints);
	st = attributeAffects(aPinY, aOutProbePoints);

	return (MS::kSuccess);
}
 
MStatus diptych::compute(const MPlug &plug, MDataBlock &data)
{

	MStatus st;
	if (!(
			plug == aOutSquareMatrix ||
			plug == aOutBoardMatrix ||
			plug == aOutPinMatrix ||
			plug == aOutPaintingMatrix ||
			plug == aOutMesh ||
			plug == aOutProbePoints))
	{
		return (MS::kUnknownParameter);
	}

	// MMatrix wm = NodeUtils::firstWorldMatrix(thisMObject());

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
	MMatrix paintingMat = calculatePaintingMatrix(data, pinMat[0][0], pinMat[1][1]);

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

	// create mesh data structure for output
	//////////////
	MFnMeshData fnC;
	MObject outGeom = fnC.create(&st);
	mser;
	MPointArray outProbePoints;
	st = generateMeshAndProbes(data, boardMat, paintingMat, outProbePoints, outGeom);
	MVectorArray outProbeVectors;
	for (size_t i = 0; i < outProbePoints.length(); i++)
	{
		outProbeVectors.append(MVector(outProbePoints[i]));
	}
	


	/////////////
	MDataHandle hMesh = data.outputValue(aOutMesh, &st);
	hMesh.set(outGeom);
	data.setClean(aOutMesh);


	MDataHandle hOutProbePoints = data.outputValue(aOutProbePoints);
	MFnVectorArrayData fnOutPoints;
	MObject dOutPoints = fnOutPoints.create(outProbeVectors, &st);
	mser;
	hOutProbePoints.set(dOutPoints);
	hOutProbePoints.setClean();

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

MMatrix diptych::calculatePaintingMatrix(MDataBlock &data, double xScale, double yScale) const
{

	MMatrix paintingMat;
	paintingMat.setToIdentity();

	bool pinPainting = data.inputValue(aPinPainting).asBool();
	if (!pinPainting)
	{
		return paintingMat;
	}

	MFloatVector pin0 = data.inputValue(aPin0).asFloatVector();
	MFloatVector pinX = data.inputValue(aPinX).asFloatVector();
	MFloatVector pinY = data.inputValue(aPinY).asFloatVector();

	MFloatVector nx = (pinX - pin0).normal();
	MFloatVector ny = (pinY - pin0).normal();

	if ((nx.isEquivalent(MVector::zero) || ny.isEquivalent(MVector::zero) || nx.isParallel(ny)))
	{
		return paintingMat;
	}

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

	paintingMat[3][0] = xScale;
	paintingMat[3][1] = yScale;
	paintingMat *= cornerPinMatrix;

	return paintingMat;
}

MStatus diptych::generateMeshAndProbes(
	MDataBlock &data,
	const MMatrix &boardMatrix,
	const MMatrix &paintingMatrix,
	MPointArray &outProbePoints,
	MObject &outGeom ) const
{
	MStatus st;

	float probeBorder = data.inputValue(aProbeBorder).asFloat();

	const int2 &numProbePoints = data.inputValue(aNumProbes).asInt2();
	int numX = std::max(2, numProbePoints[0]);
	int numY = std::max(2, numProbePoints[1]);

	MPointArray points;
	MIntArray connectivity;
	MIntArray faceCounts;
	points.clear();
	connectivity.clear();
	faceCounts.clear();

	double boardAspect = boardMatrix[0][0] / boardMatrix[1][1];

	MMatrix centerMatrix = MMatrix::identity;
	centerMatrix[3][0] = -1.0;
	centerMatrix[3][1] = -1.0;
	centerMatrix[0][0] = 2.0;
	centerMatrix[1][1] = 2.0;

	MMatrix shrinkMatrix = MMatrix::identity;
	shrinkMatrix[0][0] = 1.0 - probeBorder;
	shrinkMatrix[1][1] = 1.0 - probeBorder;

	MMatrix growMatrix = MMatrix::identity;
	growMatrix[0][0] = 1.0 + probeBorder;
	growMatrix[1][1] = 1.0 + probeBorder;

	MMatrix mirrorMatrix = MMatrix::identity;
	mirrorMatrix[3][0] = boardMatrix[3][0] * -2.0;

	MMatrix rightPatchMatrix = centerMatrix * shrinkMatrix * boardMatrix;
	MMatrix leftPatchMatrix = rightPatchMatrix * mirrorMatrix;

	MMatrix rightBorderMatrix = centerMatrix * growMatrix * boardMatrix;
	MMatrix leftBorderMatrix = rightBorderMatrix * mirrorMatrix;

	MDoubleArray rightDisplacement;
	MDoubleArray leftDisplacement;
	int numPoints = numX * numY * 2;
	getDisplacement(data, numPoints, rightDisplacement, leftDisplacement);

	int offset = 0;
	createPatch(numX, numY, offset, rightPatchMatrix, rightDisplacement, points, faceCounts, connectivity);
	createBorder(numX, numY, offset, rightBorderMatrix, points, faceCounts, connectivity);
	offset = points.length();
	createPatch(numX, numY, offset, leftPatchMatrix, leftDisplacement, points, faceCounts, connectivity);
	createBorder(numX, numY, offset, leftBorderMatrix, points, faceCounts, connectivity);

	MMatrix worldRightPatchMatrix =	rightPatchMatrix * paintingMatrix;
	MMatrix worldLeftPatchMatrix =	leftPatchMatrix * paintingMatrix;
	appendProbePoints(numX, numY,  worldRightPatchMatrix,  outProbePoints);
	appendProbePoints(numX, numY, worldLeftPatchMatrix,  outProbePoints);

	

	MFnMesh fnMesh;

	fnMesh.create(
		points.length(),
		faceCounts.length(),
		points,
		faceCounts,
		connectivity,
		outGeom, &st);

	return st;
}


 
void diptych::createPatch(
	  int numX,
      int numY,
      int offset,
      const MMatrix &matrix,
      const MDoubleArray &displacement,
      MPointArray &points,
      MIntArray &faceCounts,
      MIntArray &connectivity) const
{

	// Make the main set of points to probe
	
	// x and y are 0-1 in x and y direction
	// d is the index into the displacement array
	int d = 0;
	int vid = points.length();
	for (int j = 0; j < numY; j++)
	{
		double y = (double)j / (double)(numY - 1);
		for (int i = 0; i < numX; i++)
		{
			double x = (double)i / (double)(numX - 1);
			MPoint point = MPoint(x, y, displacement[d]) * matrix;
			points.append(point);

			if (j > 0 && i > 0)
			{
				connectivity.append(vid);
				connectivity.append((vid - 1));
				connectivity.append(((vid - 1) - numX));
				connectivity.append((vid - numX));
				faceCounts.append(4);
			}
			vid++;
			d++;
		}
	}
}

void diptych::createBorder(
	int numX,
	int numY,
	int offset,
	const MMatrix &matrix,
	MPointArray &points,
	MIntArray &faceCounts,
	MIntArray &connectivity) const
{
	int vid = points.length();
	// Bottom row

	// This function was difficult to write and hopefully it never needs changing. We already have a
	// grid patch of points. We want to create a border of points. We create the bottom border. Then
	// in parallel we create the left and right columns. Then we create the top border. The
	// connectivity is the most difficult part. Rather than document iot, itf it ever needs
	// adjusting, we'll just have to rewrite it. Soz!

	// For each border point we create, src_id is the index of the vertex of the existing patch
	// whose z value we want to match.  
	int src_id;

	// Bottom row. Make points and faces
	for (int i = 0; i < numX; i++)
	{
		double x = (double)i / (double)(numX - 1);
		src_id = offset + i;
		MPoint point = MPoint(x, 0.0, points[src_id].z) * matrix;
		points.append(point);

		if (i > 0)
		{
			connectivity.append(vid);
			connectivity.append(offset + i);
			connectivity.append(offset + (i - 1));
			connectivity.append(vid - 1);
			faceCounts.append(4);
		}
		vid++;
	}

	// When we get to the top corner border vertices, remember the IDs as we'll need them when
	// completing faces on the top row. It will be easier than calculating them mathematically.
	int topLeftId =0;
	int topRightId =0;
	
	// Make points and faces up the left and right.
	for (int j = 1; j < numY; j++)
	{
		double y = (double)j / (double)(numY - 1);
		// Left column
		src_id = offset + (j * numX);
		MPoint point = MPoint(0.0, y, points[src_id].z) * matrix;
		points.append(point);
		connectivity.append(vid);
		connectivity.append(j == 1 ? vid - numX : vid - 2);
		connectivity.append(offset + ((j - 1) * numX));
		connectivity.append(offset + (j * numX));
		faceCounts.append(4);
		vid++;

		// Right column
		src_id = offset + ((j * numX) + (numX - 1));
		point = MPoint(1.0, y, points[src_id].z) * matrix;
		points.append(point);
		connectivity.append(vid);
		connectivity.append(offset + ((j * numX) + (numX - 1)));
		connectivity.append(offset + ((j * numX) - 1));
		connectivity.append(vid - 2);
		faceCounts.append(4);

		if (j == numY-1)
		{
			topLeftId = vid -1;
			topRightId = vid;
		}
		vid++;
	}

	// Top row. Remember, the top left and right points are already in place. 
	for (int i = 1; i < numX; i++)
	{
		if (i < numX - 1) // don't make last point
		{
			double x = (double)i / (double)(numX - 1);
			src_id = offset + (i + (numX * (numY - 1)));
			MPoint point = MPoint(x, 1.0,  points[src_id].z ) * matrix;
			points.append(point);
		}

		connectivity.append(i==(numX - 1)? topRightId : vid);
		connectivity.append(i==1 ? topLeftId : vid - 1);
		connectivity.append(offset + (i - 1 + (numX * (numY - 1))));
		connectivity.append(offset + (i + (numX * (numY - 1))));
		faceCounts.append(4);
	
		vid++;
	}

}

void diptych::getDisplacement(
	MDataBlock &data,
	int numPoints,
	MDoubleArray &rightDisplacement,
	MDoubleArray &leftDisplacement) const
{
	int numPointsPerSide = numPoints / 2;
	MArrayDataHandle ha = data.inputArrayValue(aDisplacement);
	unsigned nInputs = ha.elementCount();
	if (numPoints != nInputs)
	{
		rightDisplacement = MDoubleArray(numPointsPerSide, 0.0);
		leftDisplacement = MDoubleArray(numPointsPerSide, 0.0);
		return;
	}
	rightDisplacement.clear();
	leftDisplacement.clear();
	for (unsigned i = 0; i < numPointsPerSide; i++, ha.next())
	{
		rightDisplacement.append(ha.inputValue().asDouble());
	}
	for (unsigned i = 0; i < numPointsPerSide; i++, ha.next())
	{
		leftDisplacement.append(ha.inputValue().asDouble());
	}
}



void diptych::appendProbePoints(
	int numX,
	int numY,
	const MMatrix &matrix,
	MPointArray &points) const
{
	for (int j = 0; j < numY; j++)
	{
		double y = (double)j / (double)(numY - 1);
		for (int i = 0; i < numX; i++)
		{
			double x = (double)i / (double)(numX - 1);
			points.append(MPoint(x, y, 0.0) * matrix);
		}
	}
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
