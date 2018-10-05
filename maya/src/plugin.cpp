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
#include "projectionPoints.h"
#include "imgTestCmd.h"
#include "cImgData.h"
#include "cImgNode.h"
#include "cImgShader.h"


MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);

	const MString UserClassifyindexShader("texture/2d:drawdb/shader/texture/2d/indexShader");
	const MString UserClassifycImgShader("texture/2d:drawdb/shader/texture/2d/cImgShader");

	st = plugin.registerData( "strokeGeometryData", strokeGeometryData::id,
	                          strokeGeometryData::creator ); mser;


	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); mser;

	st = plugin.registerData( "cImgData", cImgData::id,
	                          cImgData::creator ); mser;

	st = plugin.registerNode( "cImgNode", cImgNode::id, cImgNode::creator,
	                          cImgNode::initialize); msert;

	st = plugin.registerNode( "strokeNode", strokeNode::id, strokeNode::creator,
	                          strokeNode::initialize); msert;

	st = plugin.registerNode( "strokeCurve", strokeCurve::id, strokeCurve::creator,
	                          strokeCurve::initialize); msert;

	st = plugin.registerNode( "projectStroke", projectStroke::id, projectStroke::creator,
	                          projectStroke::initialize); msert;


	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); msert;

	st = plugin.registerCommand( "paintingQuery", paintingCmd::creator ,
	                             paintingCmd::newSyntax); mser;


	st = plugin.registerCommand( "imgTest", imgTestCmd::creator ,
	                             imgTestCmd::newSyntax); mser;

	st = plugin.registerNode( "indexShader", indexShader::id, &indexShader::creator,
	                          &indexShader::initialize, MPxNode::kDependNode, &UserClassifyindexShader ); mser;

	st = plugin.registerNode( "cImgShader", cImgShader::id, &cImgShader::creator,
	                          &cImgShader::initialize, MPxNode::kDependNode, &UserClassifycImgShader ); mser;


	st = plugin.registerNode( "curveContainment", curveContainment::id,
	                          curveContainment::creator,
	                          curveContainment::initialize); msert;

	st = plugin.registerNode( "projectionPoints", projectionPoints::id,
	                          projectionPoints::creator,
	                          projectionPoints::initialize); msert;

	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin( obj );

	st = plugin.deregisterNode( projectionPoints::id ); mser;

	st = plugin.deregisterNode( curveContainment::id ); mser;

	st = plugin.deregisterNode( cImgShader::id ); mser;

	st = plugin.deregisterNode( indexShader::id ); mser;

	st = plugin.deregisterCommand( "imgTestCmd" ); mser;

	st = plugin.deregisterCommand( "paintingCmd" ); mser;

	st = plugin.deregisterNode( painting::id ); mser;
	st = plugin.deregisterNode( projectStroke::id ); mser;
	st = plugin.deregisterNode( strokeCurve::id ); mser;

	st = plugin.deregisterNode( strokeNode::id ); mser;
	st = plugin.deregisterNode( cImgNode::id ); mser;
	st = plugin.deregisterData( cImgData::id); mser;


	st = plugin.deregisterData( paintingData::id ); mser;

	st = plugin.deregisterData( strokeGeometryData::id ); mser;


	return st;
}


