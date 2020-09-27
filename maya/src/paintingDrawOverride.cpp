

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"

#include "paintingNode.h"
#include "paintingDrawOverride.h"
#include "brush.h"

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
		// if (data == NULL) {
		// 	MGlobal::displayError("Fail to allocate draw data");
		// 	return NULL;
		// }
	}

	MObject paintingObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	MPlug(paintingObj, painting::aPointSize).getValue(data->pointSize);
	MPlug(paintingObj, painting::aLineLength).getValue(data->lineLength);
	MPlug(paintingObj, painting::aLineThickness).getValue(data->lineThickness);
	MPlug(paintingObj, painting::aStackGap).getValue(data->stackGap);
	MPlug(paintingObj, painting::aDisplayTargets).getValue(data->displayTargets);
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

	if ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) || (displayStyle & MHWRender::MFrameContext::kFlatShaded))
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
	double stackHeight = 0.0;
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


			stackHeight += cdata->stackGap;
			MPointArray triangles;
			stroke.getTriangleStrip(brush, stackHeight, triangles, cdata->displayContactWidth, segments);
			drawManager.mesh(
				MHWRender::MUIDrawManager::kTriStrip, triangles);
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
	painting::TargetDisplay style = painting::TargetDisplay(cdata->displayTargets);

	if (style == painting::kTargetsPoint)
	{
		drawWireframeTargetsPoint(drawManager, cdata);
		return;
	}
	if (style == painting::kTargetsLine)
	{
		drawWireframeTargetsLine(drawManager, cdata);
		return;
	}
	if (style == painting::kTargetsMatrix)
	{
		drawWireframeTargetsMatrix(drawManager, cdata);
	}
}

void paintingDrawOverride::drawWireframeTargetsPoint(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	bool withTraversal = cdata->displayApproachTargets;

	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			MPointArray points;
			stroke.getPoints(points, stackHeight, withTraversal);
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
	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			MPointArray starts;
			stroke.getPoints(starts, stackHeight, withTraversal);
			MVectorArray ends;
			stroke.getZAxes(ends, withTraversal);
			drawLines(drawManager, starts, ends, cdata->lineLength);
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
	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			MPointArray starts;
			stroke.getPoints(starts, stackHeight, withTraversal);
			MVectorArray xends, yends, zends;

			stroke.getXAxes(xends, withTraversal);
			stroke.getYAxes(yends, withTraversal);
			stroke.getZAxes(zends, withTraversal);

			drawLines(drawManager, starts, xends, cdata->lineLength, MColor(1.0, 0.0, 0.0));
			drawLines(drawManager, starts, yends, cdata->lineLength, MColor(0.0, 1.0, 0.0));
			drawLines(drawManager, starts, zends, cdata->lineLength, MColor(0.0, 0.0, 1.0));
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeBorders(
	MHWRender::MUIDrawManager &drawManager,
	const PaintingDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setLineWidth(cdata->lineThickness);

	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		const Brush &brush = cdata->geom->brushFromId(cluster.brushId());
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;

			MPointArray lineLoop;
			stroke.getBorderLoop(brush, stackHeight, lineLoop, cdata->displayContactWidth);
			drawManager.mesh(
				MHWRender::MUIDrawManager::kClosedLine, lineLoop);
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

	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		drawManager.setColor(cdata->geom->paintFromId(cluster.paintId()).color);

		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			MPointArray lines;
			stroke.getPoints(lines, stackHeight, true);
			drawManager.mesh(
				MHWRender::MUIDrawManager::kLineStrip, lines);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawWireframeArrows(
	MHWRender::MUIDrawManager &drawManager, const PaintingDrawData *cdata)
{

	if (cdata->arrowheadSize < 0.000001)
	{
		return;
	}
	drawManager.beginDrawable();
	MPoint head(MVector::xAxis * cdata->arrowheadSize);
	MPoint right(MVector(0.5, 0.3, 0.0) * cdata->arrowheadSize);
	MPoint left(MVector(0.5, -0.3, 0.0) * cdata->arrowheadSize);

	double stackHeight = 0.0;
	drawManager.setLineWidth(cdata->lineThickness);

	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			MMatrixArray mats;
			stroke.getDirectionMatrices(mats, stackHeight);

			for (unsigned i = 0; i < mats.length(); ++i)
			{

				MPointArray linestrip;
				linestrip.append(MPoint(mats[i][3][0], mats[i][3][1], mats[i][3][2]));
				linestrip.append(head * mats[i]);
				linestrip.append(left * mats[i]);
				linestrip.append(right * mats[i]);
				linestrip.append(linestrip[1]); // back to the heead
				drawManager.mesh(
					MHWRender::MUIDrawManager::kLineStrip, linestrip);
			}
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
	MPointArray pivots;
	double stackHeight = 0.0;
	for (auto cluster : cdata->geom->clusters())
	{
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;
			pivots.append(stroke.pivot().position() + MVector(0.0f, 0.0f, stackHeight));
		}
	}
	drawManager.mesh(
		MHWRender::MUIDrawManager::kPoints, pivots);
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
	double stackHeight = 0.0;
	MColor textColor(0.1f, 0.8f, 0.8f, 1.0f); // Text color
	drawManager.setColor(textColor);
	drawManager.setFontSize(MHWRender::MUIDrawManager::kSmallFontSize);

	for (auto cluster : cdata->geom->clusters())
	{
		// const Brush &brush = geom.brushFromId(cluster.brushId());
		for (auto stroke : cluster.strokes())
		{
			stackHeight += cdata->stackGap;

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

			MPoint textPos = stroke.getHead(stackHeight) + MVector(cdata->idDisplayOffset);
			drawManager.text(textPos, text, MHWRender::MUIDrawManager::kLeft);
		}
	}
	drawManager.endDrawable();
}

void paintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MPointArray &starts,
									 const MVectorArray &directions,
									 double length)
{
	MPointArray lines;
	for (int i = 0; i < starts.length(); ++i)
	{
		lines.append(starts[i]);
		lines.append(starts[i] + (directions[i] * length));
	}
	drawManager.mesh(
		MHWRender::MUIDrawManager::kLines, lines);
}

void paintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
									 const MPointArray &starts,
									 const MVectorArray &directions, double length,
									 const MColor &color)
{
	MPointArray lines;
	for (int i = 0; i < starts.length(); ++i)
	{
		lines.append(starts[i]);
		lines.append(starts[i] + (directions[i] * length));
	}
	drawManager.setColor(color);
	drawManager.mesh(
		MHWRender::MUIDrawManager::kLines, lines);
}
