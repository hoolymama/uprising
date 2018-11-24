#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>


#include "strokeData.h"
#include "paintingData.h"
#include "curveStrokeNode.h"
#include "paintingNode.h"


#include "cImgData.h"
#include "cImgFile.h"

#include "cImgProcess.h"
#include "cImgShader.h"

// #include "paintingCmd.h"

// #include "flowStrokeNode.h"
// #include "indexShader.h"
// #include "curveContainment.h"
// #include "projectionPoints.h"
// #include "imgTestCmd.h"




// #include "cImgExpr.h"
// #include "cImgNode.h"
// #include "cImgDetail.h"
// #include "cImgQuantize.h"
// #include "cImgDistinct.h"
// #include "cImgGradSampler.h"
// #include "cImgUVSampler.h"

// #include "cImgNoise.h"
// #include "cImgIndexSampler.h"
// #include "cImgPngFile.h"
// #include "pngInfo.h"
MStatus initializePlugin( MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_VENDOR, PLUGIN_VERSION , MAYA_VERSION);

	const MString UserClassifyindexShader("texture/2d:drawdb/shader/texture/2d/indexShader");
	const MString UserClassifycImgShader("texture/2d:drawdb/shader/texture/2d/cImgShader");


	st = plugin.registerData( "strokeData", strokeData::id,
	                          strokeData::creator ); mser;


	st = plugin.registerData( "paintingData", paintingData::id,
	                          paintingData::creator ); mser;


	st = plugin.registerData( "cImgData", cImgData::id,
	                          cImgData::creator ); mser;



	st = plugin.registerNode( "cImgFile", cImgFile::id, cImgFile::creator,
	                          cImgFile::initialize); msert;

	st = plugin.registerNode( "cImgProcess", cImgProcess::id, cImgProcess::creator,
	                          cImgProcess::initialize); msert;



	st = plugin.registerNode( "cImgShader", cImgShader::id, &cImgShader::creator,
	                          &cImgShader::initialize, MPxNode::kDependNode, &UserClassifycImgShader ); mser;


	st = plugin.registerNode( "strokeNode", strokeNode::id, strokeNode::creator,
	                          strokeNode::initialize); msert;

	st = plugin.registerNode( "curveStrokes", curveStrokeNode::id, curveStrokeNode::creator,
	                          curveStrokeNode::initialize); msert;

	// st = plugin.registerNode( "flowStrokes", flowStrokeNode::id, flowStrokeNode::creator,
	//                           flowStrokeNode::initialize); msert;


	st = plugin.registerNode( "painting", painting::id, painting::creator,
	                          painting::initialize, MPxNode::kLocatorNode ); msert;

	// st = plugin.registerCommand( "paintingQuery", paintingCmd::creator ,
	//                              paintingCmd::newSyntax); mser;


	// st = plugin.registerCommand( "pngInfo", pngInfo::creator ,
	//                              pngInfo::newSyntax); mser;


	// st = plugin.registerNode( "curveContainment", curveContainment::id,
	//                           curveContainment::creator,
	//                           curveContainment::initialize); msert;

	// st = plugin.registerNode( "projectionPoints", projectionPoints::id,
	//                           projectionPoints::creator,
	//                           projectionPoints::initialize, MPxNode::kLocatorNode); msert;





	MGlobal::executePythonCommand("import uprising;uprising.load()");

	return st;

}

MStatus uninitializePlugin( MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");


	MFnPlugin plugin( obj );

	// st = plugin.deregisterNode( curveContainment::id ); mser;


	// st = plugin.deregisterCommand( "pngInfo" ); mser;

	// st = plugin.deregisterCommand( "paintingCmd" ); mser;

	st = plugin.deregisterNode( painting::id ); mser;

	// st = plugin.deregisterNode( flowStroke::id ); mser;

	st = plugin.deregisterNode( curveStrokeNode::id ); mser;

	st = plugin.deregisterNode( strokeNode::id ); mser;

	st = plugin.deregisterNode( cImgShader::id ); mser;


	// st = plugin.deregisterNode( cImgNoise::id); mser;

	// st = plugin.deregisterNode( cImgDetail::id); mser;

	st = plugin.deregisterNode( cImgProcess::id); mser;

	// st = plugin.deregisterNode( cImgPngFile::id); mser;

	// st = plugin.deregisterNode( cImgExpr::id); mser;

	st = plugin.deregisterNode( cImgFile::id); mser;

	st = plugin.deregisterData( cImgData::id); mser;

	st = plugin.deregisterData( paintingData::id ); mser;

	st = plugin.deregisterData( strokeData::id ); mser;


	return st;
}


