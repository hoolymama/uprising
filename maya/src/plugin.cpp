#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "strokeFactory.h"


MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);

	st = plugin.registerNode( "strokeFactory", strokeFactory::id, strokeFactory::creator,
	                          strokeFactory::initialize, MPxNode::kLocatorNode ); ert;
	strokeFactoryCallback::id = MDGMessage::addNodeAddedCallback (
	                              strokeFactoryCallback::makeDefaultConnections, "strokeFactory", NULL,  &st ); er;




	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin( obj );

	st = plugin.deregisterNode( strokeFactory::id ); er;



	return st;
}


