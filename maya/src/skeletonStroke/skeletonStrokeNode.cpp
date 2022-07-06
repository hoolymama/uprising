
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>

#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNurbsCurveData.h>

#include "skeletonStrokeNode.h"
#include "stroke.h"

#include "cImgFloatData.h"
#include "cImgData.h"

#include "brushData.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
#include "errorMacros.h"
#include "mayaMath.h"
#include "skChainData.h"

const double rad_to_deg = (180 / 3.1415927);

MObject skeletonStrokeNode::aSplitAngle;
MObject skeletonStrokeNode::aActive;
MObject skeletonStrokeNode::aChains;
MObject skeletonStrokeNode::aInputData;
MObject skeletonStrokeNode::aSelector;
MObject skeletonStrokeNode::aGoalPoint;
MObject skeletonStrokeNode::aAwayFromGoal;
MObject skeletonStrokeNode::aSmoothNeighbors;
MObject skeletonStrokeNode::aSmoothPositions;
MObject skeletonStrokeNode::aSmoothWeights;

MTypeId skeletonStrokeNode::id(k_skeletonStrokeNode);

skeletonStrokeNode::skeletonStrokeNode() {}

skeletonStrokeNode::~skeletonStrokeNode() {}

void *skeletonStrokeNode::creator()
{
    return new skeletonStrokeNode();
}

MStatus skeletonStrokeNode::initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnUnitAttribute uAttr;

    inheritAttributesFrom("paintStrokeCreator");

    /// skeleton generation
    //////////////////
    MAngle fullCircle(360.0, MAngle::kDegrees);

    aSplitAngle = uAttr.create("splitAngle", "span", MFnUnitAttribute::kAngle);
    uAttr.setHidden(false);
    uAttr.setKeyable(true);
    uAttr.setStorable(true);
    uAttr.setDefault(fullCircle);

    aChains = tAttr.create("chains", "chn", skChainData::id);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
    mser;

    aActive = nAttr.create("active", "act", MFnNumericData::kBoolean);
    nAttr.setKeyable(true);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setDefault(true);

    aInputData = cAttr.create("inputData", "inp");
    cAttr.setArray(true);
    cAttr.addChild(aActive);
    cAttr.addChild(aSplitAngle);
    cAttr.addChild(aChains);

    cAttr.setIndexMatters(true);

    st = addAttribute(aInputData);
    mser;

    aSelector = nAttr.create("selector", "slc", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aSelector);

    aGoalPoint = nAttr.createPoint("goalPoint", "gpt");
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    addAttribute(aGoalPoint);

    aAwayFromGoal = nAttr.create("awayFromGoal", "afg", MFnNumericData::kBoolean);
    nAttr.setKeyable(true);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setDefault(true);
    addAttribute(aAwayFromGoal);

    aSmoothNeighbors = nAttr.create("neighborsToSmooth", "smn", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aSmoothNeighbors);

    aSmoothPositions = nAttr.create("smoothPositions", "smps", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(false);
    st = addAttribute(aSmoothPositions);

    aSmoothWeights = nAttr.create("smoothWeights", "swts", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(false);
    st = addAttribute(aSmoothWeights);

    attributeAffects(aActive, aOutput);
    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aChains, aOutput);

    attributeAffects(aInputData, aOutput);
    attributeAffects(aSelector, aOutput);
    attributeAffects(aGoalPoint, aOutput);
    attributeAffects(aAwayFromGoal, aOutput);

    attributeAffects(aSmoothNeighbors, aOutput);
    attributeAffects(aSmoothPositions, aOutput);
    attributeAffects(aSmoothWeights, aOutput);

    return (MS::kSuccess);
}

MStatus skeletonStrokeNode::generateStrokeGeometry(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{

    MStatus st;

    short int nodeState = data.inputValue(state).asShort();
    if (nodeState != 0)
    {
        return MS::kUnknownParameter;
    }

    MObject thisObj = thisMObject();
    MFnDependencyNode fnNode(thisObj);
    MString nodeName = fnNode.name();

    //////////////////////////////////////////////////////////////
    float pointDensity = data.inputValue(aPointDensity).asFloat();
    pointDensity = fmax(pointDensity, 0.001);
    int selector = data.inputValue(aSelector).asInt();
    int minimumPoints = data.inputValue(aMinimumPoints).asInt();
    float entryTransitionLength = data.inputValue(aEntryTransitionLength).asFloat();
    float exitTransitionLength = data.inputValue(aExitTransitionLength).asFloat();
    float extendEntry = data.inputValue(aExtendEntry).asFloat();
    float extendExit = data.inputValue(aExtendExit).asFloat();
    bool followStroke = data.inputValue(aBrushFollowStroke).asBool();
    int layerId = data.inputValue(aLayerId).asInt();
    int brushId = data.inputValue(aBrushId).asInt();

    int smoothNeighbors = data.inputValue(aSmoothNeighbors).asInt();
    bool smoothPositions = data.inputValue(aSmoothPositions).asBool();
    bool smoothWeights = data.inputValue(aSmoothWeights).asBool();

    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float minimumStrokeAdvance = data.inputValue(aMinimumStrokeAdvance).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();
    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
    MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());

    float3 &fpoint = data.inputValue(aGoalPoint).asFloat3();
    MFloatPoint goalPoint(fpoint[0], fpoint[1], fpoint[2]);

    // flatten the goal point in the canvas plane.
    goalPoint *= canvasMatrix.inverse();
    goalPoint.z = 0.0;
    goalPoint *= canvasMatrix;
    bool awayFromGoal = data.inputValue(aAwayFromGoal).asBool();

    MArrayDataHandle hInputData = data.inputArrayValue(aInputData, &st);
    unsigned nInputs = hInputData.elementCount();

    int start;
    int end;
    if (selector < 0)
    {
        start = 0;
        end = nInputs;
    }
    else if (selector < nInputs)
    {
        start = selector;
        end = selector + 1;
    }
    else
    {
        start = 0;
        end = 0;
    }

    for (unsigned i = start; i < end; i++)
    {
        hInputData.jumpToArrayElement(i);

        int elIndex = hInputData.elementIndex();

        MDataHandle hComp = hInputData.inputValue(&st);
        if (st.error())
        {
            continue;
        }

        bool active = hComp.child(aActive).asBool();
        if (!active)
        {
            continue;
        }

        MDataHandle hChains = hComp.child(aChains);

        MObject dChains = hChains.data();
        MFnPluginData fnChains(dChains, &st);
        if (st.error())
        {

            continue;
        }
        skChainData *scData = (skChainData *)fnChains.data(&st);
        if (st.error())
        {

            continue;
        }
        float splitAngle = hComp.child(aSplitAngle).asAngle().asRadians();

        const std::vector<skChain> *geom = scData->fGeometry;

        std::vector<skChain>::const_iterator current_chain;

        for (current_chain = geom->begin(); current_chain != geom->end(); current_chain++)
        {
            unsigned count = createStrokesForChain(
                *current_chain,
                canvasNormal,
                goalPoint,
                awayFromGoal,
                elIndex,
                minimumPoints,
                pointDensity,
                extendEntry,
                extendExit,
                strokeLength,
                minimumStrokeAdvance,
                overlap,
                splitAngle,
                splitTestInterval,
                pOutStrokes);
        }
    }

    paintStrokeCreator::applyBrushStrokeSpec(data, pOutStrokes);

    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
        curr_stroke->setLayerId(layerId);
        curr_stroke->setBrushId(brushId);
    }

    if ((smoothPositions || smoothWeights) && smoothNeighbors > 0)
    {

        for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
        {
            curr_stroke->smoothTargets(smoothNeighbors, smoothPositions, smoothWeights);
        }
    }

    paintStrokeCreator::generateStrokeGeometry(plug, data, pOutStrokes);

    return MS::kSuccess;
}

unsigned skeletonStrokeNode::createStrokesForChain(
    const skChain &current_chain,

    const MFloatVector &canvasNormal,
    const MFloatPoint &goalPoint,
    bool awayFromGoal,
    unsigned parentId,
    int minimumPoints,
    float pointDensity,
    float extendEntry,
    float extendExit,
    float strokeLength,
    float minimumStrokeAdvance,
    float overlap,
    float splitAngle,
    float splitTestInterval,
    std::vector<Stroke> *pOutStrokes)
{
    MStatus st;
    MPointArray editPoints;
    MFloatArray radii;
    getChainPointsAndRadii(current_chain, extendEntry, extendExit, editPoints, radii);

    // Curve for the whole chain.
    ////////////////////////////
    MFnNurbsCurve curveFn;
    MFnNurbsCurveData dataCreator;
    MObject curveData = dataCreator.create(&st);
    MObject dChainCurve = curveFn.createWithEditPoints(
        editPoints, 3, MFnNurbsCurve::kOpen, true, false, false, curveData, &st);

    if (st.error())
        return 0;
    MDoubleArray knotVals;
    st = curveFn.getKnots(knotVals);
    int numKnots = knotVals.length();
    double recip = 1.0 / knotVals[(numKnots - 1)];
    for (int i = 0; i < numKnots; ++i)
    {
        knotVals[i] = knotVals[i] * recip;
    }
    curveFn.setKnots(knotVals, 0, (numKnots - 1));

    ////////////////////////////

    MFloatVectorArray boundaries;
    unsigned num = getStrokeBoundaries(
        dChainCurve,
        canvasNormal,
        strokeLength,
        minimumStrokeAdvance,
        overlap,
        extendEntry,
        extendExit,
        splitAngle,
        splitTestInterval,
        boundaries);
    ;
    if (!num)
    {
        return 0;
    }

    for (int i = 0; i < num; ++i)
    {
        const float &startDist = boundaries[i].x;
        const float &endDist = boundaries[i].y;
        const float &coil = boundaries[i].z;

        MFloatArray strokeRadii;
        MDoubleArray curveParams;

        // float maxRadius;
        unsigned numPoints = createStrokeData(
            dChainCurve,
            radii,
            startDist,
            endDist,
            pointDensity,
            minimumPoints,
            curveParams,
            strokeRadii);

        bool doReverse = false;
        MPoint startPoint, endPoint;
        double param;
        param = curveFn.findParamFromLength(startDist);
        st = curveFn.getPointAtParam(param, startPoint, MSpace::kObject);
        mser;
        param = curveFn.findParamFromLength(endDist);
        st = curveFn.getPointAtParam(param, endPoint, MSpace::kObject);
        mser;

        if (startPoint.distanceTo(goalPoint) < endPoint.distanceTo(goalPoint))
        {
            doReverse = true;
        }
        if (awayFromGoal)
        {
            doReverse = !doReverse;
        }

        Stroke stroke;
        if (doReverse)
        {
            stroke = createReverseStroke(
                dChainCurve,
                curveParams,
                strokeRadii);
        }
        else
        {
            stroke = createStroke(
                dChainCurve,
                curveParams,
                strokeRadii);
        }

        if (stroke.valid())
        {
            stroke.setParentId(parentId);
            stroke.setCoil(coil);
            pOutStrokes->push_back(stroke);
        }
    }

    return num;
}

unsigned skeletonStrokeNode::createStrokeData(
    const MObject &dCurve,
    const MFloatArray &radii,
    float startDist,
    float endDist,
    float density,
    int minimumPoints,
    MDoubleArray &curveParams,
    MFloatArray &strokeRadii) const
{
    MFnNurbsCurve curveFn(dCurve);
    float strokeRange = endDist - startDist;
    int numPoints = int(density * fabs(strokeRange));
    minimumPoints = std::max(minimumPoints, 2);
    numPoints = std::max(numPoints, minimumPoints);
    float gap = strokeRange / (numPoints - 1);

    // maxRadius = 0.0f;
    for (unsigned i = 0; i < numPoints; i++)
    {
        float curveDist = startDist + (i * gap);
        double uniformParam = curveFn.findParamFromLength(curveDist);
        curveParams.append(uniformParam);
        float radius = Stroke::interpFloat(radii, uniformParam);
        strokeRadii.append(radius);
        // maxRadius = fmax(maxRadius, radius);
    }
    return numPoints;
}

Stroke skeletonStrokeNode::createStroke(
    const MObject &dCurve,
    const MDoubleArray &curveParams,
    const MFloatArray &strokeRadii) const
{
    MStatus st;
    const double epsilon = 0.0001;
    unsigned len = curveParams.length();
    if (len != strokeRadii.length())
    {
        return Stroke();
    }

    MFnNurbsCurve curveFn(dCurve);
    double curveLength = curveFn.length(epsilon);

    MFloatPointArray points;

    std::vector<MFloatMatrix> targetTransforms;

    double entryDistance = curveFn.findLengthFromParam(curveParams[0]);
    double exitDistance = curveFn.findLengthFromParam(curveParams[(len - 1)]);
    float maxRadius = 0.0f;
    for (unsigned i = 0; i < len; i++)
    {
        const double &curveParam = curveParams[i];
        const float &radius = strokeRadii[i];

        MPoint point;
        st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);

        MFloatMatrix mat;

        mat[3][0] = point.x;
        mat[3][1] = point.y;
        mat[3][2] = point.z;

        ///////////////////////////////////

        targetTransforms.push_back(mat);
        maxRadius = fmax(maxRadius, radius);
    }

    Stroke stroke(targetTransforms);
    stroke.setMaxRadius(maxRadius);

    Stroke::target_iterator it = stroke.targets_begin();
    for (unsigned i = 0; it != stroke.targets_end(); i++, it++)
    {
        it->setRadius(strokeRadii[i]);
    }
    // stroke.setBrushId(brushId);
    return stroke;
}

Stroke skeletonStrokeNode::createReverseStroke(
    const MObject &dCurve,
    const MDoubleArray &curveParams,
    const MFloatArray &strokeRadii) const
{
    MStatus st;
    const double epsilon = 0.0001;
    unsigned len = curveParams.length();
    if (len != strokeRadii.length())
    {
        return Stroke();
    }

    MFnNurbsCurve curveFn(dCurve);
    double curveLength = curveFn.length(epsilon);

    MFloatPointArray points;

    std::vector<MFloatMatrix> brushTransforms;
    float maxRadius = 0.0f;
    for (unsigned j = 0; j < len; j++)
    {
        int i = len - (j + 1);
        const double &curveParam = curveParams[i];
        const float &radius = strokeRadii[i];
        MPoint point;
        st = curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
        mser;
        MFloatMatrix mat;
        mat[3][0] = point.x;
        mat[3][1] = point.y;
        mat[3][2] = point.z;
        brushTransforms.push_back(mat);
        maxRadius = fmax(maxRadius, radius);
    }
    Stroke stroke(brushTransforms);
    stroke.setMaxRadius(maxRadius);
    Stroke::target_iterator it = stroke.targets_begin();
    for (unsigned i = 0; it != stroke.targets_end(); i++, it++)
    {
        it->setRadius(strokeRadii[i]);
    }

    return stroke;
}

void skeletonStrokeNode::getChainPointsAndRadii(
    const skChain &chain,
    float extendEntry,
    float extendExit,
    MPointArray &points,
    MFloatArray &radii) const
{
    const float fepsilon = 0.0001;

    radii.clear();
    points.clear();

    const int hasExtendedEntry = (extendEntry > fepsilon) ? 1 : 0;
    const int hasExtendedExit = (extendExit > fepsilon) ? 1 : 0;

    std::vector<skPoint>::const_iterator p0, p1;
    skPoint extendedEntryPoint, extendedExitPoint;
    if (hasExtendedEntry)
    {
        p1 = chain.points().begin(); // 0
        p0 = std::next(p1);          // 1
        extendedEntryPoint = skPoint::extrapolate(*p0, *p1, extendEntry);
        points.append(MPoint(extendedEntryPoint.x, extendedEntryPoint.y, 0.0));
    }

    chain.appendPointsTo(points);

    if (hasExtendedExit)
    {
        p1 = std::prev(chain.points().end()); // -1
        p0 = std::prev(p1);                   // -2
        extendedExitPoint = skPoint::extrapolate(*p0, *p1, extendExit);
        points.append(MPoint(extendedExitPoint.x, extendedExitPoint.y, 0.0));
    }

    if (hasExtendedEntry)
    {
        radii.append(extendedEntryPoint.radius);
    }

    for (int i = 0; i < chain.size(); ++i)
    {
        radii.append(chain[i].radius);
    }

    if (hasExtendedExit)
    {
        radii.append(extendedExitPoint.radius);
    }
}

void skeletonStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
