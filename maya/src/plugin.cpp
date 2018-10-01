#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "strokeGeometryData.h"
#include "paintingData.h"
#include "strokeCurveNode.h"
#include "paintingNode.h"
#include "paintingCmd.h"

#include "projectStrokeNode.h"
#include "indexShader.h"
#include "curveContainment.h"

MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);

	const MString UserClassifyindexShader("texture/2d:drawdb/shader/texture/2d/indexShader");

	st = plugin.registerData( "strokeGeometryData", strokeGeometryData::id,
	                          strokeGeometryData::creator ); er;


	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); er;


	st = plugin.registerNode( "strokeNode", strokeNode::id, strokeNode::creator,
	                          strokeNode::initialize); ert;

	st = plugin.registerNode( "strokeCurve", strokeCurve::id, strokeCurve::creator,
	                          strokeCurve::initialize); ert;

	st = plugin.registerNode( "projectStroke", projectStroke::id, projectStroke::creator,
	                          projectStroke::initialize); ert;


	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); ert;

	st = plugin.registerCommand( "paintingQuery", paintingCmd::creator ,
	                             paintingCmd::newSyntax); er;

	st = plugin.registerNode( "indexShader", indexShader::id, &indexShader::creator,
	                          &indexShader::initialize, MPxNode::kDependNode, &UserClassifyindexShader ); er;

	st = plugin.registerNode( "curveContainment", curveContainment::id,
	                          curveContainment::creator,
	                          curveContainment::initialize); ert;


	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin( obj );

	st = plugin.deregisterNode( curveContainment::id ); er;

	st = plugin.deregisterNode( indexShader::id ); er;

	st = plugin.deregisterCommand( "paintingCmd" ); er;

	st = plugin.deregisterNode( painting::id ); er;
	st = plugin.deregisterNode( projectStroke::id ); er;
	st = plugin.deregisterNode( strokeCurve::id ); er;

	st = plugin.deregisterNode( strokeNode::id ); er;

	st = plugin.deregisterData( paintingData::id ); er;

	st = plugin.deregisterData( strokeGeometryData::id ); er;


	return st;
}


