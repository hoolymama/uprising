#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include <maya/MDrawRegistry.h>

#include "collectStrokes.h"
#include "meshStrokeNode.h"

#include "brushData.h"
#include "strokeData.h"
#include "strokeNodeBase.h"

#include "lightPaintingNode.h"
#include "lightPaintingData.h"
#include "brushNode.h"

#include "aimStrokes.h"

#include "mapColorStrokes.h"
#include "strokeMutator.h"

#include "strokeCreator.h"

#include "lightPaintingDrawOverride.h"
MStatus initializePlugin(MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin(obj, PLUGIN_VENDOR, PLUGIN_VERSION, MAYA_VERSION);

	st = plugin.registerData("brushData", brushData::id,
							 brushData::creator);
	mser;

	st = plugin.registerData("strokeData", strokeData::id,
							 strokeData::creator);
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

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode);
	msert;

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


		
	st = plugin.registerNode("strokeMutator", strokeMutator::id, strokeMutator::creator,
							 strokeMutator::initialize);
	mser;

	st = plugin.registerNode("mapColorStrokes", mapColorStrokes::id, mapColorStrokes::creator,
							 mapColorStrokes::initialize);
	mser;


	st = plugin.registerNode("aimStrokes", aimStrokes::id, aimStrokes::creator,
							 aimStrokes::initialize);
	mser;




	return st;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin(obj);

	st = plugin.deregisterNode(aimStrokes::id);
	mser;
	st = plugin.deregisterNode(mapColorStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeMutator::id);
 
	msert;


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

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		lightPainting::drawDbClassification,
		lightPainting::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(lightPainting::id);
	mser;

	st = plugin.deregisterData(lightPaintingData::id);
	mser;

	st = plugin.deregisterData(strokeData::id);
	mser;

	st = plugin.deregisterData(brushData::id);
	mser;

	return st;
}
