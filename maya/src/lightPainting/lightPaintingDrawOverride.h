#ifndef lightPaintingDrawOverride_H
#define lightPaintingDrawOverride_H

#include <stdlib.h>

#include <maya/MDagPath.h>

#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MSelectionList.h>
#include <maya/MBoundingBox.h>


#include <maya/MViewport2Renderer.h>
#include <maya/MStateManager.h>
#include <maya/MShaderManager.h>
#include <assert.h>

#include "lightPaintingDrawData.h"
#include "lightPaintingNode.h"


class lightPaintingDrawOverride : public MHWRender::MPxDrawOverride {
public:

	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new lightPaintingDrawOverride(obj);
	}

	virtual ~lightPaintingDrawOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool isBounded(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath) const;

	virtual MBoundingBox boundingBox(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath) const;

	virtual MUserData *prepareForDraw(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath,
	  const MHWRender::MFrameContext &frameContext,
	  MUserData *oldData);

	virtual bool hasUIDrawables() const { return true; }

	virtual void addUIDrawables(
	  const MDagPath &objPath,
	  MHWRender::MUIDrawManager &drawManager,
	  const MHWRender::MFrameContext &frameContext,
	  const MUserData *data);

	virtual bool traceCallSequence() const
	{
		return false;
	}

	virtual void handleTraceMessage( const MString &message ) const
	{
		MGlobal::displayInfo("lightPaintingDrawOverride: " + message);
		cerr <<  "lightPaintingDrawOverride: " << message.asChar() << endl;
	}

	void drawShaded(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);
	void drawWireframe(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);
	void drawWireframeTargets(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);

	void drawWireframeTargetsPoint(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);
	void drawWireframeTargetsLine(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);
	void drawWireframeTargetsMatrix(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata);

	void drawWireframeBorders(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata );
	// void drawWireframeClusterPath(
	//   MHWRender::MUIDrawManager &drawManager,
	//   const LightPaintingDrawData *cdata  );
	void drawWireframeArrows(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata );
	// void drawWireframePivots(
	//   MHWRender::MUIDrawManager &drawManager,
	//   const LightPaintingDrawData *cdata );
	void drawIds(
	  MHWRender::MUIDrawManager &drawManager,
	  const LightPaintingDrawData *cdata );

private:

	void drawLines(
	  MHWRender::MUIDrawManager &drawManager, const MFloatPointArray &starts,
	  const MFloatVectorArray &directions, float length );

	void drawLines(
	  MHWRender::MUIDrawManager &drawManager, const MFloatPointArray &starts,
	  const MFloatVectorArray &directions, float length,  const MColor &color );

	lightPaintingDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	lightPainting  *fPainting;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};


#endif
