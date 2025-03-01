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

#include <maya/MUintArray.h>

#include "brushNode.h"
#include "brushDrawData.h"

#include "brushDrawOverride.h"

const float PI = 3.141592653;

const float RAD_TO_DEG = (180 / PI);

const float TAU = 2.0 * PI;

brushDrawOverride::brushDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fNode = status ? dynamic_cast<brushNode *>(node.userNode()) : NULL;
}

brushDrawOverride::~brushDrawOverride()
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

void brushDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	brushDrawOverride *ovr = static_cast<brushDrawOverride *>(clientData);
	if (ovr && ovr->fNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fNode->thisMObject());
	}
}

MHWRender::DrawAPI brushDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool brushDrawOverride::isBounded(const MDagPath &objPath,
								  const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox brushDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *brushDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	brushDrawData *data = dynamic_cast<brushDrawData *>(oldData);
	if (!data)
	{
		data = new brushDrawData();
	}

	MObject brushObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	int circleVertexCount = 6;
	float gap = TAU / circleVertexCount;
	MFloatPointArray circleVertices;
	for (int i = 0; i < circleVertexCount; i++)
	{
		float angle = gap * i;
		float x = cos(angle) * 0.5f;
		float y = sin(angle) * 0.5f;
		circleVertices.append(MFloatPoint(x, y, 0.0f));
	}

	float width;
	float tipx;
	float tipy;
	float tipz;
	float bristleHeight;
	float paintingParam;
	float dipParam;
	float wipeParam;
	short shape;

	MPlug(brushObj, brushNode::aWidth).getValue(width);
	MPlug(brushObj, brushNode::aTip).child(0).getValue(tipx);
	MPlug(brushObj, brushNode::aTip).child(1).getValue(tipy);
	MPlug(brushObj, brushNode::aTip).child(2).getValue(tipz);

	MPlug(brushObj, brushNode::aBristleHeight).getValue(bristleHeight);
	MPlug(brushObj, brushNode::aPaintingParam).getValue(paintingParam);
	MPlug(brushObj, brushNode::aDipParam).getValue(dipParam);
	MPlug(brushObj, brushNode::aWipeParam).getValue(wipeParam);
	MPlug(brushObj, brushNode::aShape).getValue(shape);

	MPlug(brushObj, brushNode::aTcpScale).getValue(data->coneScale);

	float baseLevel = 0.0f;
	float tipLevel = tipz;
	float bristleLevel = tipz - bristleHeight;
	float ferruleLevel = bristleLevel - 1.0f;
	float taperLevel = bristleLevel + (bristleHeight * 0.5);

	data->points.clear();
	data->normals.clear();
	data->colors.clear();
	data->indices.clear();

	MFloatPointArray ringPoints;
	MFloatMatrix baseMatrix;
	baseMatrix[3][0] = tipx;
	baseMatrix[3][1] = tipy;

	MFloatMatrix mat;
	// BASE
	mat = baseMatrix;
	for (size_t i = 0; i < circleVertexCount; i++)
	{
		ringPoints.append(circleVertices[i] * mat);
	}
	// FERRULE
	MFloatMatrix ferruleMatrix;
	ferruleMatrix[3][2] = ferruleLevel;
	mat = ferruleMatrix * baseMatrix;
	for (size_t i = 0; i < circleVertexCount; i++)
	{
		MFloatPoint point = circleVertices[i] * mat;
		ringPoints.append(point);
	}

	// BRISTLE LEVEL
	MFloatMatrix bristleMatrix;
	bristleMatrix[3][2] = bristleLevel;
	bristleMatrix[0][0] = width;
	bristleMatrix[1][1] = shape ? width : 0.1f;
	mat = bristleMatrix * baseMatrix;
	for (size_t i = 0; i < circleVertexCount; i++)
	{
		MFloatPoint point = circleVertices[i] * mat;
		ringPoints.append(point);
	}

	// TAPER LEVEL
	MFloatMatrix taperMatrix;
	taperMatrix[3][2] = taperLevel;
	taperMatrix[0][0] = width;
	taperMatrix[1][1] = shape ? width : 0.1f;
	mat = taperMatrix * baseMatrix;
	for (size_t i = 0; i < circleVertexCount; i++)
	{
		MFloatPoint point = circleVertices[i] * mat;
		ringPoints.append(point);
	}

	// TIP LEVEL
	MFloatMatrix tipMatrix;
	tipMatrix[3][2] = tipLevel;
	tipMatrix[0][0] = shape ? 0.1 : width;
	tipMatrix[1][1] = 0.1f;
	mat = tipMatrix * baseMatrix;
	for (size_t i = 0; i < circleVertexCount; i++)
	{
		MFloatPoint point = circleVertices[i] * mat;
		ringPoints.append(point);
	}

	int len = ringPoints.length();
	int levels = 4;

	MColorArray levelColors;
	levelColors.setLength(levels);
	levelColors[0] = MColor(0.3f, 0.3f, 0.3f);
	levelColors[1] = MColor(0.6f, 0.6f, 0.2f);
	levelColors[2] = MColor(0.2f, 0.4f, 0.6f);
	levelColors[3] = levelColors[2];
	int quadId=0;
	for (int i = 0; i < levels; i++)
	{
		int levelOffset = i * circleVertexCount;
		for (int j = 0; j < circleVertexCount; j++)
		{
			// make a quad
			int columnOffset = 1;
			if (j == circleVertexCount - 1)
			{
				// last quad
				columnOffset = 1 - circleVertexCount;
			}

			addQuad(
				quadId,
				ringPoints[levelOffset + j],
				ringPoints[levelOffset + j + columnOffset],
				ringPoints[levelOffset + j + circleVertexCount + columnOffset],
				ringPoints[levelOffset + j + circleVertexCount],
				levelColors[i],
				data->points,
				data->normals,
				data->colors,
				data->indices);
			quadId++;
		}
	}

	// cone
	data->coneBase = MPoint(tipx, tipy, tipz-(bristleHeight*paintingParam));
	
	


	return data;
}

void brushDrawOverride::addQuad(
	int id,
	const MFloatPoint &p0,
	const MFloatPoint &p1,
	const MFloatPoint &p2,
	const MFloatPoint &p3,
	const MColor &color,
	MFloatPointArray &points,
	MFloatVectorArray &normals,
	MColorArray &colors,
	MUintArray &indices)
{
	int pointId = id * 4;
	// we get the points in CCW order
	points.append(p0);
	points.append(p1);
	points.append(p2);
	points.append(p3);

	MFloatVector n = (p1 - p0) ^ (p2 - p0).normal();
	normals.append(n);
	normals.append(n);
	normals.append(n);
	normals.append(n);

	colors.append(color);
	colors.append(color);
	colors.append(color);
	colors.append(color);

	indices.append(pointId);
	indices.append(pointId + 1);
	indices.append(pointId + 2);
	indices.append(pointId);
	indices.append(pointId + 2);
	indices.append(pointId + 3);

}

void brushDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{
	brushDrawData *cdata = (brushDrawData *)data;
	if (!cdata)
	{
		return;
	}


	// const unsigned int displayStyle = context.getDisplayStyle();

	// bool filled=false;
	// if (
	// 	(displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
	// 	 (displayStyle & MHWRender::MFrameContext::kFlatShaded)
	// )
	// {
	// 	filled=true;
	// }

	drawManager.beginDrawable();

	drawManager.setPaintStyle(MUIDrawManager::kShaded);

	// drawManager.setColor(cdata->color);

	drawManager.mesh(
		MHWRender::MUIDrawManager::kTriangles,
		cdata->points,
		&(cdata->normals),
		&(cdata->colors),
		&(cdata->indices),
		0);

	drawManager.setColor(MColor(1.0f, 0.0f, 0.0f));
	drawManager.cone(cdata->coneBase,MVector::xAxis, 0.25,cdata->coneScale, true );
	drawManager.setColor(MColor(0.0f, 1.0f, 0.0f));
	drawManager.cone(cdata->coneBase,MVector::yAxis, 0.25,cdata->coneScale, true );
	drawManager.setColor(MColor(0.0f, 0.0f, 1.0f));
	drawManager.cone(cdata->coneBase,MVector::zAxis, 0.25,cdata->coneScale, true );
	


	             

	drawManager.endDrawable();
}