/**
 * @file imagePointsDrawOverride.h
 * @brief Defines the imagePointsDrawOverride class for custom viewport rendering of image points.
 * 
 * This file contains the imagePointsDrawOverride class which provides custom drawing
 * functionality for image points in Maya's Viewport 2.0. It handles the preparation
 * of drawing data and the actual rendering of points with various display options.
 */

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

/**
 * @class imagePointsDrawOverride
 * @brief Custom draw override for image points in Maya's Viewport 2.0.
 * 
 * The imagePointsDrawOverride class extends MPxDrawOverride to provide custom drawing
 * functionality for image points. It handles:
 * - Preparation of drawing data from the node's attributes
 * - Custom rendering of points with various display options (points, circles, gradients, order)
 * - Support for both legacy viewport and Viewport 2.0
 */
class imagePointsDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	/**
	 * @brief Static creator method for the draw override.
	 * @param obj The Maya object to create the draw override for.
	 * @return A new instance of imagePointsDrawOverride.
	 */
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new imagePointsDrawOverride(obj);
	}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~imagePointsDrawOverride();

	/**
	 * @brief Returns whether the draw override supports drawing in the legacy viewport.
	 * @return The supported draw APIs.
	 */
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	/**
	 * @brief Checks if the object is bounded.
	 * @param objPath The path to the object being drawn.
	 * @param cameraPath The path to the camera being used for drawing.
	 * @return True if the object is bounded.
	 */
	virtual bool isBounded(
		const MDagPath &objPath,
		const MDagPath &cameraPath) const;

	/**
	 * @brief Returns the bounding box for the object.
	 * @param objPath The path to the object being drawn.
	 * @param cameraPath The path to the camera being used for drawing.
	 * @return The bounding box for the object.
	 */
	virtual MBoundingBox boundingBox(
		const MDagPath &objPath,
		const MDagPath &cameraPath) const;

	/**
	 * @brief Prepares drawing data for the viewport.
	 * @param objPath The path to the object being drawn.
	 * @param cameraPath The path to the camera being used for drawing.
	 * @param frameContext The current frame context.
	 * @param oldData Previous drawing data that can be reused.
	 * @return New drawing data for the current frame.
	 */
	virtual MUserData *prepareForDraw(
		const MDagPath &objPath,
		const MDagPath &cameraPath,
		const MHWRender::MFrameContext &frameContext,
		MUserData *oldData);

	/**
	 * @brief Checks if the object has UI drawables.
	 * @return Always returns true to ensure proper UI drawing.
	 */
	virtual bool hasUIDrawables() const { return true; }

	/**
	 * @brief Performs the actual drawing in the viewport.
	 * @param objPath The path to the object being drawn.
	 * @param drawManager The draw manager used for drawing.
	 * @param frameContext The current frame context.
	 * @param data The drawing data prepared by prepareForDraw.
	 */
	virtual void addUIDrawables(
		const MDagPath &objPath,
		MHWRender::MUIDrawManager &drawManager,
		const MHWRender::MFrameContext &frameContext,
		const MUserData *data);

	/**
	 * @brief Checks if call sequence tracing is enabled.
	 * @return False, as call sequence tracing is not enabled.
	 */
	virtual bool traceCallSequence() const
	{
		return false;
	}

	/**
	 * @brief Handles trace messages for debugging.
	 * @param message The trace message to handle.
	 */
	virtual void handleTraceMessage(const MString &message) const
	{
		MGlobal::displayInfo("imagePointsDrawOverride: " + message);
		cerr << "imagePointsDrawOverride: " << message.asChar() << endl;
	}

private:
	/**
	 * @brief Draws the points.
	 * @param drawManager The draw manager used for drawing.
	 * @param cdata The drawing data containing point information.
	 */
	void drawPoints(MHWRender::MUIDrawManager &drawManager,
					const imagePointsDrawData *cdata);

	/**
	 * @brief Draws the gradient lines.
	 * @param drawManager The draw manager used for drawing.
	 * @param cdata The drawing data containing gradient information.
	 */
	void drawGradient(
		MHWRender::MUIDrawManager &drawManager,
		const imagePointsDrawData *cdata);

	/**
	 * @brief Draws the border.
	 * @param drawManager The draw manager used for drawing.
	 * @param cdata The drawing data containing border information.
	 */
	void drawBorder(MHWRender::MUIDrawManager &drawManager,
					const imagePointsDrawData *cdata);

	/**
	 * @brief Draws the circles representing points.
	 * @param drawManager The draw manager used for drawing.
	 * @param cdata The drawing data containing circle information.
	 */
	void drawCircles(MHWRender::MUIDrawManager &drawManager,
					 const imagePointsDrawData *cdata);

	/**
	 * @brief Draws the order of points.
	 * @param drawManager The draw manager used for drawing.
	 * @param cdata The drawing data containing order information.
	 */
	void drawOrder(MHWRender::MUIDrawManager &drawManager,
				   const imagePointsDrawData *cdata);

	/**
	 * @brief Private constructor.
	 * @param obj The Maya object to create the draw override for.
	 */
	imagePointsDrawOverride(const MObject &obj);

	/**
	 * @brief Marks the node as dirty to trigger a redraw.
	 * @param clientData The client data containing the node to mark dirty.
	 */
	static void markDirty(void *clientData);

	imagePoints *fNode;  /**< Pointer to the associated imagePoints node. */

	MCallbackId fModelEditorChangedCbId;  /**< Callback ID for model editor changes. */
	MCallbackId fTimeChangedCbId;         /**< Callback ID for time changes. */
};

#endif
