#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"

#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>

#include <maya/MFnDependencyNode.h>


#include "pearlNode.h"
#include "pearlNodeDrawOverride.h"


pearlNodeDrawOverride::pearlNodeDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
	                            "modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
	                     "timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fNode = status ? dynamic_cast<pearlNode *>(node.userNode()) : NULL;

}

pearlNodeDrawOverride::~pearlNodeDrawOverride() {
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

void pearlNodeDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	pearlNodeDrawOverride *ovr = static_cast<pearlNodeDrawOverride *>(clientData);
	if (ovr && ovr->fNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fNode->thisMObject());
	}
}



MHWRender::DrawAPI pearlNodeDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool pearlNodeDrawOverride::isBounded( const MDagPath &objPath,
    const MDagPath &cameraPath ) const
{
	return false;
}

MBoundingBox pearlNodeDrawOverride::boundingBox(
  const MDagPath &objPath,
  const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *pearlNodeDrawOverride::prepareForDraw(
  const MDagPath &objPath,
  const MDagPath &cameraPath,
  const MHWRender::MFrameContext &frameContext,
  MUserData *oldData
)
{
	MStatus st;

	pearlNodeDrawData *data = dynamic_cast<pearlNodeDrawData *>(oldData);
	if (!data) {
		data = new pearlNodeDrawData();
	}

	MObject pearlNodeObj = objPath.node(&st);
	if (st.error()) {return 0;}

 	MPlug(pearlNodeObj, pearlNode::aDrawEdges).getValue(data->drawEdges);
	MPlug(pearlNodeObj, pearlNode::aDrawPoints).getValue(data->drawPoints);
	MPlug(pearlNodeObj, pearlNode::aDrawCircles).getValue(data->drawCircles);

	if (!(data->drawEdges && data->drawPoints && data->drawCircles) )   {
		return 0;
	}

	MPlug plugPearlChainData(pearlNodeObj, pearlNode::aOutput);
	MObject dPearlChainData;
	st = plugPearlChainData.getValue(dPearlChainData);
	MFnPluginData fnPearlChainData(dPearlChainData);
	pearlChainData *pld = (pearlChainData *)fnPearlChainData.data();
	if (!pld)
	{
		return 0;
	}

	std::vector< pearlChain > *geom  = pld->fGeometry;

	MDoubleArray params;
	std::vector<pearlChain>::const_iterator iter;
	for (iter = geom->begin(); iter != geom->end(); iter++)
	{
		iter->appendPointsTo(data->points);
		iter->appendParamsTo(params);
		iter->appendRadiiTo(data->radius);
		data->counts.append(iter->size());
	}

	MPlug(pearlNodeObj, pearlNode::aPointSize).getValue(data->pointSize);

	MColor c1, c2;
	MPlug(pearlNodeObj, pearlNode::aColor1).child(0).getValue(c1[0]);
	MPlug(pearlNodeObj, pearlNode::aColor1).child(1).getValue(c1[1]);
	MPlug(pearlNodeObj, pearlNode::aColor1).child(2).getValue(c1[2]);

	MPlug(pearlNodeObj, pearlNode::aColor2).child(0).getValue(c2[0]);
	MPlug(pearlNodeObj, pearlNode::aColor2).child(1).getValue(c2[1]);
	MPlug(pearlNodeObj, pearlNode::aColor2).child(2).getValue(c2[2]);

	bool doRandColor;
	MPlug(pearlNodeObj, pearlNode::aRandomChainColor).getValue(doRandColor);

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


void pearlNodeDrawOverride::addUIDrawables(
  const MDagPath &objPath,
  MHWRender::MUIDrawManager &drawManager,
  const MHWRender::MFrameContext &context,
  const MUserData *data)
{
	pearlNodeDrawData *cdata = (pearlNodeDrawData *)data;
	if (! cdata) { return; }

	if (cdata->drawPoints)
	{
		drawPoints(drawManager, cdata);
	}
	if (cdata->drawEdges)
	{
		drawEdges(drawManager, cdata);
	}
	if (cdata->drawCircles)
	{
		drawCircles(drawManager, cdata);
	}
}

void pearlNodeDrawOverride::drawPoints(
  MHWRender::MUIDrawManager &drawManager,
  const pearlNodeDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	drawManager.mesh( MHWRender::MUIDrawManager::kPoints , cdata->points , 0,
	                  &(cdata->colors));
	drawManager.endDrawable();
}

void pearlNodeDrawOverride::drawEdges(
  MHWRender::MUIDrawManager &drawManager,
  const pearlNodeDrawData *cdata)
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


void pearlNodeDrawOverride::drawCircles(
  MHWRender::MUIDrawManager &drawManager,
  const pearlNodeDrawData *cdata)
{
	drawManager.beginDrawable();
	unsigned len = cdata->points.length();
	for (int i = 0; i < len; ++i)
	{
		drawManager.setColor(cdata->colors[i]);
		drawManager.circle(cdata->points[i], 
		MVector::zAxis, cdata->radius[i]);
	}
	drawManager.endDrawable();
}
