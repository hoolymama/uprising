/**
 * @file paletteDrawOverride.h
 * @brief Defines the PaletteDrawOverride class for custom viewport rendering of palette nodes.
 * 
 * This file contains the PaletteDrawOverride class which provides custom viewport 2.0 
 * rendering for palette nodes. It handles the preparation of draw data, bounding box 
 * calculations, and UI drawing operations for visualizing palette elements in Maya's viewport.
 */

#ifndef PaletteDrawOverride_H
#define PaletteDrawOverride_H

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

#include "paletteDrawData.h"
#include "paletteNode.h"

/**
 * @class PaletteDrawOverride
 * @brief Custom draw override for palette nodes in Maya's Viewport 2.0.
 * 
 * The PaletteDrawOverride class extends MPxDrawOverride to provide custom drawing
 * functionality for palette nodes in Maya's Viewport 2.0. It handles:
 * - Preparation of draw data for rendering
 * - Bounding box calculations
 * - UI drawing operations for visualizing palette elements
 * - Callback management for viewport updates
 */
class PaletteDrawOverride : public MHWRender::MPxDrawOverride {
public:
	/**
	 * @brief Creator function for Maya's plugin system.
	 * @param obj The Maya object to create the draw override for.
	 * @return Pointer to a new PaletteDrawOverride object.
	 */
	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new PaletteDrawOverride(obj);
	}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~PaletteDrawOverride();

	/**
	 * @brief Returns the draw APIs supported by this override.
	 * @return MHWRender::DrawAPI value indicating supported draw APIs.
	 */
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	/**
	 * @brief Indicates whether the object is bounded.
	 * @param objPath The DAG path to the object.
	 * @param cameraPath The DAG path to the camera.
	 * @return true if the object is bounded, false otherwise.
	 */
	virtual bool isBounded(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath) const;

	/**
	 * @brief Returns the bounding box for the object.
	 * @param objPath The DAG path to the object.
	 * @param cameraPath The DAG path to the camera.
	 * @return MBoundingBox representing the object's bounds.
	 */
	virtual MBoundingBox boundingBox(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath) const;

	/**
	 * @brief Prepares data needed for drawing.
	 * @param objPath The DAG path to the object.
	 * @param cameraPath The DAG path to the camera.
	 * @param frameContext The current frame context.
	 * @param oldData Previously prepared data that can be reused.
	 * @return MUserData pointer containing the prepared draw data.
	 */
	virtual MUserData *prepareForDraw(
	  const MDagPath &objPath,
	  const MDagPath &cameraPath,
	  const MHWRender::MFrameContext &frameContext,
	  MUserData *oldData);

	/**
	 * @brief Indicates whether the override provides UI drawables.
	 * @return true, as this override provides UI drawables.
	 */
	virtual bool hasUIDrawables() const { return true; }

	/**
	 * @brief Adds UI drawables to the draw manager.
	 * @param objPath The DAG path to the object.
	 * @param drawManager The UI draw manager.
	 * @param frameContext The current frame context.
	 * @param data The user data prepared by prepareForDraw.
	 */
	virtual void addUIDrawables(
	  const MDagPath &objPath,
	  MHWRender::MUIDrawManager &drawManager,
	  const MHWRender::MFrameContext &frameContext,
	  const MUserData *data);

	/**
	 * @brief Indicates whether to trace the call sequence for debugging.
	 * @return false, as tracing is disabled by default.
	 */
	virtual bool traceCallSequence() const
	{
		return false;
	}

	/**
	 * @brief Handles trace messages for debugging.
	 * @param message The trace message to handle.
	 */
	virtual void handleTraceMessage( const MString &message ) const
	{
		MGlobal::displayInfo("PaletteDrawOverride: " + message);
		cerr <<  "PaletteDrawOverride: " << message.asChar() << endl;
	}

	/**
	 * @brief Draws the shaded representation of the palette.
	 * @param drawManager The UI draw manager.
	 * @param context The current frame context.
	 * @param cdata The palette draw data.
	 */
	void drawShaded(
	  MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	  const PaletteDrawData *cdata);
	  
private:
	/**
	 * @brief Constructor.
	 * @param obj The Maya object to create the draw override for.
	 */
	PaletteDrawOverride(const MObject &obj);

	/**
	 * @brief Callback function to mark the viewport as dirty when node attributes change.
	 * @param clientData Pointer to the client data (the PaletteDrawOverride instance).
	 */
	static void markDirty(void *clientData);

	/** @brief Pointer to the associated palette node. */
	paletteNode  *fPaletteNode;

	/** @brief Callback ID for model editor changes. */
	MCallbackId fModelEditorChangedCbId;
	
	/** @brief Callback ID for time changes. */
	MCallbackId fTimeChangedCbId;
};

#endif
