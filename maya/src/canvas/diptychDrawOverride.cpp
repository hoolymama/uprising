#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
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


	MFnMatrixData fnMatrixData;
   	MObject matObj;

   	MPlug(diptychObj, diptych::aOutSquareMatrix).getValue(matObj);
   	fnMatrixData.setObject(matObj);
	data->squareMatrix = fnMatrixData.matrix(&st);mser;

   	MPlug(diptychObj, diptych::aOutBoardMatrix).getValue(matObj);
   	fnMatrixData.setObject(matObj);
	data->boardMatrix = fnMatrixData.matrix(&st);mser;
 

   	MPlug(diptychObj, diptych::aOutPinMatrix).getValue(matObj);
   	fnMatrixData.setObject(matObj);
	data->pinMatrix = fnMatrixData.matrix(&st);mser;
 

   	MPlug(diptychObj, diptych::aOutPaintingMatrix).getValue(matObj);
   	fnMatrixData.setObject(matObj);
	data->paintingMatrix = fnMatrixData.matrix(&st);mser;
 


	MPlug(diptychObj, diptych::aBoardColor).child(0).getValue(data->boardColor[0]);
	MPlug(diptychObj, diptych::aBoardColor).child(1).getValue(data->boardColor[1]);
	MPlug(diptychObj, diptych::aBoardColor).child(2).getValue(data->boardColor[2]);

	MPlug(diptychObj, diptych::aSquareColor).child(0).getValue(data->squareColor[0]);
	MPlug(diptychObj, diptych::aSquareColor).child(1).getValue(data->squareColor[1]);
	MPlug(diptychObj, diptych::aSquareColor).child(2).getValue(data->squareColor[2]);

	MPlug(diptychObj, diptych::aPinColor).child(0).getValue(data->pinColor[0]);
	MPlug(diptychObj, diptych::aPinColor).child(1).getValue(data->pinColor[1]);
	MPlug(diptychObj, diptych::aPinColor).child(2).getValue(data->pinColor[2]);

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

	MUIDrawManager::LineStyle style = MUIDrawManager::kSolid;
	drawManager.setLineStyle(style);
	drawManager.setColor(cdata->boardColor);
	drawManager.rect(MPoint(cdata->boardMatrix[3][0], 0, 0), MVector::yAxis, MVector::zAxis, cdata->boardMatrix[0][0], cdata->boardMatrix[1][1]);
	if (cdata->mirror)
	{
		drawManager.rect(MPoint(-(cdata->boardMatrix[3][0]), 0, 0), MVector::yAxis, MVector::zAxis, cdata->boardMatrix[0][0], cdata->boardMatrix[1][1]);
	}

 
	drawManager.setLineStyle(MUIDrawManager::kDashed);
	drawManager.setColor(cdata->squareColor);
	drawManager.rect(MPoint(cdata->squareMatrix[3][0], cdata->squareMatrix[3][1], 0), MVector::yAxis, MVector::zAxis, cdata->squareMatrix[0][0], cdata->squareMatrix[1][1]);
	if (cdata->mirror)
	{
		drawManager.rect(MPoint(-(cdata->squareMatrix[3][0]), cdata->squareMatrix[3][1], 0), MVector::yAxis, MVector::zAxis, cdata->squareMatrix[0][0], cdata->squareMatrix[1][1]);
	}

	drawManager.setColor(cdata->pinColor);
	drawManager.rect(MPoint::origin, MVector::yAxis, MVector::zAxis, cdata->pinMatrix[0][0], cdata->pinMatrix[1][1]);
 
	drawManager.setLineStyle(MUIDrawManager::kSolid);

	MPoint p0 = MPoint::origin * cdata->paintingMatrix;
	MPoint pX = MPoint(1.0, 0.0, 0.0) * cdata->paintingMatrix;
	MPoint pY = MPoint(0.0, 1.0, 0.0) * cdata->paintingMatrix;
	MPoint pZ = MPoint(0.0, 0.0, 1.0) * cdata->paintingMatrix;
	

	drawManager.setColor(MColor(1.0, 0.0, 0.0));
	drawManager.line(p0, pX);
	drawManager.setColor(MColor(0.0, 1.0, 0.0));
	drawManager.line(p0, pY);
	drawManager.setColor(MColor(0.0, 0.0, 1.0));
	drawManager.line(p0, pZ);

	drawManager.endDrawable();
}
