#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "strokeGeometryData.h"
#include "paintingData.h"
#include "strokeCurveNode.h"
#include "paintingNode.h"
#include "paintingCmd.h"

MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);


	st = plugin.registerData( "strokeGeometryData", strokeGeometryData::id,
	                          strokeGeometryData::creator ); er;


	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); er;


	st = plugin.registerNode( "strokeNode", strokeNode::id, strokeNode::creator,
	                          strokeNode::initialize); ert;

	st = plugin.registerNode( "strokeCurve", strokeCurve::id, strokeCurve::creator,
	                          strokeCurve::initialize); ert;

	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); ert;

	st = plugin.registerCommand( "paintingQuery", paintingCmd::creator ,
	                             paintingCmd::newSyntax); er;
	// strokeFactoryCallback::id = MDGMessage::addNodeAddedCallback (
	//                               strokeFactoryCallback::makeDefaultConnections, "strokeFactory", NULL,  &st ); er;




	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin( obj );


	st = plugin.deregisterCommand( "paintingCmd" ); er;

	st = plugin.deregisterNode( painting::id ); er;

	st = plugin.deregisterNode( strokeCurve::id ); er;

	st = plugin.deregisterNode( strokeNode::id ); er;

	st = plugin.deregisterData( paintingData::id ); er;

	st = plugin.deregisterData( strokeGeometryData::id ); er;


	return st;
}


