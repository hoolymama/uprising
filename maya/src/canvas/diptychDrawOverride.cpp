#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include <maya/MMatrixArray.h>
#include "errorMacros.h"

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMatrixData.h>

#include "diptych.h"
#include "diptychDrawOverride.h"

diptychDrawOverride::diptychDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fNode = status ? dynamic_cast<diptych *>(node.userNode()) : NULL;
}

diptychDrawOverride::~diptychDrawOverride()
{
	fNode = NULL;
	if (fModelEditorChangedCbId != 0)
	{
		MMessage::removeCallback(fModelEditorChangedCbId);
		fModelEditorChangedCbId = 0;
	}

	if (fTimeChangedCbId != 0)
	{
		MMessage::removeCallback(fTimeChangedCbId);
		fTimeChangedCbId = 0;
	}
}

void diptychDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	diptychDrawOverride *ovr = static_cast<diptychDrawOverride *>(clientData);
	if (ovr && ovr->fNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fNode->thisMObject());
	}
}

MHWRender::DrawAPI diptychDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool diptychDrawOverride::isBounded(const MDagPath &objPath,
									const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox diptychDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *diptychDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	diptychDrawData *data = dynamic_cast<diptychDrawData *>(oldData);
	if (!data)
	{
		data = new diptychDrawData();
	}

	MObject diptychObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	MPlug(diptychObj, diptych::aMirror).getValue(data->mirror);

	MPlug(diptychObj, diptych::aDisplaySquareMatrix).getValue(data->displaySquareMatrix);
	MPlug(diptychObj, diptych::aDisplayBoardMatrix).getValue(data->displayBoardMatrix);
	MPlug(diptychObj, diptych::aDisplayPinMatrix).getValue(data->displayPinMatrix);
	MPlug(diptychObj, diptych::aDisplayPaintingMatrix).getValue(data->displayPaintingMatrix);
	MPlug(diptychObj, diptych::aDisplayProbes).getValue(data->displayProbes);

	double probeBorder;
	MPlug(diptychObj, diptych::aProbeBorder).getValue(probeBorder);

	double paintingLocatorSize;
	MPlug(diptychObj, diptych::aPaintingLocatorSize).getValue(paintingLocatorSize);

	MPlug(diptychObj, diptych::aProbePointSize).getValue(data->probePointSize);

	MMatrix squareMatrix;
	MMatrix boardMatrix;
	MMatrix pinMatrix;
	MMatrix paintingMatrix;

	MFnMatrixData fnMatrixData;
	MObject matObj;

	MPlug(diptychObj, diptych::aOutSquareMatrix).getValue(matObj);
	fnMatrixData.setObject(matObj);
	squareMatrix = fnMatrixData.matrix(&st);
	mser;

	MPlug(diptychObj, diptych::aOutBoardMatrix).getValue(matObj);
	fnMatrixData.setObject(matObj);
	boardMatrix = fnMatrixData.matrix(&st);
	mser;

	MPlug(diptychObj, diptych::aOutPinMatrix).getValue(matObj);
	fnMatrixData.setObject(matObj);
	pinMatrix = fnMatrixData.matrix(&st);
	mser;

	MPlug(diptychObj, diptych::aOutPaintingMatrix).getValue(matObj);
	fnMatrixData.setObject(matObj);
	paintingMatrix = fnMatrixData.matrix(&st);
	mser;

	data->origin = MPoint::origin * paintingMatrix;
	data->up = MVector::yAxis * paintingMatrix;
	data->normal = MVector::zAxis * paintingMatrix;

	// BOARD RECTANGLE
	double boardX = boardMatrix[3][0];
	data->boardCenter = MPoint(boardX, 0, 0) * paintingMatrix;
	data->boardCenterMirror = MPoint(-boardX, 0, 0) * paintingMatrix;
	data->boardScaleX = boardMatrix[0][0];
	data->boardScaleY = boardMatrix[1][1];

	// SQUARE RECTANGLE
	double squareX = squareMatrix[3][0];
	double squareY = squareMatrix[3][1];
	data->squareCenter = MPoint(squareX, squareY, 0) * paintingMatrix;
	data->squareCenterMirror = MPoint(-squareX, squareY, 0) * paintingMatrix;
	data->squareScaleX = squareMatrix[0][0];
	data->squareScaleY = squareMatrix[1][1];

	// PIN RECTANGLE
	data->pinScaleX = pinMatrix[0][0];
	data->pinScaleY = pinMatrix[1][1];

	// PAINTING COORDINATE SYSTEM
	data->paintingXAxis = MPoint(paintingLocatorSize, 0.0, 0.0) * paintingMatrix;
	data->paintingYAxis = MPoint(0.0, paintingLocatorSize, 0.0) * paintingMatrix;
	data->paintingZAxis = MPoint(0.0, 0.0, paintingLocatorSize) * paintingMatrix;

	// PROBE POINTS
	////////////////////////////////////////////////////////////////////////////////
	data->probePoints.clear();
	MPointArray probePointsLocal;
	int numProbesX;
	int numProbesY;
	MPlug(diptychObj, diptych::aNumProbes).child(0).getValue(numProbesX);
	MPlug(diptychObj, diptych::aNumProbes).child(1).getValue(numProbesY);

	MMatrix centerMatrix = MMatrix::identity;
	centerMatrix[3][0] = -1.0;
	centerMatrix[3][1] = -1.0;
	centerMatrix[0][0] = 2.0;
	centerMatrix[1][1] = 2.0;

	MMatrix shrinkMatrix = MMatrix::identity;
	shrinkMatrix[0][0] = 1.0 - probeBorder;
	shrinkMatrix[1][1] = 1.0 - probeBorder;

	MMatrix mirrorMatrix = MMatrix::identity;
	mirrorMatrix[3][0] = boardMatrix[3][0] * -2.0;

	MMatrix rightMatrix = centerMatrix * shrinkMatrix * boardMatrix * paintingMatrix;
	MMatrix leftMatrix = centerMatrix * shrinkMatrix * boardMatrix * mirrorMatrix * paintingMatrix;
	MMatrixArray mats;
	mats.append(rightMatrix);
	mats.append(leftMatrix);
	for (int k = 0; k < mats.length(); k++)
	{
		for (int j = 0; j < numProbesY; j++)
		{
			double y = (double)j / (double)(numProbesY - 1);
			for (int i = 0; i < numProbesX; i++)
			{
				double x = (double)i / (double)(numProbesX - 1);
				MPoint probePoint = MPoint(x, y, 0) * mats[k];
				data->probePoints.append(probePoint);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////

	// COLORS
	MPlug(diptychObj, diptych::aBoardColor).child(0).getValue(data->boardColor[0]);
	MPlug(diptychObj, diptych::aBoardColor).child(1).getValue(data->boardColor[1]);
	MPlug(diptychObj, diptych::aBoardColor).child(2).getValue(data->boardColor[2]);

	MPlug(diptychObj, diptych::aSquareColor).child(0).getValue(data->squareColor[0]);
	MPlug(diptychObj, diptych::aSquareColor).child(1).getValue(data->squareColor[1]);
	MPlug(diptychObj, diptych::aSquareColor).child(2).getValue(data->squareColor[2]);

	MPlug(diptychObj, diptych::aPinColor).child(0).getValue(data->pinColor[0]);
	MPlug(diptychObj, diptych::aPinColor).child(1).getValue(data->pinColor[1]);
	MPlug(diptychObj, diptych::aPinColor).child(2).getValue(data->pinColor[2]);

	MPlug(diptychObj, diptych::aProbeColor).child(0).getValue(data->probeColor[0]);
	MPlug(diptychObj, diptych::aProbeColor).child(1).getValue(data->probeColor[1]);
	MPlug(diptychObj, diptych::aProbeColor).child(2).getValue(data->probeColor[2]);

	return data;
}

void diptychDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{
	diptychDrawData *cdata = (diptychDrawData *)data;
	if (!cdata)
	{
		return;
	}

	drawManager.beginDrawable();

	// BOARD MATRIX

	if (cdata->displayBoardMatrix)
	{
		drawManager.setLineStyle(MUIDrawManager::kSolid);
		drawManager.setColor(cdata->boardColor);

		drawManager.rect(
			cdata->boardCenter,
			cdata->up,
			cdata->normal,
			cdata->boardScaleX,
			cdata->boardScaleY);
		if (cdata->mirror)
		{
			drawManager.rect(
				cdata->boardCenterMirror,
				cdata->up,
				cdata->normal,
				cdata->boardScaleX,
				cdata->boardScaleY);
		}
	}

	// SQUARE MATRIX
	if (cdata->displaySquareMatrix)
	{
		drawManager.setLineStyle(MUIDrawManager::kDashed);
		drawManager.setColor(cdata->squareColor);

		drawManager.rect(
			cdata->squareCenter,
			cdata->up,
			cdata->normal,
			cdata->squareScaleX,
			cdata->squareScaleY);
		if (cdata->mirror)
		{
			drawManager.rect(
				cdata->squareCenterMirror,
				cdata->up,
				cdata->normal,
				cdata->squareScaleX,
				cdata->squareScaleY);
		}
	}

	// PIN
	if (cdata->displayPinMatrix)
	{
		drawManager.setLineStyle(MUIDrawManager::kDashed);
		drawManager.setColor(cdata->pinColor);
		drawManager.rect(
			cdata->origin,
			cdata->up,
			cdata->normal,
			cdata->pinScaleX,
			cdata->pinScaleY);
	}

	// PAINTING MATRIX
	drawManager.setLineStyle(MUIDrawManager::kSolid);
	if (cdata->displayPaintingMatrix)
	{
		drawManager.setColor(MColor(1.0, 0.0, 0.0));
		drawManager.line(cdata->origin, cdata->paintingXAxis);
		drawManager.setColor(MColor(0.0, 1.0, 0.0));
		drawManager.line(cdata->origin, cdata->paintingYAxis);
		drawManager.setColor(MColor(0.0, 0.0, 1.0));
		drawManager.line(cdata->origin, cdata->paintingZAxis);
	}

	// probePoints
	if (cdata->displayProbes)
	{
		drawManager.setPointSize(cdata->probePointSize);
		drawManager.setLineStyle(MUIDrawManager::kSolid);
		drawManager.setColor(cdata->probeColor);
		drawManager.points(cdata->probePoints, false);
	}

	// Some primitives

	drawManager.setPaintStyle	(MUIDrawManager::kShaded);	


	drawManager.setColor(cdata->probeColor);

	drawManager.box(
		MPoint(0.0, 0.0, 10.0),
		MVector::zAxis,
		MVector::xAxis, 1.0, 1.0, 1.0, true);

	drawManager.setColor(cdata->pinColor);


	drawManager.sphere(MPoint(0.0, 0.0, 6.0), 0.5, true);


	drawManager.endDrawable();
}