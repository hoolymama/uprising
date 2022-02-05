#ifndef pearlNodeDrawOverride_H
#define pearlNodeDrawOverride_H

// Viewport 2.0 includes

#include <stdlib.h>





#include <maya/MDagPath.h>

#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MSelectionList.h>
#include <maya/MBoundingBox.h>


// #include <maya/MViewport2Renderer.h>
#include <assert.h>

#include "pearlNodeDrawData.h"


class pearlNodeDrawOverride : public MHWRender::MPxDrawOverride {
public:

	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new pearlNodeDrawOverride(obj);
	}

	virtual ~pearlNodeDrawOverride();

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
		MGlobal::displayInfo("pearlNodeDrawOverride: " + message);
		cerr <<  "pearlNodeDrawOverride: " << message.asChar() << endl;
	}

private:




	void drawPoints(MHWRender::MUIDrawManager &drawManager,
	                const pearlNodeDrawData *cdata);

	void drawEdges(MHWRender::MUIDrawManager &drawManager,
	               const pearlNodeDrawData *cdata);

	void drawCircles(MHWRender::MUIDrawManager &drawManager,
	                 const pearlNodeDrawData *cdata);

	pearlNodeDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	pearlNode  *fNode;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};


#endif
