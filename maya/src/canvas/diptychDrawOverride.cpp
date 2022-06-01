#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDependencyNode.h>

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

	MPlug(diptychObj, diptych::aOutBoardOffset).getValue(data->boardOffset);

	MPlug(diptychObj, diptych::aOutBoardSize).child(0).getValue(data->boardSizeX);

	MPlug(diptychObj, diptych::aOutBoardSize).child(1).getValue(data->boardSizeY);

	MPlug(diptychObj, diptych::aBoardColor).child(0).getValue(data->boardColor[0]);
	MPlug(diptychObj, diptych::aBoardColor).child(1).getValue(data->boardColor[1]);
	MPlug(diptychObj, diptych::aBoardColor).child(2).getValue(data->boardColor[2]);

	MPlug(diptychObj, diptych::aOutSquareOffset).child(0).getValue(data->squareOffsetX);
	MPlug(diptychObj, diptych::aOutSquareOffset).child(1).getValue(data->squareOffsetY);

	MPlug(diptychObj, diptych::aOutSquareSize).getValue(data->squareSize);

	MPlug(diptychObj, diptych::aSquareColor).child(0).getValue(data->squareColor[0]);
	MPlug(diptychObj, diptych::aSquareColor).child(1).getValue(data->squareColor[1]);
	MPlug(diptychObj, diptych::aSquareColor).child(2).getValue(data->squareColor[2]);

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
	drawManager.rect(MPoint(cdata->boardOffset, 0, 0), MVector::yAxis, MVector::zAxis, cdata->boardSizeX, cdata->boardSizeY);
	if (cdata->mirror)
	{
		drawManager.rect(MPoint(-(cdata->boardOffset), 0, 0), MVector::yAxis, MVector::zAxis, cdata->boardSizeX, cdata->boardSizeY);
	}

	style = MUIDrawManager::kDashed;
	drawManager.setLineStyle(style);
	drawManager.setColor(cdata->squareColor);
	drawManager.rect(MPoint(cdata->squareOffsetX, cdata->squareOffsetY, 0), MVector::yAxis, MVector::zAxis, cdata->squareSize, cdata->squareSize);
	if (cdata->mirror)
	{
		drawManager.rect(MPoint(-(cdata->squareOffsetX), cdata->squareOffsetY, 0), MVector::yAxis, MVector::zAxis, cdata->squareSize, cdata->squareSize);
	}
	drawManager.endDrawable();
}
