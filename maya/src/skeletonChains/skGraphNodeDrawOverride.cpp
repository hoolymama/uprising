#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>

#include "skGraphNode.h"
#include "skGraphNodeDrawOverride.h"


skGraphNodeDrawOverride::skGraphNodeDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
	                            "modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
	                     "timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fNode = status ? dynamic_cast<skGraphNode *>(node.userNode()) : NULL;

}

skGraphNodeDrawOverride::~skGraphNodeDrawOverride() {
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

void skGraphNodeDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	skGraphNodeDrawOverride *ovr = static_cast<skGraphNodeDrawOverride *>(clientData);
	if (ovr && ovr->fNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fNode->thisMObject());
	}
}



MHWRender::DrawAPI skGraphNodeDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool skGraphNodeDrawOverride::isBounded( const MDagPath &objPath,
    const MDagPath &cameraPath ) const
{
	return false;
}

MBoundingBox skGraphNodeDrawOverride::boundingBox(
  const MDagPath &objPath,
  const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *skGraphNodeDrawOverride::prepareForDraw(
  const MDagPath &objPath,
  const MDagPath &cameraPath,
  const MHWRender::MFrameContext &frameContext,
  MUserData *oldData
)
{
	MStatus st;

	skGraphNodeDrawData *data = dynamic_cast<skGraphNodeDrawData *>(oldData);
	if (!data) {
		data = new skGraphNodeDrawData();
	}

	MObject skGraphNodeObj = objPath.node(&st);
	if (st.error()) {return NULL;}

	MPlug(skGraphNodeObj, skGraphNode::aPointSize).getValue(data->pointSize);
	MPlug(skGraphNodeObj, skGraphNode::aRadiusMult).getValue(data->radiusMult);

	MColor c1, c2;
	MPlug(skGraphNodeObj, skGraphNode::aColor1).child(0).getValue(c1[0]);
	MPlug(skGraphNodeObj, skGraphNode::aColor1).child(1).getValue(c1[1]);
	MPlug(skGraphNodeObj, skGraphNode::aColor1).child(2).getValue(c1[2]);

	MPlug(skGraphNodeObj, skGraphNode::aColor2).child(0).getValue(c2[0]);
	MPlug(skGraphNodeObj, skGraphNode::aColor2).child(1).getValue(c2[1]);
	MPlug(skGraphNodeObj, skGraphNode::aColor2).child(2).getValue(c2[2]);



	// Need points for the UIDrawable
	MObject d;
	MPlug( skGraphNodeObj, skGraphNode::aOutPoints ).getValue(d); mser;
	const MVectorArray &v = MFnVectorArrayData(d).array();
	data->points.clear();
	for (int i = 0; i < v.length(); ++i)
	{
		data->points.append(MPoint(v[i]));
	}

	MPlug( skGraphNodeObj, skGraphNode::aOutRadius ).getValue(d); mser;
	data->radius = MFnDoubleArrayData(d).array();

	MPlug( skGraphNodeObj, skGraphNode::aOutParams ).getValue(d); mser;
	MDoubleArray params = MFnDoubleArrayData(d).array();

	MPlug( skGraphNodeObj, skGraphNode::aOutCounts ).getValue(d); mser;
	data->counts = MFnIntArrayData(d).array();

	MPlug(skGraphNodeObj, skGraphNode::aDrawEdges).getValue(data->drawEdges);
	MPlug(skGraphNodeObj, skGraphNode::aDrawPoints).getValue(data->drawPoints);
	MPlug(skGraphNodeObj, skGraphNode::aDrawCircles).getValue(data->drawCircles);

	bool doRandColor;
	MPlug(skGraphNodeObj, skGraphNode::aRandomChainColor).getValue(doRandColor);




	// Color calcs
	data->colors.clear();
	int pIndex = 0;
	unsigned nChains = data->counts.length();
	if (doRandColor) {
		for (int c = 0; c < nChains; ++c)
		{
			MColor randCol(drand48(), drand48(), drand48());
			for (int i = 0; i < data->counts[c]; ++i)
			{
				data->colors.append(randCol);
			}
		}
	}
	else {
		for (int c = 0; c < nChains; ++c)
		{
			for (int i = 0; i < data->counts[c]; ++i)
			{
				float p = params[pIndex];
				float r =  ((c1.r * (1.0 - p)) + (c2.r * p));
				float g =  ((c1.g * (1.0 - p)) + (c2.g * p));
				float b =  ((c1.b * (1.0 - p)) + (c2.b * p));
				data->colors.append(MColor(r, g, b));
				pIndex ++;
			}
		}
	}





	return data;
}


void skGraphNodeDrawOverride::addUIDrawables(
  const MDagPath &objPath,
  MHWRender::MUIDrawManager &drawManager,
  const MHWRender::MFrameContext &context,
  const MUserData *data)
{
	skGraphNodeDrawData *cdata = (skGraphNodeDrawData *)data;
	if (! cdata) { return; }

	if (cdata->drawPoints)
	{
		// cerr << "drawPoints(drawManager, cdata)"  << endl;

		drawPoints(drawManager, cdata);
	}
	if (cdata->drawEdges)
	{
		// cerr << "drawEdges(drawManager, cdata)"  << endl;

		drawEdges(drawManager, cdata);
	}
	if (cdata->drawCircles)
	{
		// cerr << "drawCircles(drawManager, cdata)"  << endl;

		drawCircles(drawManager, cdata);
	}
}

void skGraphNodeDrawOverride::drawPoints(
  MHWRender::MUIDrawManager &drawManager,
  const skGraphNodeDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	drawManager.mesh( MHWRender::MUIDrawManager::kPoints , cdata->points , 0,
	                  &(cdata->colors));
	drawManager.endDrawable();
}

void skGraphNodeDrawOverride::drawEdges(
  MHWRender::MUIDrawManager &drawManager,
  const skGraphNodeDrawData *cdata)
{
	unsigned pIndex = 0;
	drawManager.beginDrawable();
	unsigned nChains = cdata->counts.length();
	MPointArray pts;
	MColorArray cols;
	for (int c = 0; c < nChains; ++c)
	{
		unsigned len =  cdata->counts[c];
		pts.clear();
		cols.clear();
		for (int i = 0; i < len; ++i)
		{
			pts.append(cdata->points[pIndex]);
			cols.append(cdata->colors[pIndex]);
			pIndex++;
		}
		drawManager.mesh( MHWRender::MUIDrawManager::kLineStrip , pts , 0,  &cols);
	}
	drawManager.endDrawable();
}


void skGraphNodeDrawOverride::drawCircles(
  MHWRender::MUIDrawManager &drawManager,
  const skGraphNodeDrawData *cdata)
{
	drawManager.beginDrawable();
	unsigned len = cdata->points.length();
	for (int i = 0; i < len; ++i)
	{
		drawManager.setColor(cdata->colors[i]);
		drawManager.circle(cdata->points[i], MVector::zAxis, cdata->radius[i]);
	}
	drawManager.endDrawable();
}
