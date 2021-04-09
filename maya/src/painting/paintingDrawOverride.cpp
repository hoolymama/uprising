

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"
#include <maya/MFnMatrixData.h>
#include "paintingNode.h"
#include "paintingDrawOverride.h"
#include "brush.h"
#include "enums.h"

paintingDrawOverride::paintingDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fPainting = status ? dynamic_cast<painting *>(node.userNode()) : NULL;
}

paintingDrawOverride::~paintingDrawOverride()
{
	fPainting = NULL;
	if (fModelEditorChangedCbId != 0)
	{
		MMessage::removeCallback(fModelEditorChangedCbId);
		fModelEditorChangedCbId = 0;
	}

	if (fTimeChangedCbId != 0)
	{
		MMessage::removeCallback(fTimeChangedCbId);
		fTimeChangedCbId = 0;
	}
}

void paintingDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	paintingDrawOverride *ovr = static_cast<paintingDrawOverride *>(clientData);
	if (ovr && ovr->fPainting)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fPainting->thisMObject());
	}
}

MHWRender::DrawAPI paintingDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool paintingDrawOverride::isBounded(const MDagPath &objPath,
									 const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox paintingDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *paintingDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	PaintingDrawData *data = dynamic_cast<PaintingDrawData *>(oldData);
	if (!data)
	{
		data = new PaintingDrawData();
	}

	MObject paintingObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}


	MObject dViewMatrix;
	MPlug(paintingObj, painting::aViewMatrix).getValue(dViewMatrix);
	MFnMatrixData fnMatrix(dViewMatrix,&st);mser;
	const MMatrix & mat = fnMatrix.matrix(&st) ;mser;

	data->drawingNormal = MFloatVector(MVector::zAxis * mat).normal();

	MPlug(paintingObj, painting::aPointSize).getValue(data->pointSize);
	MPlug(paintingObj, painting::aLineLength).getValue(data->lineLength);
	MPlug(paintingObj, painting::aLineThickness).getValue(data->lineThickness);

	MPlug(paintingObj, painting::aDisplayTargets).getValue(data->displayTargets);
	// MPlug(paintingObj, painting::aDisplayTargetColors).getValue(data->displayTargetColors);

	MPlug(paintingObj, painting::aDisplayApproachTargets).getValue(data->displayApproachTargets);

	MPlug(paintingObj, painting::aDisplayClusterPath).getValue(data->displayClusterPath);
	MPlug(paintingObj, painting::aDisplayPivots).getValue(data->displayPivots);
	MPlug(paintingObj, painting::aDisplayContactWidth).getValue(data->displayContactWidth);
	MPlug(paintingObj, painting::aDisplayIds).getValue(data->displayIds);
	MPlug(paintingObj, painting::aDisplayParentIds).getValue(data->displayParentIds);
	MPlug(paintingObj, painting::aDisplayLayerIds).getValue(data->displayLayerIds);
	MPlug(paintingObj, painting::aDisplayBrushIds).getValue(data->displayBrushIds);
	MPlug(paintingObj, painting::aDisplayPaintIds).getValue(data->displayPaintIds);
	MPlug(paintingObj, painting::aDisplayRepeatIds).getValue(data->displayRepeatIds);
	MPlug(paintingObj, painting::aArrowheadSize).getValue(data->arrowheadSize);
	MPlug(paintingObj, painting::aStackGap).getValue(data->stackGap);
	MPlug(paintingObj, painting::aDrawParam).getValue(data->drawParam);

	MPlug offPlug(paintingObj, painting::aIdDisplayOffset);

	offPlug.child(0).getValue(data->idDisplayOffset[0]);
	offPlug.child(1).getValue(data->idDisplayOffset[1]);
	offPlug.child(2).getValue(data->idDisplayOffset[2]);


	MPlug(paintingObj, painting::aWireColor).child(0).getValue(data->wireColor[0]);
	MPlug(paintingObj, painting::aWireColor).child(1).getValue(data->wireColor[1]);
	MPlug(paintingObj, painting::aWireColor).child(2).getValue(data->wireColor[2]);

	MPlug plugPaintingData(paintingObj, painting::aOutput);
	MObject dPaintingData;
	st = plugPaintingData.getValue(dPaintingData);
	MFnPluginData fnPaintingData(dPaintingData);
	paintingData *ptd = (paintingData *)fnPaintingData.data();
	if (!ptd)
	{
		return NULL;
	}
	data->geom = ptd->geometry();

	/////////////// Precalc stackGap offsets.
	const float epsilon = 0.00001;
	data->stackOffsets.clear();
	if ( data->stackGap > epsilon) {
		float offset = 0.0f;
		const MFloatVector &n = data->drawingNormal;
		for (auto cluster : data->geom->clusters())
		{
			for (auto stroke : cluster.strokes())
			{
				data->stackOffsets.append(n*offset);
				offset+=data->stackGap;
			}
		}
	}
	///////////////

	return data;
}

void paintingDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{
 
	PaintingDrawData *cdata = (PaintingDrawData *)data;
	if (!cdata)
	{
		return;
	}
	if (!(cdata->geom && cdata->geom->clusters().size()))
	{
		return;
	}


	const unsigned int displayStyle = context.getDisplayStyle();

	if (
		(displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
		 (displayStyle & MHWRender::MFrameContext::kFlatShaded)
	)
	{
		drawShaded(drawManager, cdata);
		;
	}
	if (displayStyle & MHWRender::MFrameContext::kWireFrame)
	{
		drawWireframe(drawManager, cdata);
	}
	drawIds(drawManager, cdata);
}





void paintingDrawOverride::drawShaded(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata
	)
{

	// PaintingEnums::TargetColorsDisplay targetDisplayMode = PaintingEnums::TargetColorsDisplay(cdata->displayTargetColors);


	// TODO: Split function based on draw param.
	int total_segments = 0;
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			total_segments += stroke.size() -1;
		}
	}
	int num_visited_segments = 0;
	int segments = -1;

	float drawParam =  cdata->drawParam;

	drawManager.beginDrawable();

	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

	bool done = false;
	for (auto cluster : cdata->geom->clusters())
	{
		const Brush &brush = cdata->geom->brushFromId(cluster.brushId());

		drawManager.setColor(cdata->geom->paintFromId(cluster.paintId()).color);

		drawManager.setDepthPriority(5);

		for (auto stroke : cluster.strokes())
		{
			///////// Calculate how many segments to draw
			int this_stroke_segments =  stroke.size() -1;
			float last_param = num_visited_segments / float(total_segments);
			int next_accum_segments = num_visited_segments + this_stroke_segments;
			float next_param = next_accum_segments / float(total_segments);

			if (next_param > drawParam) {
				segments = (int)(
					((drawParam - last_param) / (next_param - last_param)) * this_stroke_segments
					);
				done = true;
			}

			MFloatPointArray points;
			stroke.getTriangleStrip(cdata->drawingNormal,brush, points, cdata->displayContactWidth, segments);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}


			drawManager.mesh(MHWRender::MUIDrawManager::kTriStrip, points);
			num_visited_segments = next_accum_segments;
			if (done){
				break;
			}
		}
		if (done){
			break;
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframe(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	drawWireframeTargets(drawManager, cdata);
	drawWireframeBorders(drawManager, cdata);
	drawWireframeClusterPath(drawManager, cdata);
	drawWireframeArrows(drawManager, cdata);
	drawWireframePivots(drawManager, cdata);
}

void paintingDrawOverride::drawWireframeTargets(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	PaintingEnums::TargetDisplay style = PaintingEnums::TargetDisplay(cdata->displayTargets);

	if (style == PaintingEnums::kTargetsPoint)
	{
		drawWireframeTargetsPoint(drawManager, cdata);
		return;
	}
	if (style == PaintingEnums::kTargetsLine)
	{
		drawWireframeTargetsLine(drawManager, cdata);
		return;
	}
	if (style == PaintingEnums::kTargetsMatrix)
	{
		drawWireframeTargetsMatrix(drawManager, cdata);
	}
}

void paintingDrawOverride::drawWireframeTargetsPoint(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	float stackHeight = 0.0f;
	MFloatVector stackOffset ;

	bool withTraversal = cdata->displayApproachTargets;

	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);

	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;
	
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			stackOffset = cdata->drawingNormal *stackHeight;
			MFloatPointArray points;
			stroke.getPoints(points, withTraversal);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}
			drawManager.mesh(
				MHWRender::MUIDrawManager::kPoints, points);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeTargetsLine(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{	

	bool withTraversal = cdata->displayApproachTargets;

	drawManager.beginDrawable();
	drawManager.setColor(MColor(0.0, 0.0, 1.0));
	drawManager.setLineWidth(cdata->lineThickness);
	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			MFloatPointArray points;
			stroke.getPoints(points, withTraversal);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}

			MFloatVectorArray ends;
			stroke.getZAxes(ends, withTraversal);
			drawLines(drawManager, points, ends, cdata->lineLength);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeTargetsMatrix(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	bool withTraversal = cdata->displayApproachTargets;

	drawManager.beginDrawable();

	drawManager.setLineWidth(cdata->lineThickness);

	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{

			MFloatPointArray points;
			stroke.getPoints(points, withTraversal);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}

			MFloatVectorArray xends, yends, zends;

			stroke.getXAxes(xends, withTraversal);
			stroke.getYAxes(yends, withTraversal);
			stroke.getZAxes(zends, withTraversal);

			drawLines(drawManager, points, xends, cdata->lineLength, MColor(1.0, 0.0, 0.0));
			drawLines(drawManager, points, yends, cdata->lineLength, MColor(0.0, 1.0, 0.0));
			drawLines(drawManager, points, zends, cdata->lineLength, MColor(0.0, 0.0, 1.0));
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeBorders(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
 
	MFloatVector stackOffset ;
	drawManager.beginDrawable();
	drawManager.setLineWidth(cdata->lineThickness);
	drawManager.setColor(cdata->wireColor);
	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;


	for (auto cluster : cdata->geom->clusters())
	{
		const Brush &brush = cdata->geom->brushFromId(cluster.brushId());
		for (auto stroke : cluster.strokes())
		{
			MFloatPointArray points;
			stroke.getBorderLoop(cdata->drawingNormal, brush, points, cdata->displayContactWidth);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}

			drawManager.mesh(
				MHWRender::MUIDrawManager::kClosedLine, points);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeClusterPath(
	MHWRender::MUIDrawManager
		&drawManager,
	const PaintingDrawData *cdata)
{
	if (!cdata->displayClusterPath)
	{
		return;
	}

	drawManager.beginDrawable();
	drawManager.setLineWidth(cdata->lineThickness);
	drawManager.setColor(cdata->wireColor);
	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

 
	for (auto cluster : cdata->geom->clusters())
	{
		drawManager.setColor(cdata->geom->paintFromId(cluster.paintId()).color);

		for (auto stroke : cluster.strokes())
		{
			MFloatPointArray points;
			stroke.getPoints(points, true);
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}

			drawManager.mesh(
				MHWRender::MUIDrawManager::kLineStrip, points);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeArrows(
	MHWRender::MUIDrawManager &drawManager, const PaintingDrawData *cdata)
{

	if (cdata->arrowheadSize < 0.0001f)
	{
		return;
	}
	drawManager.beginDrawable();
	MFloatPoint head(MFloatVector::xAxis * cdata->arrowheadSize);
	MFloatPoint right(MFloatVector(0.5f, 0.3f, 0.0f) * cdata->arrowheadSize);
	MFloatPoint left(MFloatVector(0.5f, -0.3f, 0.0f) * cdata->arrowheadSize);

	drawManager.setLineWidth(cdata->lineThickness);
	drawManager.setColor(cdata->wireColor);
	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
 
			const std::vector<Target> & targets =  stroke.targets();
			std::vector<Target>::const_iterator citer;

			for (citer = targets.begin(); citer != targets.end(); citer++)
			{
				MFloatMatrix mat = citer->viewMatrix(cdata->drawingNormal);
	
				MFloatPointArray points;
				points.append(MFloatPoint(mat[3][0], mat[3][1], mat[3][2]));
				points.append(head * mat);
				points.append(left * mat);
				points.append(right * mat);
				points.append(points[1]); // back to the head
				if (doStackOffset) {
					offsetPoints(cdata->stackOffsets[strokeIndex], points);
				}
				drawManager.mesh(
					MHWRender::MUIDrawManager::kLineStrip, points);
			}
			strokeIndex++;
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframePivots(
	MHWRender::MUIDrawManager &drawManager, const PaintingDrawData *cdata)
{
	if (!cdata->displayPivots)
	{
		return;
	}
	drawManager.beginDrawable();
	MFloatPointArray points;
 	unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;

	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			points.append(stroke.pivot().position());
			if (doStackOffset) {
				offsetPoints(cdata->stackOffsets[strokeIndex], points);
				strokeIndex++;
			}
		}
	}
	drawManager.mesh(
		MHWRender::MUIDrawManager::kPoints, points);
	drawManager.endDrawable();
}

void paintingDrawOverride::drawIds(
	MHWRender::MUIDrawManager &drawManager, const PaintingDrawData *cdata)
{

	if (!(
			cdata->displayIds ||
			cdata->displayParentIds ||
			cdata->displayLayerIds ||
			cdata->displayBrushIds ||
			cdata->displayPaintIds ||
			cdata->displayRepeatIds))
	{
		return;
	}

	drawManager.beginDrawable();
 
	MColor textColor(0.1f, 0.8f, 0.8f, 1.0f); // Text color
	drawManager.setColor(textColor);
	drawManager.setFontSize(MHWRender::MUIDrawManager::kSmallFontSize);
		unsigned strokeIndex = 0;
	bool doStackOffset = cdata->stackOffsets.length() > 0;


	for (auto cluster : cdata->geom->clusters())
	{
		// const Brush &brush = geom.brushFromId(cluster.brushId());
		for (auto stroke : cluster.strokes())
		{
 
			MString text("");

			if (cdata->displayIds)
			{
				text = text + "Stk:" + stroke.strokeId() + "\n";
			}
			if (cdata->displayParentIds)
			{
				text = text + "Par:" + stroke.parentId() + "\n";
			}
			if (cdata->displayBrushIds)
			{
				text = text + "Bru:" + stroke.brushId() + "\n";
			}
			if (cdata->displayPaintIds)
			{
				text = text + "Pai:" + stroke.paintId() + "\n";
			}
			if (cdata->displayLayerIds)
			{
				text = text + "Lay:" + stroke.layerId() + "\n";
			}
			if (cdata->displayRepeatIds)
			{
				text = text + "Rep:" + stroke.repeatId() + "\n";
			}

			MPoint textPos = stroke.getHead(cdata->drawingNormal) + MVector(cdata->idDisplayOffset);
			if (doStackOffset) {
				textPos += cdata->stackOffsets[strokeIndex] ;
				strokeIndex++;
			}
			drawManager.text(textPos, text, MHWRender::MUIDrawManager::kLeft);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MFloatPointArray &starts,
									 const MFloatVectorArray &directions,
									 float length)
{
	MFloatPointArray lines;
	for (int i = 0; i < starts.length(); ++i)
	{
		lines.append(starts[i]);
		lines.append(starts[i] + (directions[i] * length));
	}
	drawManager.mesh(
		MHWRender::MUIDrawManager::kLines, lines);
}

void paintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MFloatPointArray &starts,
									 const MFloatVectorArray &directions, 
									 float length,
									 const MColor &color)
{
	MFloatPointArray lines;
	for (int i = 0; i < starts.length(); ++i)
	{
		lines.append(starts[i]);
		lines.append(starts[i] + (directions[i] * length));
	}
	drawManager.setColor(color);
	drawManager.mesh(
		MHWRender::MUIDrawManager::kLines, lines);
}
