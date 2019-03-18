#ifndef clapperHw_H
#define clapperHw_H

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


// #ifdef _WIN32

// #define WIN32_LEAN_AND_MEAN
// #include <d3d11.h>
// #include <d3dx11.h>
// #include <d3dcompiler.h>

// #ifndef D3DCOMPILE_ENABLE_STRICTNESS
// #define D3DCOMPILE_ENABLE_STRICTNESS D3D10_SHADER_ENABLE_STRICTNESS
// #define D3DCOMPILE_DEBUG D3D10_SHADER_DEBUG
// #endif

// #include <xnamath.h>

// #endif //_WIN32



class PaintingDrawOverride : public MHWRender::MPxDrawOverride {
public:

	static MHWRender::MPxDrawOverride *Creator(const MObject &obj)
	{
		return new PaintingDrawOverride(obj);
	}

	virtual ~PaintingDrawOverride();

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
		MGlobal::displayInfo("footPrintDrawOverride: " + message);
		cerr <<  "footPrintDrawOverride: " << message.asChar() << endl;

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


	void drawLines(
	  MHWRender::MUIDrawManager &drawManager, const MPointArray &starts,
	  const MVectorArray &directions, double length );

	void drawLines(
	  MHWRender::MUIDrawManager &drawManager, const MPointArray &starts,
	  const MVectorArray &directions, double length,  const MColor &color );

	PaintingDrawOverride(const MObject &obj);

	static void markDirty(void *clientData);

	painting  *fPainting;

	MCallbackId fModelEditorChangedCbId;
	MCallbackId fTimeChangedCbId;
};


#endif
