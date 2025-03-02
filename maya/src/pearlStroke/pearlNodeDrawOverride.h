/**
 * @file pearlNodeDrawOverride.h
 * @brief Defines the pearlNodeDrawOverride class for custom viewport rendering of pearl nodes.
 * 
 * This file contains the pearlNodeDrawOverride class which provides custom drawing
 * functionality for pearl nodes in Maya's Viewport 2.0. It handles the preparation
 * of drawing data and the actual rendering of pearls with various display options.
 */

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

class pearlNode;

/**
 * @class pearlNodeDrawOverride
 * @brief Custom draw override for pearl nodes in Maya's Viewport 2.0.
 * 
 * The pearlNodeDrawOverride class extends MPxDrawOverride to provide custom drawing
 * functionality for pearl nodes. It handles:
 * - Preparation of drawing data from the node's attributes
 * - Custom rendering of pearls with various display options (points, edges, circles)
 * - Support for both legacy viewport and Viewport 2.0
 */
class pearlNodeDrawOverride : public MHWRender::MPxDrawOverride {
public:

	/**
	 * @brief Static creator method for the draw override.
	 * @param obj The Maya object to create the draw override for.
	 * @return A new instance of pearlNodeDrawOverride.
	 */
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new pearlNodeDrawOverride(obj);
	}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~pearlNodeDrawOverride();

	/**
	 * @brief Returns whether the draw override supports drawing in the legacy viewport.
	 * @return True if drawing in the legacy viewport is supported.
	 */
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool isBounded(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath) const;

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
	 * @brief Checks if the transform needs to be updated.
	 * @return Always returns true to ensure proper transform updates.
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

	/**
	 * @brief Private constructor.
	 * @param obj The Maya object to create the draw override for.
	 */
	pearlNodeDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	pearlNode  *fNode;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};


#endif
