#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>

#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

#include <maya/MVectorArray.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnIntArrayData.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnMatrixAttribute.h>

#include "skGraphNode.h"

#include "skChain.h"

#include <jMayaIds.h>
#include <attrUtils.h>

#include "skChainData.h"
#include "cImgUtils.h"

#include "errorMacros.h"

const double PI = 3.141592653;

const double rad_to_deg = (180 / PI);

// const double TAU = 2.0 * PI;

// static int circleVertexCount = 16;
// const float gap = TAU / circleVertexCount;

// static float circle[][4] = {
// 	{sin(gap * 0), cos(gap * 0), 0.0f, 1.0f},
// 	{sin(gap * 1), cos(gap * 1), 0.0f, 1.0f},
// 	{sin(gap * 2), cos(gap * 2), 0.0f, 1.0f},
// 	{sin(gap * 3), cos(gap * 3), 0.0f, 1.0f},
// 	{sin(gap * 4), cos(gap * 4), 0.0f, 1.0f},
// 	{sin(gap * 5), cos(gap * 5), 0.0f, 1.0f},
// 	{sin(gap * 6), cos(gap * 6), 0.0f, 1.0f},
// 	{sin(gap * 7), cos(gap * 7), 0.0f, 1.0f},
// 	{sin(gap * 8), cos(gap * 8), 0.0f, 1.0f},
// 	{sin(gap * 9), cos(gap * 9), 0.0f, 1.0f},
// 	{sin(gap * 10), cos(gap * 10), 0.0f, 1.0f},
// 	{sin(gap * 11), cos(gap * 11), 0.0f, 1.0f},
// 	{sin(gap * 12), cos(gap * 12), 0.0f, 1.0f},
// 	{sin(gap * 13), cos(gap * 13), 0.0f, 1.0f},
// 	{sin(gap * 14), cos(gap * 14), 0.0f, 1.0f},
// 	{sin(gap * 15), cos(gap * 15), 0.0f, 1.0f}};

MObject skGraphNode::aChains;

MObject skGraphNode::aOutPoints;
MObject skGraphNode::aOutParams;
MObject skGraphNode::aOutRadius;
MObject skGraphNode::aOutCounts;

MObject skGraphNode::aPointSize;
MObject skGraphNode::aColor1;
MObject skGraphNode::aColor2;

MObject skGraphNode::aDrawEdges;
MObject skGraphNode::aDrawPoints;
MObject skGraphNode::aDrawCircles;
MObject skGraphNode::aRandomChainColor;

MTypeId skGraphNode::id(k_skGraphNode);
MString skGraphNode::drawDbClassification("drawdb/geometry/skGraphNode");
MString skGraphNode::drawRegistrantId("skGraphNodePlugin");

skGraphNode::skGraphNode() {}

skGraphNode::~skGraphNode() {}

void *skGraphNode::creator()
{
	return new skGraphNode();
}

// const double epsilon = 0.0001;

MStatus skGraphNode::initialize()
{
	MStatus st;
	MString method("skGraphNode::initialize");

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	aChains = tAttr.create("chains", "chn", skChainData::id);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);

	tAttr.setDisconnectBehavior(MFnAttribute::kReset);
	st = addAttribute(aChains);
	mser;


	aOutPoints = tAttr.create("outPoints", "opts", MFnData::kVectorArray);
	tAttr.setStorable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutPoints);

	aOutRadius = tAttr.create("outRadius", "orad", MFnData::kDoubleArray);
	tAttr.setStorable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutRadius);

	aOutParams = tAttr.create("outParams", "opm", MFnData::kDoubleArray);
	tAttr.setStorable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutParams);

	aOutCounts = tAttr.create("outCounts", "ocnt", MFnData::kIntArray);
	tAttr.setStorable(false);
	tAttr.setReadable(true);
	st = addAttribute(aOutCounts);



	aPointSize = nAttr.create("pointSize", "psz", MFnNumericData::kFloat);
	nAttr.setDefault(2.0f);
	nAttr.setKeyable(true);
	st = addAttribute(aPointSize);
	mser;
 
	aColor1 = nAttr.createColor("drawColor1", "dc1");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aColor1);
	mser;

	aColor2 = nAttr.createColor("drawColor2", "dc2");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	st = addAttribute(aColor2);
	mser;
	
	aDrawEdges = nAttr.create("drawEdges", "ded",
							  MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);

	addAttribute(aDrawEdges);

	aDrawPoints = nAttr.create("drawPoints", "dpt",
							   MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	addAttribute(aDrawPoints);

	aDrawCircles = nAttr.create("drawCircles", "dci",
								MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	addAttribute(aDrawCircles);

	aRandomChainColor = nAttr.create("randomChainColor", "rcc",
									 MFnNumericData::kBoolean);
	nAttr.setHidden(false);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	addAttribute(aRandomChainColor);

	attributeAffects(aChains, aOutPoints);
	attributeAffects(aChains, aOutRadius);
	attributeAffects(aChains, aOutCounts);
	attributeAffects(aChains, aOutParams);

	return (MS::kSuccess);
}

MStatus skGraphNode::extract(

	MDataBlock &data, MVectorArray &points, MDoubleArray &params,
	MDoubleArray &radius, MIntArray &counts) const
{

	MStatus st;

	MDataHandle hChains = data.inputValue(aChains, &st);
	msert;
	MObject dChains = hChains.data();
	MFnPluginData fnChains(dChains, &st);
	msert;
	skChainData *scData = (skChainData *)fnChains.data(&st);
	msert;
	const std::vector<skChain> *geom = scData->fGeometry;
	if ((!geom) || geom->size() == 0)
	{
		return MS::kUnknownParameter;
	}

	std::vector<skChain>::const_iterator iter;
	for (iter = geom->begin(); iter != geom->end(); iter++)
	{
		iter->appendPointsTo(points);
		iter->appendParamsTo(params);
		iter->appendRadiiTo(radius);
		counts.append(iter->size());
	}
	return MS::kSuccess;
}

MStatus skGraphNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus st;
	if (!(
			plug == aOutCounts || plug == aOutPoints || plug == aOutRadius || plug == aOutParams))
	{
		return (MS::kUnknownParameter);
	}

	MVectorArray resultPoints;
	MDoubleArray resultParams;
	MDoubleArray resultRadius;
	MIntArray resultCounts;

	extract(data, resultPoints, resultParams, resultRadius, resultCounts);

	outputData(skGraphNode::aOutPoints, data, resultPoints);
	outputData(skGraphNode::aOutParams, data, resultParams);
	outputData(skGraphNode::aOutRadius, data, resultRadius);
	outputData(skGraphNode::aOutCounts, data, resultCounts);

	return MS::kSuccess;
}

void skGraphNode::draw(M3dView &view,
					   const MDagPath &path,
					   M3dView::DisplayStyle style,
					   M3dView::DisplayStatus status)
{
}

bool skGraphNode::isBounded() const
{
	return false;
}

MBoundingBox skGraphNode::boundingBox() const
{
	return MBoundingBox();
}

void skGraphNode::postConstructor()
{
	// setExistWithoutInConnections(true);
	// setExistWithoutOutConnections(true);
}
