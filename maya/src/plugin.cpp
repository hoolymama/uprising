#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "strokeCurveData.h"
#include "paintingData.h"
#include "strokeCurveNode.h"
#include "paintingNode.h"

MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);


	st = plugin.registerData( "strokeCurveData", strokeCurveData::id,
	                          strokeCurveData::creator ); er;


	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); er;


	st = plugin.registerNode( "strokeCurve", strokeCurve::id, strokeCurve::creator,
	                          strokeCurve::initialize); ert;

	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); ert;

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

	st = plugin.deregisterNode( painting::id ); er;

	st = plugin.deregisterNode( strokeCurve::id ); er;

	st = plugin.deregisterData( strokeCurveData::id ); er;

	st = plugin.deregisterData( paintingData::id ); er;


	return st;
}


