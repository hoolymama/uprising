/**
 * @file brushDrawOverride.h
 * @brief Defines the Viewport 2.0 draw override for brushes.
 *
 * This file implements the Viewport 2.0 draw override for brushes:
 * - Provides custom drawing for brushes in Maya's viewport
 * - Handles bounding box calculation
 * - Manages draw preparation and UI drawing
 * - Supports callbacks for viewport updates
 */

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

/**
 * @class brushDrawOverride
 * @brief Viewport 2.0 draw override for brushes.
 *
 * brushDrawOverride provides custom drawing for brushes:
 * - Implements MPxDrawOverride for Viewport 2.0
 * - Handles bounding box calculation
 * - Manages draw preparation and UI drawing
 * - Supports callbacks for viewport updates
 */
class brushDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	/**
	 * @brief Creator function for the draw override.
	 * @param obj Object to create draw override for
	 * @return Pointer to new brushDrawOverride instance
	 */
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new brushDrawOverride(obj);
	}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~brushDrawOverride();

	/**
	 * @brief Gets the supported draw APIs.
	 * @return Supported draw APIs
	 */
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	/**
	 * @brief Checks if the object is bounded.
	 * @param objPath Path to the object
	 * @param cameraPath Path to the camera
	 * @return True if the object is bounded
	 */
	virtual bool isBounded(
		const MDagPath &objPath,
		const MDagPath &cameraPath) const;

	/**
	 * @brief Gets the bounding box of the object.
	 * @param objPath Path to the object
	 * @param cameraPath Path to the camera
	 * @return Bounding box of the object
	 */
	virtual MBoundingBox boundingBox(
		const MDagPath &objPath,
		const MDagPath &cameraPath) const;

	/**
	 * @brief Prepares data for drawing.
	 * @param objPath Path to the object
	 * @param cameraPath Path to the camera
	 * @param frameContext Frame context for drawing
	 * @param oldData Previous draw data
	 * @return User data for drawing
	 */
	virtual MUserData *prepareForDraw(
		const MDagPath &objPath,
		const MDagPath &cameraPath,
		const MHWRender::MFrameContext &frameContext,
		MUserData *oldData);

	/**
	 * @brief Checks if the override has UI drawables.
	 * @return True as the override has UI drawables
	 */
	virtual bool hasUIDrawables() const { return true; }

	/**
	 * @brief Adds UI drawables to the draw manager.
	 * @param objPath Path to the object
	 * @param drawManager Draw manager to add drawables to
	 * @param frameContext Frame context for drawing
	 * @param data User data for drawing
	 */
	virtual void addUIDrawables(
		const MDagPath &objPath,
		MHWRender::MUIDrawManager &drawManager,
		const MHWRender::MFrameContext &frameContext,
		const MUserData *data);

	/**
	 * @brief Checks if call sequence tracing is enabled.
	 * @return False as call sequence tracing is disabled
	 */
	virtual bool traceCallSequence() const
	{
		return false;
	}

	/**
	 * @brief Handles trace messages.
	 * @param message Trace message
	 */
	virtual void handleTraceMessage(const MString &message) const
	{
		MGlobal::displayInfo("brushDrawOverride: " + message);
		cerr << "brushDrawOverride: " << message.asChar() << endl;
	}

private:
	/**
	 * @brief Constructor.
	 * @param obj Object to create draw override for
	 */
	brushDrawOverride(const MObject &obj);

	/**
	 * @brief Callback for marking the override as dirty.
	 * @param clientData Client data for callback
	 */
	static void markDirty(void *clientData);

	/**
	 * @brief Adds a quad to the draw data.
	 * @param id Quad ID
	 * @param p0 First vertex
	 * @param p1 Second vertex
	 * @param p2 Third vertex
	 * @param p3 Fourth vertex
	 * @param color Quad color
	 * @param points Output points array
	 * @param normals Output normals array
	 * @param colors Output colors array
	 * @param indices Output indices array
	 */
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

	/** @brief Pointer to the brush node */
	brushNode *fNode;

	/** @brief Callback ID for model editor changes */
	MCallbackId fModelEditorChangedCbId;
	/** @brief Callback ID for time changes */
	MCallbackId fTimeChangedCbId;
};

#endif
