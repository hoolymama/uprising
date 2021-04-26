
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
#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include "cImgFloatData.h"
#include "cImgData.h"

#include "brushData.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
#include "errorMacros.h"
#include "mayaMath.h"
#include "skChainData.h"

const double rad_to_deg = (180 / 3.1415927);

MObject skeletonStrokeNode::aBrushes;
MObject skeletonStrokeNode::aSplitAngle;
MObject skeletonStrokeNode::aChains;
MObject skeletonStrokeNode::aInputData;
MObject skeletonStrokeNode::aSelector;

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
    mser;

    aInputData = cAttr.create("inputData", "inp");
    cAttr.setArray(true);
    cAttr.addChild(aSplitAngle);
    cAttr.addChild(aChains);
    cAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aInputData);
    mser;

    aBrushes = tAttr.create("brushes", "bsh", brushData::id);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setConnectable(true);
    tAttr.setArray(true);
    tAttr.setIndexMatters(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aBrushes);

    aSelector = nAttr.create("selector", "slc", MFnNumericData::kInt);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(-1);
    st = addAttribute(aSelector);

    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aChains, aOutput);
    attributeAffects(aBrushes, aOutput);
    attributeAffects(aInputData, aOutput);
    attributeAffects(aSelector, aOutput);

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
    bool applyBrushBias = data.inputValue(aApplyBrushBias).asBool();
    int layerId = data.inputValue(aLayerId).asInt();
    int paintId = data.inputValue(aPaintId).asInt();

    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();
    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
;

    std::vector<std::pair<int, Brush> > brushes;
    st = collectBrushes(data, brushes);
    if (!brushes.size())
    {
        MGlobal::displayWarning("No brushes: " + nodeName);
        return MS::kUnknownParameter;
    }

    MArrayDataHandle hInputData = data.inputArrayValue(aInputData, &st);
    unsigned nInputs = hInputData.elementCount();

    for (unsigned i = 0; i < nInputs; i++, hInputData.next())
    {
        int elIndex = hInputData.elementIndex();
        if (selector > -1 && selector != elIndex)
        {
            continue;
        }

        MDataHandle hComp = hInputData.inputValue(&st);
        MDataHandle hChains = hComp.child(aChains);
        if (st.error())
        {
            continue;
        }

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
                brushes,
                canvasMatrix,
                elIndex,
                minimumPoints,
                followStroke,
                applyBrushBias,
                pointDensity,
                entryTransitionLength,
                exitTransitionLength,
                extendEntry,
                extendExit,
                strokeLength,
                overlap,
                splitAngle,
                splitTestInterval,
                pOutStrokes);
        }
    }

    strokeCreator::applyRotations(data, pOutStrokes);

    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
        curr_stroke->setPaintId(paintId);
        curr_stroke->setLayerId(layerId);
    }

    return MS::kSuccess;
}

unsigned skeletonStrokeNode::createStrokesForChain(
    const skChain &current_chain,
    const std::vector<std::pair<int, Brush> > &brushes,
    const MFloatMatrix &canvasMatrix,
    unsigned parentId,
    int minimumPoints,
    bool followStroke,
    bool applyBrushBias,
    float pointDensity,
    float entryTransitionLength,
    float exitTransitionLength,
    float extendEntry,
    float extendExit,
    float strokeLength,
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

    MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());

    MFloatVectorArray boundaries;
    unsigned num = getStrokeBoundaries(
        dChainCurve,
        canvasNormal,
        strokeLength,
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

        float maxRadius;
        unsigned numPoints = createStrokeData(
            dChainCurve,
            radii,
            startDist,
            endDist,
            pointDensity,
            minimumPoints,
            curveParams,
            strokeRadii,
            maxRadius);

        const std::pair<int, Brush> selectedBrushPair = selectBrush(maxRadius, brushes);

        Stroke stroke = createStroke(
            dChainCurve,
            selectedBrushPair,
            canvasMatrix,
            curveParams,
            strokeRadii,
            followStroke,
            applyBrushBias,
            entryTransitionLength,
            exitTransitionLength);

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
    MFloatArray &strokeRadii,
    float &maxRadius) const
{
    MFnNurbsCurve curveFn(dCurve);
    float strokeRange = endDist - startDist;
    int numPoints = int(density * fabs(strokeRange));
    minimumPoints = std::max(minimumPoints, 2);
    numPoints = std::max(numPoints, minimumPoints);
    float gap = strokeRange / (numPoints - 1);

    maxRadius = 0.0f;
    for (unsigned i = 0; i < numPoints; i++)
    {
        float curveDist = startDist + (i * gap);
        double uniformParam = curveFn.findParamFromLength(curveDist);
        curveParams.append(uniformParam);
        float radius = Stroke::interpFloat(radii, uniformParam);
        strokeRadii.append(radius);
        maxRadius = fmax(maxRadius, radius);
    }
    return numPoints;
}

Stroke skeletonStrokeNode::createStroke(
    const MObject &dCurve,
    const std::pair<int, Brush> &brushPair,
    const MFloatMatrix &canvasMatrix,
    const MDoubleArray &curveParams,
    const MFloatArray &strokeRadii,
    bool followStroke,
    bool applyBrushBias,
    float entryTransitionLength,
    float exitTransitionLength) const
{
    MStatus st;
    const double epsilon = 0.0001;
    unsigned len = curveParams.length();
    if (len != strokeRadii.length())
    {
        return Stroke();
    }
    const int &brushId = brushPair.first;
    const Brush &brush = brushPair.second;

    MFnNurbsCurve curveFn(dCurve);
    double curveLength = curveFn.length(epsilon);

    float brushWidth = fmax(brush.width(), 0.01);
    MFloatMatrix brushMatrix(mayaMath::rotationOnly(brush.matrix() * canvasMatrix));

    float forwardBias0 = 0.0;
    float forwardBias1 = 0.0;

    if (applyBrushBias)
    {
        forwardBias0 = fmax(0.0, brush.forwardBias0());
        forwardBias1 = fmax(0.0, brush.forwardBias1());
    }

    MFloatPointArray points;
    MFloatArray weights;
    std::vector<MFloatMatrix> brushTransforms;

    double entryDistance = curveFn.findLengthFromParam(curveParams[0]);
    double exitDistance = curveFn.findLengthFromParam(curveParams[(len - 1)]);
    double totalLength = exitDistance - entryDistance;

    entryTransitionLength = fmax(entryTransitionLength, 0.0);
    exitTransitionLength = fmax(exitTransitionLength, 0.0);
    float totalTransitionLength = exitTransitionLength + entryTransitionLength;
    if (totalTransitionLength > totalLength)
    {
        float mult = totalLength / totalTransitionLength;
        exitTransitionLength *= mult;
        entryTransitionLength *= mult;
    }

    double entryTransitionDistance = entryDistance + entryTransitionLength;
    double exitTransitionDistance = exitDistance - exitTransitionLength;

    for (unsigned i = 0; i < len; i++)
    {
        const double &curveParam = curveParams[i];
        const float &radius = strokeRadii[i];
        double distanceOnCurve = curveFn.findLengthFromParam(curveParam, &st);

        float weight = calculateTargetWeight(
            distanceOnCurve, entryDistance, exitDistance,
            entryTransitionDistance, exitTransitionDistance,
            entryTransitionLength, exitTransitionLength,
            radius, brushWidth);

        /////////////////////////////////// calcuate biased point
        float forwardBias = (forwardBias1 * weight) + (forwardBias0 * (1.0f - weight));
        mser;
        double biasedDist = distanceOnCurve + forwardBias;
        double biasedCurveParam = curveFn.findParamFromLength(biasedDist, &st);

        MPoint point;
        st = curveFn.getPointAtParam(biasedCurveParam, point, MSpace::kObject); mser;
        // tangent will possibly be used for followStroke
        MVector tangent = curveFn.tangent(biasedCurveParam);

        float extraDist = biasedDist - curveLength;
        if (extraDist> 0.0) {
            point += tangent.normal() * extraDist;
        }
        /////////////////////////////////// calcuate matrix
        MFloatMatrix mat(brushMatrix);
        if (followStroke)
        {
            MFloatVector front = MFloatVector::yNegAxis * mat;
            mat *= MFloatMatrix(MQuaternion(front, tangent).asMatrix().matrix);
        }
        mat[3][0] = point.x;
        mat[3][1] = point.y;
        mat[3][2] = point.z;

        ///////////////////////////////////

        brushTransforms.push_back(mat);
        weights.append(weight);
    }
    Stroke stroke(brushTransforms, weights);
    stroke.setBrushId(brushId);
    return stroke;
}

MStatus skeletonStrokeNode::collectBrushes(
    MDataBlock &data,
    std::vector<std::pair<int, Brush> > &brushes) const
{

    /*
        Brushes, sorted by width, starting with largest.
    */

    MStatus st;
    MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
    msert;
    unsigned nPlugs = ha.elementCount();

    for (unsigned i = 0; i < nPlugs; i++, ha.next())
    {
        int index = ha.elementIndex(&st);
        if (st.error())
        {
            continue;
        }
        MDataHandle h = ha.inputValue(&st);
        if (st.error())
        {
            continue;
        }
        MObject d = h.data();
        MFnPluginData fnP(d, &st);
        if (st.error())
        {
            continue;
        }
        brushData *bData = (brushData *)fnP.data();

        const Brush *pBrush = bData->fGeometry;
        brushes.push_back(std::make_pair(index, *pBrush));
        // if (pBrush->matches(filter))
        // // {
        // //     brushes.push_back(std::make_pair(index, *pBrush));
        // // }
    }

    /*
    Sort from largest to smallest. This is to help us select the best available brush.

    */
    // TODO: Get rid of map indices. Just use Physical ID.

    std::sort(brushes.begin(), brushes.end(),
              [](const std::pair<int, Brush> &a, const std::pair<int, Brush> &b) -> bool {
                  return a.second.width() > b.second.width();
              });
    return MS::kSuccess;
}

const std::pair<int, Brush> skeletonStrokeNode::selectBrush(
    float radius,
    const std::vector<std::pair<int, Brush> > &brushes) const
{
    /*
    The brushes are already sorted widest to finest. We test each brush in turn to
    see if it is big enough for the stroke. When we come across the first brush that
    is too small, we select the previous brush. It will in theory be the best suited.
    If the first brush (the biggest brush) is too small for the stroke, we're just
    going to have to use it.

    If there are no brushes (should never happen!!),
    then we return a pair with key -1.
    */
    if (!brushes.size())
    {
        return std::pair<int, Brush>();
    }
    std::pair<int, Brush> result;

    std::vector<std::pair<int, Brush> >::const_iterator brushIter;
    for (brushIter = brushes.begin(); brushIter != brushes.end(); brushIter++)
    {
        float brushRad = brushIter->second.width() * 0.5;
        if (brushRad <= radius)
        {
            if (brushIter == brushes.begin())
            {
                result = *brushIter;
            }
            break;
        }
        result = *brushIter;
    }
    return result;
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

// MStatus skeletonStrokeNode::connectionBroken(
//     const MPlug &plug,
//     const MPlug &otherPlug,
//     bool asSrc)
// {

//     MStatus st;

//     MString method("skeletonStrokeNode::connectionBroken");
//     unsigned int el;

//     if (plug == aChains)
//     {
//         MDataBlock data = forceCache();
//         MArrayDataHandle hInData = data.inputArrayValue(aInputData, &st);
//         msert;
//         MArrayDataBuilder bInData = hInData.builder(&st);
//         msert;
//         el = plug.parent().logicalIndex();
//         st = bInData.removeElement(el);
//         msert;
//     }
//     return MS::kSuccess;
// }
