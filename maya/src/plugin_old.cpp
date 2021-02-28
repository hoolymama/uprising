#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "brushData.h"
#include "strokeData.h"
#include "paintingData.h"
#include "curveStrokeNode.h"

#include "skGraphNode.h"
 
#include "skeletonStrokeNode.h"
#include "meshStrokeNode.h"

#include "paintingNode.h"
#include "lightPaintingNode.h"
#include "lightPaintingData.h"

#include "brushNode.h"
#include "collectStrokes.h"

#include "strokeMutator.h"
#include "tiltStrokes.h"
#include "mapStrokes.h"
#include "aimStrokes.h"
#include "mapColorStrokes.h"



#include "paintingCmd.h"
#include "brushCmd.h"

#include "skChainData.h"

#include "skChainNode.h"
#include "gateRamp.h"
#include "paintingDrawOverride.h"
#include "skGraphNodeDrawOverride.h"

MStatus initializePlugin(MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin(obj, PLUGIN_VENDOR, PLUGIN_VERSION, MAYA_VERSION);

	const MString UserClassifyindexShader("texture/2d:drawdb/shader/texture/2d/indexShader");
	const MString UserClassifycImgShader("texture/2d:drawdb/shader/texture/2d/cImgShader");

	MGlobal::executePythonCommand("import pymel.core as pm;pm.loadPlugin('Kit')");

	st = plugin.registerData("skChainData", skChainData::id,
							 skChainData::creator);
	mser;

	st = plugin.registerData("brushData", brushData::id,
							 brushData::creator);
	mser;

	st = plugin.registerData("strokeData", strokeData::id,
							 strokeData::creator);
	mser;

	st = plugin.registerData("paintingData", paintingData::id,
							 paintingData::creator);
	mser;

	st = plugin.registerNode("skChainNode", skChainNode::id, skChainNode::creator,
							 skChainNode::initialize);
	msert;

	st = plugin.registerNode("strokeNode", strokeNode::id, strokeNode::creator,
							 strokeNode::initialize);
	msert;

	st = plugin.registerNode("curveStroke", curveStrokeNode::id, curveStrokeNode::creator,
							 curveStrokeNode::initialize);
	msert;

	st = plugin.registerNode("skeletonStroke", skeletonStrokeNode::id,
							 skeletonStrokeNode::creator,
							 skeletonStrokeNode::initialize);
	msert;

	st = plugin.registerNode("meshStroke", meshStrokeNode::id,
							 meshStrokeNode::creator,
							 meshStrokeNode::initialize);
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

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode);
	msert;

	st = plugin.registerNode("skGraph", skGraphNode::id, skGraphNode::creator,
							 skGraphNode::initialize, MPxNode::kLocatorNode,
							 &skGraphNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId,
		skGraphNodeDrawOverride::Creator);
	mser;

	st = plugin.registerNode("collectStrokes", collectStrokes::id, collectStrokes::creator,
							 collectStrokes::initialize);
	msert;


	
	st = plugin.registerNode("strokeMutator", strokeMutator::id, strokeMutator::creator,
							 strokeMutator::initialize);
	mser;

 
	st = plugin.registerNode("tiltStrokes", tiltStrokes::id, tiltStrokes::creator,
							 tiltStrokes::initialize);
	mser;

	st = plugin.registerNode("aimStrokes", aimStrokes::id, aimStrokes::creator,
							 aimStrokes::initialize);
	mser;

	st = plugin.registerNode("mapStrokes", mapStrokes::id, mapStrokes::creator,
							 mapStrokes::initialize);
	mser;


	st = plugin.registerNode("mapColorStrokes", mapColorStrokes::id, mapColorStrokes::creator,
							 mapColorStrokes::initialize);
	mser;


	st = plugin.registerNode("gateRamp", gateRamp::id, gateRamp::creator,
							 gateRamp::initialize);
	msert;

	st = plugin.registerCommand("paintingQuery", paintingCmd::creator,
								paintingCmd::newSyntax);
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

	st = plugin.deregisterCommand("paintingCmd");
	mser;

	st = plugin.deregisterNode(gateRamp::id);
	mser;
	
 
	st = plugin.deregisterNode(mapColorStrokes::id);
	mser;
	st = plugin.deregisterNode(mapStrokes::id);
	st = plugin.deregisterNode(aimStrokes::id);
	msert;
	st = plugin.deregisterNode(tiltStrokes::id);
	msert;
	msert;
	st = plugin.deregisterNode(strokeMutator::id);
 
	msert;



	st = plugin.deregisterNode(collectStrokes::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(skGraphNode::id);
	mser;

	st = plugin.deregisterNode(brushNode::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		painting::drawDbClassification,
		painting::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(painting::id);
	mser;

	st = plugin.deregisterNode(meshStrokeNode::id);
	mser;

	st = plugin.deregisterNode(skeletonStrokeNode::id);
	mser;

	st = plugin.deregisterNode(curveStrokeNode::id);
	mser;

	st = plugin.deregisterNode(strokeNode::id);
	mser;

	st = plugin.deregisterNode(skChainNode::id);
	mser;

	st = plugin.deregisterData(paintingData::id);
	mser;

	st = plugin.deregisterData(strokeData::id);
	mser;

	st = plugin.deregisterData(brushData::id);
	mser;

	st = plugin.deregisterData(skChainData::id);
	mser;

	return st;
}
