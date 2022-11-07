#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDependencyNode.h>

#include "imagePoints.h"
#include "imagePointsDrawOverride.h"

imagePointsDrawOverride::imagePointsDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fNode = status ? dynamic_cast<imagePoints *>(node.userNode()) : NULL;
}

imagePointsDrawOverride::~imagePointsDrawOverride()
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

void imagePointsDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	imagePointsDrawOverride *ovr = static_cast<imagePointsDrawOverride *>(clientData);
	if (ovr && ovr->fNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fNode->thisMObject());
	}
}

MHWRender::DrawAPI imagePointsDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool imagePointsDrawOverride::isBounded(const MDagPath &objPath,
										const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox imagePointsDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *imagePointsDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	imagePointsDrawData *data = dynamic_cast<imagePointsDrawData *>(oldData);
	if (!data)
	{
		data = new imagePointsDrawData();
	}

	MObject imagePointsObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	MPlug(imagePointsObj, imagePoints::aCircleDisplaySize).getValue(data->circleDisplaySize);
	MPlug(imagePointsObj, imagePoints::aPointSize).getValue(data->pointSize);
	MPlug(imagePointsObj, imagePoints::aLineThickness).getValue(data->lineThickness);

	MPlug(imagePointsObj, imagePoints::aDisplayPoints).getValue(data->displayPoints);
	MPlug(imagePointsObj, imagePoints::aDisplayCircles).getValue(data->displayCircles);
	MPlug(imagePointsObj, imagePoints::aDisplayOrder).getValue(data->displayOrder);

	MPlug(imagePointsObj, imagePoints::aColor).child(0).getValue(data->color[0]);
	MPlug(imagePointsObj, imagePoints::aColor).child(1).getValue(data->color[1]);
	MPlug(imagePointsObj, imagePoints::aColor).child(2).getValue(data->color[2]);

	MPlug(imagePointsObj, imagePoints::aDisplayGradient).getValue(data->displayGradient);
	MPlug(imagePointsObj, imagePoints::aGradientLineLength).getValue(data->gradientLineLength);

	MPlug(imagePointsObj, imagePoints::aGradientColor).child(0).getValue(data->gradientColor[0]);
	MPlug(imagePointsObj, imagePoints::aGradientColor).child(1).getValue(data->gradientColor[1]);
	MPlug(imagePointsObj, imagePoints::aGradientColor).child(2).getValue(data->gradientColor[2]);

	MObject dPoints;
	MPlug(imagePointsObj, imagePoints::aOutPoints).getValue(dPoints);
	mser;
	MVectorArray pts(MFnVectorArrayData(dPoints).array());
	data->points.clear();
	unsigned numPoints = pts.length();

	for (unsigned i = 0; i < numPoints; i++)
	{
		data->points.append(pts[i]);
	}

	MObject dGrad;
	data->lineList.clear();
	MPlug(imagePointsObj, imagePoints::aOutGradients).getValue(dGrad);
	const MVectorArray grad(MFnVectorArrayData(dGrad).array());

	if (data->displayGradient && grad.length() == numPoints)
	{
		for (unsigned i = 0; i < numPoints; i++)
		{

			data->lineList.append(pts[i]);
			data->lineList.append(pts[i] + (grad[i] * data->gradientLineLength));
		}
	}

	MObject dRadius;
	MPlug(imagePointsObj, imagePoints::aOutRadius).getValue(dRadius);
	mser;
	data->radii = MFnDoubleArrayData(dRadius).array();

	return data;
}

void imagePointsDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{
	imagePointsDrawData *cdata = (imagePointsDrawData *)data;
	if (!cdata)
	{
		return;
	}

	if (cdata->displayCircles)
	{
		drawCircles(drawManager, cdata);
	}

	if (cdata->displayPoints > 0)
	{
		drawPoints(drawManager, cdata);
	}

	if (cdata->displayGradient)
	{
		drawGradient(drawManager, cdata);
	}

	{
		drawPoints(drawManager, cdata);
	}

	if (cdata->displayOrder > 0)
	{
		drawOrder(drawManager, cdata);
	}

	drawBorder(drawManager, cdata);
}

void imagePointsDrawOverride::drawPoints(
	MHWRender::MUIDrawManager &drawManager,
	const imagePointsDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	drawManager.setColor(cdata->color);

	drawManager.points(cdata->points, false);
	drawManager.endDrawable();
}

void imagePointsDrawOverride::drawGradient(
	MHWRender::MUIDrawManager &drawManager,
	const imagePointsDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setColor(cdata->gradientColor);
	drawManager.lineList(cdata->lineList, false);
	drawManager.endDrawable();
}

void imagePointsDrawOverride::drawOrder(
	MHWRender::MUIDrawManager &drawManager,
	const imagePointsDrawData *cdata)
{
	drawManager.beginDrawable();
	// drawManager.setPointSize(cdata->pointSize);
	drawManager.setColor(cdata->color);
	unsigned len = cdata->points.length();
	for (unsigned i = 0; i < len; i++)
	{
		MString text;
		text += i;
		drawManager.text(
			MPoint(cdata->points[i]),
			text, MHWRender::MUIDrawManager::kLeft);
	}
	drawManager.endDrawable();
}

void imagePointsDrawOverride::drawCircles(
	MHWRender::MUIDrawManager &drawManager,
	const imagePointsDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	drawManager.setColor(cdata->color);
	unsigned len = cdata->points.length();
	if (cdata->radii.length() != len)
	{
		return;
	}
	for (size_t i = 0; i < len; i++)
	{
		MPoint p(cdata->points[i]);
		drawManager.circle(p, MVector::zAxis, cdata->radii[i] * cdata->circleDisplaySize);
	}
	drawManager.endDrawable();
}

void imagePointsDrawOverride::drawBorder(
	MHWRender::MUIDrawManager &drawManager,
	const imagePointsDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	drawManager.setColor(cdata->color);
	drawManager.rect(MPoint::origin, MVector::yAxis, MVector::zAxis, 1.0, 1.0);
	drawManager.endDrawable();
}