#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include <maya/MDrawRegistry.h>

#include "collectStrokes.h"
#include "meshStrokeNode.h"

#include "skeletonStrokeNode.h"
#include "curveStrokeNode.h"

#include "brushCmd.h"
#include "brushData.h"

#include "paintingNodeBase.h"

#include "paintingNode.h"
#include "paintingData.h"

#include "strokeData.h"
#include "strokeNodeBase.h"

#include "lightPaintingNode.h"
#include "lightPaintingData.h"
#include "lightPaintingCmd.h"

#include "paintingCmd.h"
#include "strokeCmd.h"

#include "brushNode.h"

#include "aimStrokes.h"
#include "displaceStrokes.h"
#include "brushLifter.h"

#include "tiltStrokes.h"
#include "mapColorStrokes.h"
#include "strokeMutator.h"

#include "strokeCreator.h"
#include "paintStrokeCreator.h"
#include "skChainData.h"
#include "skChainNode.h"
#include "imagePoints.h"

#include "skGraphNode.h"
#include "skGraphNodeDrawOverride.h"

#include "paintingDrawOverride.h"
#include "lightPaintingDrawOverride.h"
#include "imagePointsDrawOverride.h"

#include "paletteData.h"
#include "paletteNode.h"
#include "colorSpreadNode.h"
#include "paletteDrawOverride.h"

#include "cImgGradField.h"


#include "gateRamp.h"
#include "rotateTargets.h"

#include "bulgeStrokes.h"
#include "mapStrokes.h"

#include "particleStrokeNode.h"

#include "particleTrailsNode.h"
#include "particleTrailsData.h"


MStatus initializePlugin(MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin(obj, PLUGIN_VENDOR, PLUGIN_VERSION, MAYA_VERSION);

	MGlobal::executePythonCommand("import pymel.core as pm;pm.loadPlugin('Kit')");

	st = plugin.registerData("particleTrailsData", particleTrailsData::id,
							 particleTrailsData::creator);
	mser;


	st = plugin.registerData("skChainData", skChainData::id,
							 skChainData::creator);
	mser;

	st = plugin.registerData("brushData", brushData::id,
							 brushData::creator);
	mser;

	st = plugin.registerData("paletteData", paletteData::id,
							 paletteData::creator);
	mser;


	st = plugin.registerData("strokeData", strokeData::id,
							 strokeData::creator);
	mser;

	st = plugin.registerData("paintingData", paintingData::id,
							 paintingData::creator);

	st = plugin.registerNode("cImgGradField", cImgGradField::id,
							 cImgGradField::creator, 
							 cImgGradField::initialize, 
							 MPxNode::kFieldNode);
	mser;





	st = plugin.registerNode("paintingBase", paintingBase::id, paintingBase::creator,
							 paintingBase::initialize, MPxNode::kLocatorNode);
	msert;

	st = plugin.registerNode("painting", painting::id, painting::creator,
							 painting::initialize, MPxNode::kLocatorNode,
							 &painting::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		painting::drawDbClassification,
		painting::drawRegistrantId,
		paintingDrawOverride::Creator);
	mser;


	st = plugin.registerData("lightPaintingData", lightPaintingData::id,
							 lightPaintingData::creator);
	mser;

	st = plugin.registerNode("lightPainting", lightPainting::id, lightPainting::creator,
							 lightPainting::initialize, MPxNode::kLocatorNode,
							 &lightPainting::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		lightPainting::drawDbClassification,
		lightPainting::drawRegistrantId,
		lightPaintingDrawOverride::Creator);
	mser;


	st = plugin.registerNode("skGraph", skGraphNode::id, skGraphNode::creator,
							 skGraphNode::initialize, MPxNode::kLocatorNode,
							 &skGraphNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId,
		skGraphNodeDrawOverride::Creator);
	mser;


	st = plugin.registerNode("palette", paletteNode::id, paletteNode::creator,
							 paletteNode::initialize, MPxNode::kLocatorNode,
							 &paletteNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		paletteNode::drawDbClassification,
		paletteNode::drawRegistrantId,
		PaletteDrawOverride::Creator);
	mser;



	st = plugin.registerNode("particleTrails", particleTrailsNode::id, particleTrailsNode::creator,
							 particleTrailsNode::initialize);

	st = plugin.registerNode("colorSpread", colorSpreadNode::id, colorSpreadNode::creator,
							 colorSpreadNode::initialize);
	msert;
	st = plugin.registerNode("skChainNode", skChainNode::id, skChainNode::creator,
							 skChainNode::initialize);
	msert;

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode);
	msert;
	// brushNodeCallback::id = MDGMessage::addNodeAddedCallback(
	// 	brushNodeCallback::makeDefaultConnections, "brushNode", NULL, &st);
	// mser;

	st = plugin.registerNode("strokeNodeBase", strokeNodeBase::id, strokeNodeBase::creator,
							 strokeNodeBase::initialize);
	msert;

	st = plugin.registerNode("collectStrokes", collectStrokes::id, collectStrokes::creator,
							 collectStrokes::initialize);
	msert;

	st = plugin.registerNode("strokeCreator", strokeCreator::id, strokeCreator::creator,
							 strokeCreator::initialize);
	mser;

	st = plugin.registerNode("paintStrokeCreator", paintStrokeCreator::id, paintStrokeCreator::creator,
							 paintStrokeCreator::initialize);
	mser;
	
	st = plugin.registerNode("particleStroke", particleStrokeNode::id,
							 particleStrokeNode::creator,
							 particleStrokeNode::initialize);
	msert;



	st = plugin.registerNode("meshStroke", meshStrokeNode::id,
							 meshStrokeNode::creator,
							 meshStrokeNode::initialize);
	msert;

	st = plugin.registerNode("skeletonStroke", skeletonStrokeNode::id,
							 skeletonStrokeNode::creator,
							 skeletonStrokeNode::initialize);
	msert;

	st = plugin.registerNode("curveStroke", curveStrokeNode::id, curveStrokeNode::creator,
							 curveStrokeNode::initialize);
	msert;

	st = plugin.registerNode("strokeMutator", strokeMutator::id, strokeMutator::creator,
							 strokeMutator::initialize);
	mser;

	st = plugin.registerNode("mapColorStrokes", mapColorStrokes::id, mapColorStrokes::creator,
							 mapColorStrokes::initialize);
	mser;

	st = plugin.registerNode("aimStrokes", aimStrokes::id, aimStrokes::creator,
							 aimStrokes::initialize);
	mser;

	st = plugin.registerNode("tiltStrokes", tiltStrokes::id, tiltStrokes::creator,
							 tiltStrokes::initialize);
	mser;


	st = plugin.registerNode("bulgeStrokes", bulgeStrokes::id, bulgeStrokes::creator,
							 bulgeStrokes::initialize);
	mser;


	st = plugin.registerNode("mapStrokes", mapStrokes::id, mapStrokes::creator,
							 mapStrokes::initialize);
	mser;


	st = plugin.registerNode("rotateTargets", rotateTargets::id, rotateTargets::creator,
							 rotateTargets::initialize);
	mser;


	

	st = plugin.registerNode("displaceStrokes", displaceStrokes::id, displaceStrokes::creator,
							 displaceStrokes::initialize);
	mser;

	st = plugin.registerNode("brushLifter", brushLifter::id, brushLifter::creator,
							 brushLifter::initialize);
	mser;

	st = plugin.registerNode("gateRamp", gateRamp::id, gateRamp::creator,
							 gateRamp::initialize);
	msert;

	st = plugin.registerNode("imagePoints", imagePoints::id, imagePoints::creator,
							 imagePoints::initialize, MPxNode::kLocatorNode,
							 &imagePoints::drawDbClassification);

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		imagePoints::drawDbClassification,
		imagePoints::drawRegistrantId,
		imagePointsDrawOverride::Creator);
	mser;

	st = plugin.registerCommand("lightPaintingQuery", lightPaintingCmd::creator,
								lightPaintingCmd::newSyntax);
	mser;

	st = plugin.registerCommand("paintingQuery", paintingCmd::creator,
								paintingCmd::newSyntax);
	mser;

	st = plugin.registerCommand("strokeQuery", strokeCmd::creator,
								strokeCmd::newSyntax);
	mser;

	st = plugin.registerCommand("brushQuery", brushCmd::creator,
								brushCmd::newSyntax);
	mser;

	return st;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin(obj);

	st = plugin.deregisterCommand("brushCmd");
	mser;

	st = plugin.deregisterCommand("strokeCmd");
	mser;

	st = plugin.deregisterCommand("paintingCmd");
	mser;

	st = plugin.deregisterCommand("lightPaintingCmd");
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		imagePoints::drawDbClassification,
		imagePoints::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(imagePoints::id);
	mser;

	st = plugin.deregisterNode(gateRamp::id);
	mser;

	st = plugin.deregisterNode(brushLifter::id);
	mser;

	st = plugin.deregisterNode(displaceStrokes::id);
	mser;

	st = plugin.deregisterNode(rotateTargets::id);
	mser;

	st = plugin.deregisterNode(mapStrokes::id);
	mser;

	st = plugin.deregisterNode(bulgeStrokes::id);
	mser;

	st = plugin.deregisterNode(tiltStrokes::id);
	mser;

	st = plugin.deregisterNode(aimStrokes::id);
	mser;

	st = plugin.deregisterNode(mapColorStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeMutator::id);

	msert;

	st = plugin.deregisterNode(curveStrokeNode::id);
	mser;

	st = plugin.deregisterNode(skeletonStrokeNode::id);
	mser;

	st = plugin.deregisterNode(meshStrokeNode::id);
	mser;

	st = plugin.deregisterNode(particleStrokeNode::id);
	mser;

	

	st = plugin.deregisterNode(paintStrokeCreator::id);
	mser;

	st = plugin.deregisterNode(strokeCreator::id);
	mser;

	st = plugin.deregisterNode(collectStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeNodeBase::id);
	mser;

	st = plugin.deregisterNode(brushNode::id);
	mser;

	st = plugin.deregisterNode(skChainNode::id);
	mser;



	st = plugin.deregisterNode(colorSpreadNode::id);
	mser;

	st = plugin.deregisterNode(particleTrailsNode::id);
	mser;




	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		paletteNode::drawDbClassification,
		paletteNode::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(paletteNode::id);
	mser;



	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(skGraphNode::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		lightPainting::drawDbClassification,
		lightPainting::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(lightPainting::id);
	mser;

	st = plugin.deregisterData(lightPaintingData::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		painting::drawDbClassification,
		painting::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(painting::id);
	mser;
	st = plugin.deregisterNode(paintingBase::id);
	mser;

	st = plugin.deregisterNode(cImgGradField::id);
	mser;

	st = plugin.deregisterData(paintingData::id);
	mser;

	st = plugin.deregisterData(strokeData::id);
	mser;

	st = plugin.deregisterData(paletteData::id);
	mser;

	st = plugin.deregisterData(brushData::id);
	mser;

	st = plugin.deregisterData(skChainData::id);
	mser;

	st = plugin.deregisterData(particleTrailsData::id);
	mser;

	


	return st;
}
