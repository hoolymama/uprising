#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include <maya/MDrawRegistry.h>

#include "collectStrokes.h"
#include "meshStrokeNode.h"
#include "skeletonStrokeNode.h"

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
#include "brushNode.h"

#include "aimStrokes.h"
#include "brushLifter.h"

#include "mapColorStrokes.h"
#include "strokeMutator.h"

#include "strokeCreator.h"

#include "skChainData.h"
#include "skChainNode.h"
#include "skGraphNode.h"
#include "paintingDrawOverride.h"
#include "lightPaintingDrawOverride.h"
#include "skGraphNodeDrawOverride.h"

MStatus initializePlugin(MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin(obj, PLUGIN_VENDOR, PLUGIN_VERSION, MAYA_VERSION);

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

	paintingBaseCallback::id = MDGMessage::addNodeAddedCallback(
		paintingBaseCallback::makeDefaultConnections, "painting", NULL, &st);mser;


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

	paintingBaseCallback::id = MDGMessage::addNodeAddedCallback(
		paintingBaseCallback::makeDefaultConnections, "lightPainting", NULL, &st);mser;

	st = plugin.registerNode("skGraph", skGraphNode::id, skGraphNode::creator,
							 skGraphNode::initialize, MPxNode::kLocatorNode,
							 &skGraphNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId,
		skGraphNodeDrawOverride::Creator);
	mser;

	st = plugin.registerNode("skChainNode", skChainNode::id, skChainNode::creator,
							 skChainNode::initialize);
	msert;

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode);
	msert;
	brushNodeCallback::id = MDGMessage::addNodeAddedCallback(
		brushNodeCallback::makeDefaultConnections, "brushNode", NULL, &st);mser;

	st = plugin.registerNode("strokeNodeBase", strokeNodeBase::id, strokeNodeBase::creator,
							 strokeNodeBase::initialize);
	msert;

	st = plugin.registerNode("collectStrokes", collectStrokes::id, collectStrokes::creator,
							 collectStrokes::initialize);
	msert;

	st = plugin.registerNode("strokeCreator", strokeCreator::id, strokeCreator::creator,
							 strokeCreator::initialize);
	mser;

	st = plugin.registerNode("meshStroke", meshStrokeNode::id,
							 meshStrokeNode::creator,
							 meshStrokeNode::initialize);
	msert;

	st = plugin.registerNode("skeletonStroke", skeletonStrokeNode::id,
							 skeletonStrokeNode::creator,
							 skeletonStrokeNode::initialize);
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

	st = plugin.registerNode("brushLifter", brushLifter::id, brushLifter::creator,
							 brushLifter::initialize);
	mser;


	st = plugin.registerCommand("lightPaintingQuery", lightPaintingCmd::creator,
								lightPaintingCmd::newSyntax);
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

	st = plugin.deregisterCommand("lightPaintingCmd");
	mser;
	st = plugin.deregisterNode(brushLifter::id);
	mser;
	st = plugin.deregisterNode(aimStrokes::id);
	mser;
	st = plugin.deregisterNode(mapColorStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeMutator::id);

	msert;

	st = plugin.deregisterNode(skeletonStrokeNode::id);
	mser;

	st = plugin.deregisterNode(meshStrokeNode::id);
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
