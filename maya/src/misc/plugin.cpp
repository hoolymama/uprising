#include <maya/MStatus.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <errorMacros.h>
#include <maya/MDrawRegistry.h>

#include "collectStrokes.h"
#include "meshStrokeNode.h"

#include "skeletonStrokeNode.h"
#include "curveStrokeNode.h"

#include "brushCmd.h"
#include "brushData.h"

#include "brushShopData.h"

#include "paintingNodeBase.h"

#include "paintingNode.h"
#include "paintingData.h"

#include "strokeData.h"
#include "strokeNodeBase.h"

#include "lightPaintingNode.h"
#include "lightPaintingData.h"
#include "lightPaintingCmd.h"

#include "paintingCmd.h"
#include "strokeCmd.h"

#include "brushNode.h"
#include "brushDrawOverride.h"

#include "brushShopNode.h"

#include "aimStrokes.h"
#include "mirrorStrokes.h"

#include "displaceStrokes.h"
#include "brushLifter.h"

#include "tiltStrokes.h"
#include "mapColorStrokes.h"
#include "decimateStrokes.h"
#include "scribbleStrokes.h"
// #include "testMutateStrokes.h"
#include "strokeMutator.h"

#include "strokeCreator.h"
#include "paintStrokeCreator.h"
#include "skChainData.h"
#include "skChainNode.h"

#include "imagePoints.h"
#include "imagePointsDrawOverride.h"

#include "diptych.h"
#include "diptychDrawOverride.h"



#include "skGraphNode.h"
#include "skGraphNodeDrawOverride.h"

#include "paintingDrawOverride.h"
#include "lightPaintingDrawOverride.h"

#include "paletteData.h"
#include "paletteNode.h"
#include "colorSpreadNode.h"
#include "paletteDrawOverride.h"

#include "cImgGradField.h"
#include "gridableField.h"

#include "mapIdStrokes.h"

#include "gateRamp.h"
#include "stepRamp.h"

#include "rotateTargets.h"

#include "bulgeStrokes.h"


#include "particleStrokeNode.h"
#include "pearlStrokeNode.h"
#include "hatchStrokes.h"

#include "particleTrailsNode.h"
#include "particleTrailsData.h"

#include "cImgData.h"
#include "cImgFloatData.h"
#include "imageMeasureCmd.h"
#include "cImgShader.h"
#include "cImgFile.h"
#include "cImgFileSplit.h"
#include "cImgConstant.h"
#include "cImgProcess.h"
#include "cImgSkel.h"
#include "cImgGate.h"
#include "cImgDetail.h"
#include "cImgBlur.h"
#include "cImgMinMax.h"
#include "cImgNoise.h"
#include "cImgDilate.h"
#include "axisImg.h"
#include "cImgSmear.h"
#include "cImgQuantize.h"

#include "cImgPngFile.h"
#include "pngInfo.h"
#include "cImgOverride.h"
#include "cImgMult.h"
#include "cImgMerge.h"
#include "cImgGradient.h"
#include "cImgLayout.h"
#include "cImgChoice.h"
#include "cImgReverse.h"
#include "cImgFileCrop.h"
#include "cImgCrop.h"
#include "cImgFloatCrop.h"
#include "cImgDistance.h"
#include "cImgFloatGate.h"
#include "cImgSetRange.h"
#include "cImgMergeDilate.h"

#include "multVectorDoublePP.h"
#include "multVectorVectorPP.h"
#include "multDoubleDoublePP.h"
#include "addDoublePP.h"
#include "addVectorPP.h"
#include "projectToPlanePP.h"
#include "tumbler.h"

#include "pearlChainData.h"
#include "pearlNode.h"

#include "strokeMesh.h"

#include "pearlNodeDrawOverride.h"
#include "paletteOrder.h"

#include "funCurve.h"



static const MString sRegistrantId("cImgPlugin");

MStatus initializePlugin(MObject obj)
{

	MStatus st;

	MString method("initializePlugin");

	MFnPlugin plugin(obj, PLUGIN_VENDOR, PLUGIN_VERSION, MAYA_VERSION);

	// BUILD UI
	MGlobal::executePythonCommandOnIdle("from uprising import menu;menu.UprisingMenu()", true);

	const MString cImgShaderUserClassify("texture/2d:drawdb/shader/texture/2d/cImgShader");

	st = plugin.registerData("cImgData", cImgData::id,
							 cImgData::creator);
	mser;

	st = plugin.registerData("cImgFloatData", cImgFloatData::id,
							 cImgFloatData::creator);
	mser;

	st = plugin.registerNode("cImgConstant", cImgConstant::id, cImgConstant::creator,
							 cImgConstant::initialize);
	msert;

	st = plugin.registerNode("cImgFileSplit", cImgFileSplit::id, cImgFileSplit::creator,
							 cImgFileSplit::initialize);
	msert;

	st = plugin.registerNode("cImgFile", cImgFile::id, cImgFile::creator,
							 cImgFile::initialize);
	msert;

	st = plugin.registerNode("cImgPngFile", cImgPngFile::id, cImgPngFile::creator,
							 cImgPngFile::initialize);
	msert;

	st = plugin.registerNode("cImgGradient", cImgGradient::id, cImgGradient::creator,
							 cImgGradient::initialize);
	msert;

	st = plugin.registerNode("cImgProcess", cImgProcess::id, cImgProcess::creator,
							 cImgProcess::initialize);
	msert;

	st = plugin.registerNode("cImgMerge", cImgMerge::id, cImgMerge::creator,
							 cImgMerge::initialize);
	msert;

	st = plugin.registerNode("cImgSkel", cImgSkel::id, cImgSkel::creator,
							 cImgSkel::initialize);
	msert;

	st = plugin.registerNode("cImgGate", cImgGate::id, cImgGate::creator,
							 cImgGate::initialize);
	msert;
	st = plugin.registerNode("cImgFloatGate", cImgFloatGate::id, cImgFloatGate::creator,
							 cImgFloatGate::initialize);
	msert;

	st = plugin.registerNode("cImgSmear", cImgSmear::id, cImgSmear::creator,
							 cImgSmear::initialize);
	msert;


	st = plugin.registerNode("cImgQuantize", cImgQuantize::id, cImgQuantize::creator,
							 cImgQuantize::initialize);
	msert;


	

	st = plugin.registerNode("cImgDetail", cImgDetail::id, cImgDetail::creator,
							 cImgDetail::initialize);
	msert;

	st = plugin.registerNode("cImgBlur", cImgBlur::id, cImgBlur::creator,
							 cImgBlur::initialize);
	msert;

	st = plugin.registerNode("cImgDilate", cImgDilate::id,
							 cImgDilate::creator,
							 cImgDilate::initialize);
	msert;

	st = plugin.registerNode("cImgSetRange", cImgSetRange::id,
							 cImgSetRange::creator,
							 cImgSetRange::initialize);
	msert;

	st = plugin.registerNode("cImgMergeDilate", cImgMergeDilate::id,
							 cImgMergeDilate::creator,
							 cImgMergeDilate::initialize);
	msert;

	st = plugin.registerNode("cImgDistance", cImgDistance::id,
							 cImgDistance::creator,
							 cImgDistance::initialize);
	msert;

	st = plugin.registerNode("cImgReverse", cImgReverse::id,
							 cImgReverse::creator,
							 cImgReverse::initialize);
	msert;

	st = plugin.registerNode("cImgMult", cImgMult::id,
							 cImgMult::creator,
							 cImgMult::initialize);
	msert;

	st = plugin.registerNode("cImgMinMax", cImgMinMax::id,
							 cImgMinMax::creator,
							 cImgMinMax::initialize);
	msert;

	st = plugin.registerNode("cImgNoise", cImgNoise::id,
							 cImgNoise::creator,
							 cImgNoise::initialize);
	msert;

	st = plugin.registerNode("axisImg", axisImg::id, axisImg::creator,
							 axisImg::initialize);
	msert;

	st = plugin.registerNode("cImgLayout", cImgLayout::id,
							 cImgLayout::creator,
							 cImgLayout::initialize);
	msert;

	st = plugin.registerNode("cImgChoice", cImgChoice::id,
							 cImgChoice::creator,
							 cImgChoice::initialize);
	msert;

	st = plugin.registerNode("cImgFileCrop", cImgFileCrop::id,
							 cImgFileCrop::creator,
							 cImgFileCrop::initialize);
	msert;

	st = plugin.registerNode("cImgCrop", cImgCrop::id,
							 cImgCrop::creator,
							 cImgCrop::initialize);
	msert;

	st = plugin.registerNode("cImgFloatCrop", cImgFloatCrop::id,
							 cImgFloatCrop::creator,
							 cImgFloatCrop::initialize);
	msert;

	st = plugin.registerNode("cImgShader", cImgShader::id, &cImgShader::creator,
							 &cImgShader::initialize, MPxNode::kDependNode, &cImgShaderUserClassify);
	mser;

	st = plugin.registerCommand("pngInfo", pngInfo::creator,
								pngInfo::newSyntax);
	mser;

	st = plugin.registerCommand("imageMeasure", imageMeasureCmd::creator,
								imageMeasureCmd::newSyntax);
	mser;

	MHWRender::MDrawRegistry::registerShadingNodeOverrideCreator(
		"drawdb/shader/texture/2d/cImgShader",
		sRegistrantId,
		cImgOverride::creator);

	////////////////////////////////////////////////////////////////////////

	st = plugin.registerData("pearlChainData", pearlChainData::id,
							 pearlChainData::creator);
	mser;

	st = plugin.registerData("particleTrailsData", particleTrailsData::id,
							 particleTrailsData::creator);
	mser;

	st = plugin.registerData("skChainData", skChainData::id,
							 skChainData::creator);
	mser;

	st = plugin.registerData("brushData", brushData::id,
							 brushData::creator);
	mser;

	st = plugin.registerData("brushShopData", brushShopData::id,
							 brushShopData::creator);
	mser;



	st = plugin.registerData("paletteData", paletteData::id,
							 paletteData::creator);
	mser;

	st = plugin.registerData("strokeData", strokeData::id,
							 strokeData::creator);
	mser;

	st = plugin.registerData("paintingData", paintingData::id,
							 paintingData::creator);

	st = plugin.registerNode("gridableField", gridableField::id,
							 gridableField::creator,
							 gridableField::initialize,
							 MPxNode::kFieldNode);
	mser;

	st = plugin.registerNode("cImgGradField", cImgGradField::id,
							 cImgGradField::creator,
							 cImgGradField::initialize,
							 MPxNode::kFieldNode);
	mser;

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

	st = plugin.registerNode("skGraph", skGraphNode::id, skGraphNode::creator,
							 skGraphNode::initialize, MPxNode::kLocatorNode,
							 &skGraphNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		skGraphNode::drawDbClassification,
		skGraphNode::drawRegistrantId,
		skGraphNodeDrawOverride::Creator);
	mser;



	st = plugin.registerNode("paletteOrder", paletteOrder::id, paletteOrder::creator, paletteOrder::initialize);


	st = plugin.registerNode("palette", paletteNode::id, paletteNode::creator,
							 paletteNode::initialize, MPxNode::kLocatorNode,
							 &paletteNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		paletteNode::drawDbClassification,
		paletteNode::drawRegistrantId,
		PaletteDrawOverride::Creator);
	mser;

	st = plugin.registerNode("pearlNode", pearlNode::id, pearlNode::creator,
							 pearlNode::initialize, MPxNode::kLocatorNode,
							 &pearlNode::drawDbClassification);
	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		pearlNode::drawDbClassification,
		pearlNode::drawRegistrantId,
		pearlNodeDrawOverride::Creator);
	mser;

	st = plugin.registerNode("particleTrails", particleTrailsNode::id, particleTrailsNode::creator,
							 particleTrailsNode::initialize);

	st = plugin.registerNode("colorSpread", colorSpreadNode::id, colorSpreadNode::creator,
							 colorSpreadNode::initialize);
	msert;
	st = plugin.registerNode("skChainNode", skChainNode::id, skChainNode::creator,
							 skChainNode::initialize);
	msert;

	st = plugin.registerNode("brushNode", brushNode::id, brushNode::creator,
							 brushNode::initialize, MPxNode::kLocatorNode,  &brushNode::drawDbClassification);

	brushNodeCallback::id = MDGMessage::addNodeAddedCallback ( 
		brushNodeCallback::makeDefaultConnections, "brushNode", NULL);

	msert;

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		brushNode::drawDbClassification,
		brushNode::drawRegistrantId,
		brushDrawOverride::Creator);
	mser;



	st = plugin.registerNode("brushShopNode", brushShopNode::id, brushShopNode::creator,
							 brushShopNode::initialize);
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

	st = plugin.registerNode("paintStrokeCreator", paintStrokeCreator::id, paintStrokeCreator::creator,
							 paintStrokeCreator::initialize);
	mser;

	st = plugin.registerNode("particleStroke", particleStrokeNode::id,
							 particleStrokeNode::creator,
							 particleStrokeNode::initialize);
	msert;

	st = plugin.registerNode("pearlStroke", pearlStrokeNode::id,
							 pearlStrokeNode::creator,
							 pearlStrokeNode::initialize);
	msert;



	st = plugin.registerNode("meshStroke", meshStrokeNode::id,
							 meshStrokeNode::creator,
							 meshStrokeNode::initialize);
	msert;

	st = plugin.registerNode("skeletonStroke", skeletonStrokeNode::id,
							 skeletonStrokeNode::creator,
							 skeletonStrokeNode::initialize);
	msert;

	st = plugin.registerNode("curveStroke", curveStrokeNode::id, curveStrokeNode::creator,
							 curveStrokeNode::initialize);
	msert;

	st = plugin.registerNode("strokeMutator", strokeMutator::id, strokeMutator::creator,
							 strokeMutator::initialize);
	mser;

	st = plugin.registerNode("mapColorStrokes", mapColorStrokes::id, mapColorStrokes::creator,
							 mapColorStrokes::initialize);
	mser;

	st = plugin.registerNode("mapIdStrokes", mapIdStrokes::id, mapIdStrokes::creator,
							 mapIdStrokes::initialize);
	mser;

	st = plugin.registerNode("decimateStrokes", decimateStrokes::id, decimateStrokes::creator,
							 decimateStrokes::initialize);
	mser;

	st = plugin.registerNode("scribbleStrokes", scribbleStrokes::id, scribbleStrokes::creator,
							 scribbleStrokes::initialize);
	mser;


	// st = plugin.registerNode("testMutateStrokes", testMutateStrokes::id, testMutateStrokes::creator,
	// 						 testMutateStrokes::initialize);
	// mser;
	
	st = plugin.registerNode("hatchStrokes", hatchStrokes::id,
							 hatchStrokes::creator,
							 hatchStrokes::initialize);
	msert;


	st = plugin.registerNode("aimStrokes", aimStrokes::id, aimStrokes::creator,
							 aimStrokes::initialize);
	mser;

	st = plugin.registerNode("mirrorStrokes", mirrorStrokes::id, mirrorStrokes::creator,
							 mirrorStrokes::initialize);
	mser;


	st = plugin.registerNode("tiltStrokes", tiltStrokes::id, tiltStrokes::creator,
							 tiltStrokes::initialize);
	mser;

	st = plugin.registerNode("bulgeStrokes", bulgeStrokes::id, bulgeStrokes::creator,
							 bulgeStrokes::initialize);
	mser;

	// st = plugin.registerNode("mapStrokes", mapStrokes::id, mapStrokes::creator,
	// 						 mapStrokes::initialize);
	// mser;

	st = plugin.registerNode("rotateTargets", rotateTargets::id, rotateTargets::creator,
							 rotateTargets::initialize);
	mser;

	st = plugin.registerNode("displaceStrokes", displaceStrokes::id, displaceStrokes::creator,
							 displaceStrokes::initialize);
	mser;

	st = plugin.registerNode("brushLifter", brushLifter::id, brushLifter::creator,
							 brushLifter::initialize);
	mser;

	st = plugin.registerNode("gateRamp", gateRamp::id, gateRamp::creator,
							 gateRamp::initialize);
	msert;

	st = plugin.registerNode("stepRamp", stepRamp::id, stepRamp::creator,
							 stepRamp::initialize);
	msert;

	st = plugin.registerNode("imagePoints", imagePoints::id, imagePoints::creator,
							 imagePoints::initialize, MPxNode::kLocatorNode,
							 &imagePoints::drawDbClassification);

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		imagePoints::drawDbClassification,
		imagePoints::drawRegistrantId,
		imagePointsDrawOverride::Creator);
	mser;

	st = plugin.registerNode("diptych", diptych::id, diptych::creator,
							 diptych::initialize, MPxNode::kLocatorNode,
							 &diptych::drawDbClassification);

	st = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		diptych::drawDbClassification,
		diptych::drawRegistrantId,
		diptychDrawOverride::Creator);
	mser;

	st = plugin.registerCommand("lightPaintingQuery", lightPaintingCmd::creator,
								lightPaintingCmd::newSyntax);
	mser;

	st = plugin.registerCommand("paintingQuery", paintingCmd::creator,
								paintingCmd::newSyntax);
	mser;

	st = plugin.registerCommand("strokeQuery", strokeCmd::creator,
								strokeCmd::newSyntax);
	mser;

	st = plugin.registerCommand("brushQuery", brushCmd::creator,
								brushCmd::newSyntax);
	mser;

	st = plugin.registerNode("addDoublePP", addDoublePP::id, addDoublePP::creator,
							 addDoublePP::initialize);
	mser;
	st = plugin.registerNode("addVectorPP", addVectorPP::id, addVectorPP::creator,
							 addVectorPP::initialize);
	mser;
	st = plugin.registerNode("multDoubleDoublePP", multDoubleDoublePP::id,
							 multDoubleDoublePP::creator, multDoubleDoublePP::initialize);
	mser;
	st = plugin.registerNode("multVectorDoublePP", multVectorDoublePP::id,
							 multVectorDoublePP::creator, multVectorDoublePP::initialize);
	mser;
	st = plugin.registerNode("multVectorVectorPP", multVectorVectorPP::id,
							 multVectorVectorPP::creator, multVectorVectorPP::initialize);
	mser;

	st = plugin.registerNode("projectToPlanePP", projectToPlanePP::id,
							 projectToPlanePP::creator, projectToPlanePP::initialize);
	mser;

	st = plugin.registerNode("tumbler", tumbler::id, tumbler::creator, tumbler::initialize);
	mser;

	st = plugin.registerNode("strokeMesh", strokeMesh::id, strokeMesh::creator,
							 strokeMesh::initialize);
	msert;

	st = plugin.registerNode("funCurve", funCurve::id, funCurve::creator,
							 funCurve::initialize);
	msert;

	return st;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus st;

	MString method("uninitializePlugin");

	MFnPlugin plugin(obj);


	st = plugin.deregisterCommand("funCurve");
	mser;
	st = plugin.deregisterCommand("strokeMesh");
	mser;
	st = plugin.deregisterCommand("tumbler");
	mser;
	st = plugin.deregisterCommand("projectToPlanePP");
	mser;
	st = plugin.deregisterCommand("multVectorVectorPP");
	mser;
	st = plugin.deregisterCommand("multVectorDoublePP");
	mser;
	st = plugin.deregisterCommand("multDoubleDoublePP");
	mser;
	st = plugin.deregisterCommand("addVectorPP");
	mser;
	st = plugin.deregisterCommand("addDoublePP");
	mser;

	st = plugin.deregisterCommand("brushCmd");
	mser;

	st = plugin.deregisterCommand("strokeCmd");
	mser;

	st = plugin.deregisterCommand("paintingCmd");
	mser;

	st = plugin.deregisterCommand("lightPaintingCmd");
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		diptych::drawDbClassification,
		diptych::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(diptych::id);
	mser;


	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		imagePoints::drawDbClassification,
		imagePoints::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(imagePoints::id);
	mser;

	st = plugin.deregisterNode(stepRamp::id);
	mser;

	st = plugin.deregisterNode(gateRamp::id);
	mser;

	st = plugin.deregisterNode(brushLifter::id);
	mser;

	st = plugin.deregisterNode(displaceStrokes::id);
	mser;

	st = plugin.deregisterNode(rotateTargets::id);
	mser;

	// st = plugin.deregisterNode(mapStrokes::id);
	// mser;

	st = plugin.deregisterNode(bulgeStrokes::id);
	mser;

	st = plugin.deregisterNode(tiltStrokes::id);
	mser;

	st = plugin.deregisterNode(mirrorStrokes::id);
	mser;

	st = plugin.deregisterNode(aimStrokes::id);
	mser;

	st = plugin.deregisterNode(hatchStrokes::id);
	mser;

	// st = plugin.deregisterNode(testMutateStrokes::id);
	// mser;

	st = plugin.deregisterNode(scribbleStrokes::id);
	mser;

	st = plugin.deregisterNode(decimateStrokes::id);
	mser;

	st = plugin.deregisterNode(mapIdStrokes::id);
	mser;

	st = plugin.deregisterNode(mapColorStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeMutator::id);
	msert;

	st = plugin.deregisterNode(curveStrokeNode::id);
	mser;

	st = plugin.deregisterNode(skeletonStrokeNode::id);
	mser;

	st = plugin.deregisterNode(meshStrokeNode::id);
	mser;


	st = plugin.deregisterNode(pearlStrokeNode::id);
	mser;

	st = plugin.deregisterNode(particleStrokeNode::id);
	mser;

	st = plugin.deregisterNode(paintStrokeCreator::id);
	mser;

	st = plugin.deregisterNode(strokeCreator::id);
	mser;

	st = plugin.deregisterNode(collectStrokes::id);
	mser;

	st = plugin.deregisterNode(strokeNodeBase::id);
	mser;

	st = plugin.deregisterNode(brushShopNode::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		brushNode::drawDbClassification,
		brushNode::drawRegistrantId);
	mser;


	st = plugin.deregisterNode(brushNode::id);
	mser;

	st = plugin.deregisterNode(skChainNode::id);
	mser;

	st = plugin.deregisterNode(colorSpreadNode::id);
	mser;

	st = plugin.deregisterNode(particleTrailsNode::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		pearlNode::drawDbClassification,
		pearlNode::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(pearlNode::id);
	mser;

	st = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		paletteNode::drawDbClassification,
		paletteNode::drawRegistrantId);
	mser;

	st = plugin.deregisterNode(paletteNode::id);
	mser;

	st = plugin.deregisterNode(paletteOrder::id);
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

	st = plugin.deregisterNode(cImgGradField::id);
	mser;

	st = plugin.deregisterNode(gridableField::id);
	mser;

	st = plugin.deregisterData(paintingData::id);
	mser;

	st = plugin.deregisterData(strokeData::id);
	mser;

	st = plugin.deregisterData(paletteData::id);
	mser;

	st = plugin.deregisterData(brushShopData::id);

	st = plugin.deregisterData(brushData::id);
	mser;

	st = plugin.deregisterData(skChainData::id);
	mser;

	st = plugin.deregisterData(particleTrailsData::id);
	mser;

	st = plugin.deregisterData(pearlChainData::id);
	mser;

	////////////////////////////////////

	MHWRender::MDrawRegistry::deregisterShadingNodeOverrideCreator("drawdb/shader/texture/2d/cImgShader",
																   sRegistrantId);

	st = plugin.deregisterCommand("imageMeasureCmd");
	mser;

	st = plugin.deregisterCommand("pngInfo");
	mser;

	st = plugin.deregisterNode(cImgShader::id);
	mser;

	st = plugin.deregisterNode(cImgFloatCrop::id);
	mser;

	st = plugin.deregisterNode(cImgCrop::id);
	mser;

	st = plugin.deregisterNode(cImgFileCrop::id);
	mser;

	st = plugin.deregisterNode(cImgChoice::id);
	mser;

	st = plugin.deregisterNode(cImgLayout::id);
	mser;

	st = plugin.deregisterNode(axisImg::id);
	mser;

	st = plugin.deregisterNode(cImgNoise::id);
	mser;

	st = plugin.deregisterNode(cImgMinMax::id);
	mser;

	st = plugin.deregisterNode(cImgMult::id);
	mser;

	st = plugin.deregisterNode(cImgReverse::id);
	mser;

	st = plugin.deregisterNode(cImgDistance::id);
	mser;

	st = plugin.deregisterNode(cImgMergeDilate::id);
	mser;

	st = plugin.deregisterNode(cImgSetRange::id);
	mser;

	st = plugin.deregisterNode(cImgDilate::id);
	mser;

	st = plugin.deregisterNode(cImgBlur::id);
	mser;

	st = plugin.deregisterNode(cImgDetail::id);
	mser;

	st = plugin.deregisterNode(cImgQuantize::id);
	mser;

	st = plugin.deregisterNode(cImgSmear::id);
	mser;

	st = plugin.deregisterNode(cImgFloatGate::id);
	mser;

	st = plugin.deregisterNode(cImgGate::id);
	mser;

	st = plugin.deregisterNode(cImgSkel::id);
	mser;

	st = plugin.deregisterNode(cImgMerge::id);
	mser;

	st = plugin.deregisterNode(cImgProcess::id);
	mser;

	st = plugin.deregisterNode(cImgGradient::id);
	mser;

	st = plugin.deregisterNode(cImgPngFile::id);
	mser;

	st = plugin.deregisterNode(cImgFile::id);
	mser;

	st = plugin.deregisterNode(cImgFileSplit::id);
	mser;

	st = plugin.deregisterNode(cImgConstant::id);
	mser;

	st = plugin.deregisterData(cImgFloatData::id);
	mser;

	st = plugin.deregisterData(cImgData::id);
	mser;

	return st;
}
