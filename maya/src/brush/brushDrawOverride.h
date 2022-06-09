#ifndef brushDrawOverride_H
#define brushDrawOverride_H

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

#include "brushNode.h"
#include "brushDrawData.h"

class brushDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new brushDrawOverride(obj);
	}

	virtual ~brushDrawOverride();

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
		MGlobal::displayInfo("brushDrawOverride: " + message);
		cerr << "brushDrawOverride: " << message.asChar() << endl;
	}

private:
	brushDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	void addQuad(
		int id,
		const MFloatPoint &p0,
		const MFloatPoint &p1,
		const MFloatPoint &p2,
		const MFloatPoint &p3,
		const MColor &color,
		MFloatPointArray &points,
		MFloatVectorArray &normals,
		MColorArray &colors,
		MUintArray &indices);

	brushNode *fNode;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};

#endif
