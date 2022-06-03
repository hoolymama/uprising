

#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MFnPluginData.h>
#include <maya/MEventMessage.h>
#include "errorMacros.h"
// #include <maya/MFnMatrixData.h>
#include "paletteDrawOverride.h"
// #include "brush.h"
// #include "enums.h"

PaletteDrawOverride::PaletteDrawOverride(const MObject &obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{

	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", markDirty, this);

	fTimeChangedCbId = MEventMessage::addEventCallback(
		"timeChanged", markDirty, this);

	MStatus st;
	MFnDependencyNode node(obj, &st);
	fPaletteNode = st ? dynamic_cast<paletteNode *>(node.userNode()) : NULL;
}

PaletteDrawOverride::~PaletteDrawOverride()
{
	fPaletteNode = NULL;
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

void PaletteDrawOverride::markDirty(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	PaletteDrawOverride *ovr = static_cast<PaletteDrawOverride *>(clientData);
	if (ovr && ovr->fPaletteNode)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fPaletteNode->thisMObject());
	}
}

MHWRender::DrawAPI PaletteDrawOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool PaletteDrawOverride::isBounded(const MDagPath &objPath,
									 const MDagPath &cameraPath) const
{
	return false;
}

MBoundingBox PaletteDrawOverride::boundingBox(
	const MDagPath &objPath,
	const MDagPath &cameraPath) const
{
	return MBoundingBox();
}

MUserData *PaletteDrawOverride::prepareForDraw(
	const MDagPath &objPath,
	const MDagPath &cameraPath,
	const MHWRender::MFrameContext &frameContext,
	MUserData *oldData)
{
	MStatus st;

	PaletteDrawData *data = dynamic_cast<PaletteDrawData *>(oldData);
	if (!data)
	{
		data = new PaletteDrawData();
	}

	MObject paletteNodeObj = objPath.node(&st);
	if (st.error())
	{
		return NULL;
	}

	MPlug plugPaletteData(paletteNodeObj, paletteNode::aOutput);
	MObject dPaletteData;
	st = plugPaletteData.getValue(dPaletteData);
	MFnPluginData fnPaletteData(dPaletteData);
	paletteData *pdata = (paletteData *)fnPaletteData.data();
	if (!pdata)
	{
		return NULL;
	}

	data->palette = *(pdata->fGeometry);

	MPlug(paletteNodeObj, paletteNode::aXPos).getValue(data->xPosition);
	MPlug(paletteNodeObj, paletteNode::aYPos).getValue(data->yPosition);
	MPlug(paletteNodeObj, paletteNode::aWidth).getValue(data->width);
	MPlug(paletteNodeObj, paletteNode::aHeight).getValue(data->height);

	MPlug(paletteNodeObj, paletteNode::aDisplayId).getValue(data->displayId);
	MPlug(paletteNodeObj, paletteNode::aDisplayName).getValue(data->displayName);
	MPlug(paletteNodeObj, paletteNode::aDisplayRGB).getValue(data->displayRGB);
	

	return data;
}

void PaletteDrawOverride::addUIDrawables(
	const MDagPath &objPath,
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const MUserData *data)
{


	PaletteDrawData *cdata = (PaletteDrawData *)data;
	if (!cdata)
	{
		return;
	}
	if (!cdata->palette.size())
	{
		return;
	}

	const unsigned int displayStyle = context.getDisplayStyle();

	if ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) || 
		 (displayStyle & MHWRender::MFrameContext::kFlatShaded))
	{
		drawShaded(drawManager, context, cdata);
	}
	if (displayStyle & MHWRender::MFrameContext::kWireFrame)
	{
		return;
	}
}




void PaletteDrawOverride::drawShaded(
	MHWRender::MUIDrawManager &drawManager,
	const MHWRender::MFrameContext &context,
	const PaletteDrawData *cdata
	)
{

//Get the view attributes that impact drawing
    int oX,oy,portWidth,portHeight;
    context.getViewportDimensions(oX,oy,portWidth,portHeight);

	drawManager.beginDrawable();
 
	float fScaleX = cdata->width * portWidth * 0.5 ;
	float fScaleY = cdata->height * portHeight * 0.5;
	float fXPosition = cdata->xPosition * portWidth;
	float fYPosition = cdata->yPosition * portHeight;

	float centerX = fScaleX + fXPosition;
 
	int y=fYPosition + fScaleY;

	drawManager.setFontSize(11);

	float h,s,v;
	MColor textColor;

    for (std::map<int, Paint>::const_iterator iter = cdata->palette.begin();
         iter != cdata->palette.end();
         iter++)
    {
        int paintId = iter->first;
        const Paint & p = iter->second;
		const MColor &c = p.color();
		drawManager.setColor(p.color());
		MPoint center(centerX,y,0.5);

		drawManager.rect2d(center, MVector::yAxis, fScaleX, fScaleY, true);
		y = y + fScaleY + fScaleY+4;

		if (cdata->displayId || cdata->displayName|| cdata->displayRGB) {
			int red = c.r *255;
			int green = c.g *255;
			int blue = c.b *255;



			p.color().get(MColor::kHSV,h,s,v);
			textColor = v > 0.68 ? MColor(0,0,0) : MColor(1,1,1);
			MPoint textPos(center + MVector(-fScaleX,0,-0.1));
			MPoint colorPos(center + MVector(fScaleX,-fScaleY+2,-0.1));
			
			drawManager.setColor(textColor);

			MString txt = "";
			if (cdata->displayName) {
				txt+=p.name() ;
				txt+=" ";
			}
			if (cdata->displayId) {
				txt+=paintId;
			
			}
			if (txt != "") {
				drawManager.text2d(textPos, txt, MUIDrawManager::kLeft);
			}

			if (cdata->displayRGB) {
				MString coltx = "(";
				coltx+=red;coltx+=",";
				coltx+=green;coltx+=",";
				coltx+=blue;coltx+=")";
				drawManager.text2d(colorPos,coltx, MUIDrawManager::kRight)	;
			}
		}
    }

	drawManager.endDrawable();
}
