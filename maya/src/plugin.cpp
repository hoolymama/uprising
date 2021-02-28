#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>


#include "brushData.h"
#include "strokeData.h"

#include "lightPaintingNode.h"
#include "lightPaintingData.h"
#include "brushNode.h"


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

	// st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
	// 	lightPainting::drawDbClassification,
	// 	lightPainting::drawRegistrantId,
	// 	lightPaintingDrawOverride::Creator);
	// mser;

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode);
	msert;

	return st;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin(obj);


	st = plugin.deregisterNode(brushNode::id);
	mser;

	// st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
	// 	lightPainting::drawDbClassification,
	// 	lightPainting::drawRegistrantId);
	// mser;

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
