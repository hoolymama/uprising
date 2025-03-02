/**
 * @file paintingDrawOverride.h
 * @brief Defines the paintingDrawOverride class for custom viewport rendering of painting nodes.
 * 
 * This file contains the paintingDrawOverride class which provides custom viewport 2.0 
 * rendering for painting nodes. It handles the preparation of draw data, bounding box 
 * calculations, and UI drawing operations for visualizing painting elements in Maya's viewport.
 */
#ifndef paintingDrawOverride_H
#define paintingDrawOverride_H

// Viewport 2.0 includes

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

#include "paintingDrawData.h"

/**
 * @class paintingDrawOverride
 * @brief Custom draw override for painting nodes in Viewport 2.0.
 * 
 * The paintingDrawOverride class extends MPxDrawOverride to provide custom
 * viewport 2.0 rendering for painting nodes. It handles:
 * - Preparation of draw data for rendering
 * - Bounding box calculations
 * - UI drawing operations for visualizing painting elements
 * - Different drawing modes (wireframe, shaded, etc.)
 */
class paintingDrawOverride : public MHWRender::MPxDrawOverride {
public:

	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new paintingDrawOverride(obj);
	}

	virtual ~paintingDrawOverride();

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
		MGlobal::displayInfo("paintingDrawOverride: " + message);
		cerr <<  "paintingDrawOverride: " << message.asChar() << endl;
	}

	void drawShaded(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);

	void drawWireframe(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);

	void drawWireframeTargets(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);

	void drawWireframeTargetsPoint(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);

	void drawWireframeTargetsLine(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);

	void drawWireframeTargetsMatrix(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata);


	void drawWireframeBorders(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata );

	void drawWireframeClusterPath(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata  );

	void drawWireframeArrows(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata );

	void drawWireframePivots(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata );

	void drawIds(
	  MHWRender::MUIDrawManager &drawManager,
	  const PaintingDrawData *cdata );

private:


	void drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MFloatPointArray &starts,
									 const MFloatVectorArray &directions,
									 float length);

	void drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MFloatPointArray &starts,
									 const MFloatVectorArray &directions, 
									 float length,
									 const MColor &color);

	paintingDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	painting  *fPainting;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;


	static void offsetPoints(const MFloatVector &offset, MFloatPointArray & points) ;
};

inline void paintingDrawOverride::offsetPoints(
	const MFloatVector &offset,
	MFloatPointArray &points) 
{	
	unsigned len = points.length();
	for (size_t i = 0; i < len; i++)
	{
		points[i] = points[i]+offset;
	}
}

#endif
