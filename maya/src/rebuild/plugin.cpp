#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include "strokeData.h"
#include "paintingData.h"
#include "curveStrokeNode.h"



#include "skGraphNode.h"

#include "skeletonStrokeNode.h"
#include "paintingNode.h"
#include "collectStrokes.h"
#include "paintingCmd.h"

MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);

	const MString UserClassifyindexShader("texture/2d:drawdb/shader/texture/2d/indexShader");
	const MString UserClassifycImgShader("texture/2d:drawdb/shader/texture/2d/cImgShader");

	MGlobal::executePythonCommand("import pymel.core;pymel.core.loadPlugin('Kit')");

	st = plugin.registerData( "strokeData", strokeData::id,
	                          strokeData::creator ); mser;

	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); mser;

	st = plugin.registerNode( "strokeNode", strokeNode::id, strokeNode::creator,
	                          strokeNode::initialize); msert;

	st = plugin.registerNode( "curveStroke", curveStrokeNode::id, curveStrokeNode::creator,
	                          curveStrokeNode::initialize); msert;



	st = plugin.registerNode( "skeletonStroke", skeletonStrokeNode::id,
	                          skeletonStrokeNode::creator,
	                          skeletonStrokeNode::initialize); msert;


	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); msert;

	st = plugin.registerNode( "skGraph", skGraphNode::id, skGraphNode::creator,
	                          skGraphNode::initialize, MPxNode::kLocatorNode ); msert;



	st = plugin.registerNode( "collectStrokes", collectStrokes::id, collectStrokes::creator,
	                          collectStrokes::initialize ); msert;

	st = plugin.registerCommand( "paintingQuery", paintingCmd::creator ,
	                             paintingCmd::newSyntax); mser;

	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");


	MFnPlugin plugin( obj );

	st = plugin.deregisterCommand( "paintingCmd" ); mser;

	st = plugin.deregisterNode( collectStrokes::id ); mser;


	st = plugin.deregisterNode( skGraphNode::id ); mser;

	st = plugin.deregisterNode( painting::id ); mser;

	st = plugin.deregisterNode( skeletonStrokeNode::id ); mser;

	st = plugin.deregisterNode( curveStrokeNode::id ); mser;

	st = plugin.deregisterNode( strokeNode::id ); mser;

	st = plugin.deregisterData( paintingData::id ); mser;

	st = plugin.deregisterData( strokeData::id ); mser;


	return st;
}


