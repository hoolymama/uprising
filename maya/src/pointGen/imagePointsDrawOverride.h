#ifndef imagePointsDrawOverride_H
#define imagePointsDrawOverride_H

// Viewport 2.0 includes

#include <stdlib.h>

#include <maya/MDagPath.h>

#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MSelectionList.h>
#include <maya/MBoundingBox.h>

#include <assert.h>

#include "imagePoints.h"
#include "imagePointsDrawData.h"

class imagePointsDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new imagePointsDrawOverride(obj);
	}

	virtual ~imagePointsDrawOverride();

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

	virtual void handleTraceMessage(const MString &message) const
	{
		MGlobal::displayInfo("imagePointsDrawOverride: " + message);
		cerr << "imagePointsDrawOverride: " << message.asChar() << endl;
	}

private:
	void drawPoints(MHWRender::MUIDrawManager &drawManager,
					const imagePointsDrawData *cdata);

 
	void drawBorder(MHWRender::MUIDrawManager &drawManager,
					const imagePointsDrawData *cdata);

	void drawCircles(MHWRender::MUIDrawManager &drawManager,
					 const imagePointsDrawData *cdata);

	void drawOrder(MHWRender::MUIDrawManager &drawManager,
				   const imagePointsDrawData *cdata);

	imagePointsDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	imagePoints *fNode;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};

#endif
