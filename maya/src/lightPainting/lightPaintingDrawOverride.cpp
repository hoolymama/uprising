

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"
#include <maya/MFnMatrixData.h>
#include "lightPaintingDrawOverride.h"
#include "brush.h"
#include "enums.h"

lightPaintingDrawOverride::lightPaintingDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus st;
	MFnDependencyNode node(obj, &st);
	fPainting = st ? dynamic_cast<lightPainting *>(node.userNode()) : NULL;
}

lightPaintingDrawOverride::~lightPaintingDrawOverride()
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

void lightPaintingDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	lightPaintingDrawOverride *ovr = static_cast<lightPaintingDrawOverride *>(clientData);
	if (ovr && ovr->fPainting)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fPainting->thisMObject());
	}
}

MHWRender::DrawAPI lightPaintingDrawOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool lightPaintingDrawOverride::isBounded(const MDagPath &objPath,
									 const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox lightPaintingDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *lightPaintingDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	LightPaintingDrawData *data = dynamic_cast<LightPaintingDrawData *>(oldData);
	if (!data)
	{
		data = new LightPaintingDrawData();
	}

	MObject paintingObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	MPlug plugPaintingData(paintingObj, lightPainting::aOutput);
	MObject dPaintingData;
	st = plugPaintingData.getValue(dPaintingData);
	MFnPluginData fnPaintingData(dPaintingData);
	lightPaintingData *ptd = (lightPaintingData *)fnPaintingData.data();
	if (!ptd)
	{
		return NULL;
	}

	data->strokes = ptd->strokes();
	data->brush = ptd->brush();

	short colorsMode;
	MPlug(paintingObj, lightPainting::aDisplayTargetColors).getValue(colorsMode);
	data->targetDisplayMode = PaintingEnums::TargetColorsDisplay(colorsMode);

	MObject dViewMatrix;
	MPlug(paintingObj, lightPainting::aViewMatrix).getValue(dViewMatrix);
	MFnMatrixData fnMatrix(dViewMatrix,&st);mser;
	const MMatrix & mat = fnMatrix.matrix(&st) ;mser;

	// vector coming into the camera
	data->drawingNormal = MFloatVector(MVector::zAxis * mat).normal();

	MPlug(paintingObj, lightPainting::aPointSize).getValue(data->pointSize);
	MPlug(paintingObj, lightPainting::aLineLength).getValue(data->lineLength);
	MPlug(paintingObj, lightPainting::aLineThickness).getValue(data->lineThickness);
	MPlug(paintingObj, lightPainting::aDisplayTargets).getValue(data->displayTargets);

	// MPlug(paintingObj, lightPainting::aDisplayPivots).getValue(data->displayPivots);
	MPlug(paintingObj, lightPainting::aDisplayIds).getValue(data->displayIds);
	MPlug(paintingObj, lightPainting::aDisplayParentIds).getValue(data->displayParentIds);
	MPlug(paintingObj, lightPainting::aDisplayLayerIds).getValue(data->displayLayerIds);
	MPlug(paintingObj, lightPainting::aArrowheadSize).getValue(data->arrowheadSize);
	MPlug(paintingObj, lightPainting::aDrawParam).getValue(data->drawParam);

	MPlug offPlug(paintingObj, lightPainting::aIdDisplayOffset);

	offPlug.child(0).getValue(data->idDisplayOffset[0]);
	offPlug.child(1).getValue(data->idDisplayOffset[1]);
	offPlug.child(2).getValue(data->idDisplayOffset[2]);


 

	return data;
}

void lightPaintingDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{

	LightPaintingDrawData *cdata = (LightPaintingDrawData *)data;
	if (!cdata)
	{
		return;
	}
	if (!(cdata->strokes && cdata->strokes->size()))
	{
		return;
	}

	const unsigned int displayStyle = context.getDisplayStyle();

	if ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) || 
		 (displayStyle & MHWRender::MFrameContext::kFlatShaded))
	{
		drawShaded(drawManager, cdata);
	}
	if (displayStyle & MHWRender::MFrameContext::kWireFrame)
	{
		drawWireframe(drawManager, cdata);
	}
	drawIds(drawManager, cdata);
}

void lightPaintingDrawOverride::drawShaded(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata
	)
{

	// PaintingEnums::TargetColorsDisplay targetDisplayMode = PaintingEnums::TargetColorsDisplay(cdata->displayTargetColors);


	// TODO: Split function based on draw param.
	int total_segments = 0;

	for (auto stroke :  *(cdata->strokes) )
	{
		total_segments += stroke.size() -1;
	}

	int num_visited_segments = 0;
	int segments = -1;

	float drawParam =  cdata->drawParam;

	drawManager.beginDrawable();
	// float stackHeight = 0.0f;
	bool done = false;
	const Brush &brush = *(cdata->brush);

	drawManager.setDepthPriority(5);


	for (auto stroke : *(cdata->strokes))
	{
		///////// Calculate how many segments to draw
		// REFACTOR !!!
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

		// stackHeight += cdata->stackGap;
		MFloatPointArray triangles;
		stroke.getTriangleStrip(cdata->drawingNormal, brush,  triangles, false, segments);

		MColorArray colors;
		stroke.getTargetBorderColors(colors, segments, cdata->targetDisplayMode);

		drawManager.mesh(MHWRender::MUIDrawManager::kTriStrip, triangles, 0, &colors);

		num_visited_segments = next_accum_segments;
		if (done){
			break;
		}
	}
	drawManager.endDrawable();
}

void lightPaintingDrawOverride::drawWireframe(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
{
	drawWireframeTargets(drawManager, cdata);
	drawWireframeBorders(drawManager, cdata);
	drawWireframeArrows(drawManager, cdata);
}

void lightPaintingDrawOverride::drawWireframeTargets(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
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

void lightPaintingDrawOverride::drawWireframeTargetsPoint(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
{
 
	drawManager.beginDrawable();
	drawManager.setPointSize(cdata->pointSize);
	for (auto stroke : *(cdata->strokes))
	{
		MFloatPointArray points;
		stroke.getPoints(points);
		drawManager.mesh(
			MHWRender::MUIDrawManager::kPoints, points);
	}
	
	drawManager.endDrawable();
}

void lightPaintingDrawOverride::drawWireframeTargetsLine(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
{

	drawManager.beginDrawable();
	drawManager.setColor(MColor(0.0, 0.0, 1.0));

	drawManager.setLineWidth(cdata->lineThickness);
 
	for (auto stroke : *(cdata->strokes))
	{

		MFloatPointArray starts;
		stroke.getPoints(starts);
		MFloatVectorArray ends;
		stroke.getZAxes(ends);
		drawLines(drawManager, starts, ends, cdata->lineLength);
	}
 
	drawManager.endDrawable();
}

void lightPaintingDrawOverride::drawWireframeTargetsMatrix(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
{
	// bool withTraversal = cdata->displayApproachTargets;

	drawManager.beginDrawable();

	drawManager.setLineWidth(cdata->lineThickness);
 
	for (auto stroke : *(cdata->strokes))
	{
 
		MFloatPointArray starts;
		stroke.getPoints(starts);
		MFloatVectorArray xends, yends, zends;

		stroke.getXAxes(xends);
		stroke.getYAxes(yends);
		stroke.getZAxes(zends);

		drawLines(drawManager, starts, xends, cdata->lineLength, MColor(1.0, 0.0, 0.0));
		drawLines(drawManager, starts, yends, cdata->lineLength, MColor(0.0, 1.0, 0.0));
		drawLines(drawManager, starts, zends, cdata->lineLength, MColor(0.0, 0.0, 1.0));
	}
	drawManager.endDrawable();
}

void lightPaintingDrawOverride::drawWireframeBorders(
	MHWRender::MUIDrawManager &drawManager,
	const LightPaintingDrawData *cdata)
{
	drawManager.beginDrawable();
	drawManager.setLineWidth(cdata->lineThickness);
	

	const Brush &brush = *(cdata->brush);
	for (auto stroke : *(cdata->strokes))
	{
 
		MFloatPointArray lineLoop;
		stroke.getBorderLoop(
			cdata->drawingNormal, 
			brush, 
			lineLoop, 
			false);
		drawManager.mesh(
			MHWRender::MUIDrawManager::kClosedLine, lineLoop);
	}
	drawManager.endDrawable();
}

 
void lightPaintingDrawOverride::drawWireframeArrows(
	MHWRender::MUIDrawManager &drawManager, 
	const LightPaintingDrawData *cdata)
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

	for (auto stroke : *(cdata->strokes))
	{
		const std::vector<Target> & targets =  stroke.targets();
		std::vector<Target>::const_iterator citer;
		for (citer = targets.begin(); citer != targets.end(); citer++)
		{
			MFloatMatrix mat = citer->viewMatrix(cdata->drawingNormal);
 
			MFloatPointArray linestrip;
			linestrip.append(MFloatPoint(mat[3][0], mat[3][1], mat[3][2]));
			linestrip.append(head * mat);
			linestrip.append(left * mat);
			linestrip.append(right * mat);
			linestrip.append(linestrip[1]); // back to the head
			drawManager.mesh(
				MHWRender::MUIDrawManager::kLineStrip, linestrip);
		}
	}
	drawManager.endDrawable();
}


void lightPaintingDrawOverride::drawIds(
	MHWRender::MUIDrawManager &drawManager, const LightPaintingDrawData *cdata)
{

	if (!(cdata->displayIds ||
			cdata->displayParentIds ||
			cdata->displayLayerIds))
	{
		return;
	}

	drawManager.beginDrawable();
	MColor textColor(0.1f, 0.8f, 0.8f, 1.0f); // Text color
	drawManager.setColor(textColor);
	drawManager.setFontSize(MHWRender::MUIDrawManager::kSmallFontSize);

  
	for (auto stroke :  *(cdata->strokes))
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
		if (cdata->displayLayerIds)
		{
			text = text + "Lay:" + stroke.layerId() + "\n";
		}

		MPoint textPos = stroke.getHead(cdata->drawingNormal) + MFloatVector(cdata->idDisplayOffset);
		drawManager.text(textPos, text, MHWRender::MUIDrawManager::kLeft);
	
	}
	drawManager.endDrawable();
}

void lightPaintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
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

void lightPaintingDrawOverride::drawLines(MHWRender::MUIDrawManager &drawManager,
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
