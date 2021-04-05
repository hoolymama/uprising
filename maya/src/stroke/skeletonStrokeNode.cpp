
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

MObject skeletonStrokeNode::aCanvasMatrix;
MObject skeletonStrokeNode::aChains;
MObject skeletonStrokeNode::aStrokeLength;
MObject skeletonStrokeNode::aOverlap;
MObject skeletonStrokeNode::aBrushes;
MObject skeletonStrokeNode::aPaintId;
// MObject skeletonStrokeNode::aLayerId;
MObject skeletonStrokeNode::aBrushFollowStroke;
MObject skeletonStrokeNode::aSplitAngle;
MObject skeletonStrokeNode::aSplitTestInterval;
MObject skeletonStrokeNode::aEntryTransitionLength;
MObject skeletonStrokeNode::aExitTransitionLength;
MObject skeletonStrokeNode::aExtendEntry;
MObject skeletonStrokeNode::aExtendExit;
MObject skeletonStrokeNode::aMinimumPoints;
MObject skeletonStrokeNode::aStrokeDirectionMap;
// MObject skeletonStrokeNode::aOutCoil;

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

    inheritAttributesFrom("strokeCreator");

    /// skeleton generation
    //////////////////
    aChains = tAttr.create("chains", "chn", skChainData::id);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    tAttr.setArray(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aChains);
    mser;

    MFloatMatrix identity;
    identity.setToIdentity();
    aCanvasMatrix = mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(false);
    mAttr.setHidden(false);
    mAttr.setKeyable(true);
    mAttr.setDefault(identity);
    addAttribute(aCanvasMatrix);

    aStrokeLength = nAttr.create("strokeLength", "stl", MFnNumericData::kFloat);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.01f);
    nAttr.setDefault(10.0f);
    st = addAttribute(aStrokeLength);
    mser;

    aOverlap = nAttr.create("overlap", "ovlp", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aOverlap);
    mser;

    aBrushes = tAttr.create("brushes", "bsh", brushData::id);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setConnectable(true);
    tAttr.setArray(true);
    tAttr.setIndexMatters(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    addAttribute(aBrushes);

    aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kInt);
    mser;
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    st = addAttribute(aPaintId);
    mser;

    // aLayerId = nAttr.create("layerId", "lid", MFnNumericData::kInt);
    // mser;
    // nAttr.setHidden(false);
    // nAttr.setKeyable(true);
    // nAttr.setStorable(true);
    // nAttr.setWritable(true);
    // st = addAttribute(aLayerId);
    // mser;

    aMinimumPoints = nAttr.create("aMinimumPoints", "mnpts", MFnNumericData::kInt);
    mser;
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    nAttr.setDefault(2);
    st = addAttribute(aMinimumPoints);
    mser;

    aBrushFollowStroke = nAttr.create("followStroke", "fst", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(true);
    st = addAttribute(aBrushFollowStroke);
    mser;

    aSplitAngle = uAttr.create("splitAngle", "span",
                               MFnUnitAttribute::kAngle);
    uAttr.setHidden(false);
    uAttr.setKeyable(true);
    uAttr.setStorable(true);
    st = addAttribute(aSplitAngle);
    mser;

    aSplitTestInterval = nAttr.create("splitTestInterval", "spti", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aSplitTestInterval);
    mser;

    // Transitions
    //////////////
    aEntryTransitionLength = nAttr.create("entryTransitionLength", "enl", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(1.0);
    st = addAttribute(aEntryTransitionLength);
    mser;

    aExitTransitionLength = nAttr.create("exitTransitionLength", "exl", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(1.0);
    st = addAttribute(aExitTransitionLength);
    mser;

    aExtendEntry = nAttr.create("extendEntry", "een", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setDefault(0.0f);
    st = addAttribute(aExtendEntry);
    mser;

    aExtendExit = nAttr.create("extendExit", "eex", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setDefault(0.0f);
    st = addAttribute(aExtendExit);
    mser;
    //////////////

    aStrokeDirectionMap = nAttr.createColor("std", "strokeDirectionMap");
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    addAttribute(aStrokeDirectionMap);

    attributeAffects(aChains, aOutput);
    attributeAffects(aStrokeLength, aOutput);
    attributeAffects(aOverlap, aOutput);
    attributeAffects(aBrushes, aOutput);
    attributeAffects(aPaintId, aOutput);
    // attributeAffects(aLayerId, aOutput);
    attributeAffects(aBrushFollowStroke, aOutput);
    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aSplitTestInterval, aOutput);
    attributeAffects(aEntryTransitionLength, aOutput);
    attributeAffects(aExitTransitionLength, aOutput);
    attributeAffects(aExtendEntry, aOutput);
    attributeAffects(aExtendExit, aOutput);
    attributeAffects(aStrokeDirectionMap, aOutput);
    attributeAffects(aCanvasMatrix, aOutput);
    attributeAffects(aMinimumPoints, aOutput);

    return (MS::kSuccess);
}

MStatus skeletonStrokeNode::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{

    MStatus st;

    MObject thisObj = thisMObject();
    MFnDependencyNode fnNode(thisObj);
    MString nodeName = fnNode.name();

    //////////////////////////////////////////////////////////////
    float pointDensity = data.inputValue(aPointDensity).asFloat();
    pointDensity = fmax(pointDensity, 0.001);

    int minimumPoints = data.inputValue(aMinimumPoints).asInt();

    float entryTransitionLength = data.inputValue(aEntryTransitionLength).asFloat();
    float exitTransitionLength = data.inputValue(aExitTransitionLength).asFloat();
    float extendEntry = data.inputValue(aExtendEntry).asFloat();
    float extendExit = data.inputValue(aExtendExit).asFloat();

    bool followStroke = data.inputValue(aBrushFollowStroke).asBool();

    int paintId = data.inputValue(aPaintId).asInt();
    // int layerId = data.inputValue(aLayerId).asInt();

    double splitAngle = data.inputValue(aSplitAngle).asAngle().asRadians();
    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();

    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();

    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();

    std::vector<std::pair<int, Brush> > brushes;
    st = collectBrushes(data, brushes);
    if (!brushes.size())
    {
        MGlobal::displayWarning("No brushes: " + nodeName);
        return MS::kUnknownParameter;
    }

    MArrayDataHandle hChains = data.inputValue(aChains, &st);
    msert;
    unsigned nInputs = hChains.elementCount();

    m_maxCoil = 0.0;


    cerr << "MAIN: " <<  "entryTransitionLength: " << entryTransitionLength << endl;
    cerr << "MAIN: " <<  "exitTransitionLength: " << exitTransitionLength << endl;


    for (unsigned i = 0; i < nInputs; i++, hChains.next())
    {
        int parentId = hChains.elementIndex(&st);
        MDataHandle hChainsInput = hChains.inputValue(&st);
        if (st.error())
        {
            continue;
        }
        MObject dChainsInput = hChainsInput.data();
        MFnPluginData fnChains(dChainsInput, &st);
        if (st.error())
        {
            continue;
        }
        skChainData *scData = (skChainData *)fnChains.data(&st);
        if (st.error())
        {
            continue;
        }
        const std::vector<skChain> *geom = scData->fGeometry;
        std::vector<skChain>::const_iterator current_chain;

        for (current_chain = geom->begin(); current_chain != geom->end(); current_chain++)
        {
            unsigned count = createStrokesForChain(
                *current_chain,
                brushes,
                canvasMatrix,
                parentId,
                minimumPoints,
                followStroke,
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
    //   curr_stroke->setLayerId(layerId);
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
    getPointsAndRadii(current_chain, extendEntry, extendExit, editPoints, radii);

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
            entryTransitionLength,
            exitTransitionLength);

        if (stroke.valid())
        {
            stroke.setParentId(parentId);
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

    // float brushRadiusRecip = 2.0 / brushWidth;

    float forwardBias0 = fmax(0.0, brush.forwardBias0());
    float forwardBias1 = fmax(0.0, brush.forwardBias1());

    MFloatPointArray points;
    MFloatArray weights;
    std::vector<MFloatMatrix> brushTransforms;

    double entryDistance = curveFn.findLengthFromParam(curveParams[0]);
    double exitDistance = curveFn.findLengthFromParam(curveParams[(len - 1)]);
    double totalLength = exitDistance - entryDistance;

    entryTransitionLength = fmax(entryTransitionLength, 0.0);
    exitTransitionLength = fmax(exitTransitionLength, 0.0);
    float totalTransitionLength = exitTransitionLength + entryTransitionLength;
    if (totalTransitionLength > totalLength) {
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
        curveFn.getPointAtParam(biasedCurveParam, point, MSpace::kObject);
        // tangent will possibly be used for followStroke
        MVector tangent = curveFn.tangent(biasedCurveParam);
        cerr << "biasedCurveParam: "<< biasedCurveParam<<" tangent: " << tangent << endl;

        if (biasedCurveParam >=1.0) // If we are past the end of the curve, push along the tangent
        {
            float extraDist = biasedDist - curveLength;
            cerr << "past the end, extraDist: " << extraDist << endl;
            point += tangent.normal() * extraDist;
        }
        ///////////////////////////////////

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

unsigned int skeletonStrokeNode::getStrokeBoundaries(
    const MObject &dCurve,
    const MFloatVector &canvasNormal,
    float strokeLength,
    float overlap,
    float extendEntry,
    float extendExit,
    double splitAngle,
    float splitTestInterval,
    MFloatVectorArray &result)
{

    const double epsilon = 0.0001;

    MStatus st = MS::kSuccess;

    MFnNurbsCurve curveFn(dCurve, &st);
    mser;
    if (st.error())
    {
        return 0;
    }

    double curveLength = curveFn.length(epsilon);
    float lastEndDist = 0;
    strokeLength = std::max(strokeLength, 0.1f);
    if (overlap >= strokeLength)
    {
        overlap = 0.0f;
    }

    do
    {

        MFloatVector boundary;
        bool done = getBoundary(
            dCurve,
            curveLength,
            canvasNormal,
            lastEndDist,
            strokeLength,
            overlap,
            extendEntry,
            extendExit,
            splitAngle,
            splitTestInterval,
            boundary);
        if (done)
        {
            break;
        }
        result.append(boundary);
        lastEndDist = boundary[1];

    } while (true);

    return result.length();
}

bool skeletonStrokeNode::getBoundary(
    const MObject &dCurve,
    double curveLength,
    const MFloatVector &canvasNormal,
    float lastEndDist,
    float strokeLength,
    float overlap,
    float extendEntry,
    float extendExit,
    double splitAngle,
    float splitTestInterval,
    MFloatVector &result)
{
    bool done = true;
    const float epsilon = 0.0001f;
    if (lastEndDist + epsilon >= curveLength)
    {
        return done;
    }

    float startDist = lastEndDist - extendExit - extendEntry - overlap;
    startDist = fmax(startDist, 0.0f);
    if (startDist > curveLength)
    {
        return done;
    }

    float endDist = startDist + extendEntry + strokeLength;

    bool doSplitTest = (splitAngle > epsilon && splitTestInterval > 0.01);
    if (doSplitTest)
    {
        endDist = findEndDist(dCurve, canvasNormal, startDist, endDist, splitAngle, splitTestInterval);
    }
    endDist += extendExit;
    if (endDist >= curveLength)
    {
        endDist = curveLength;
    }
    result = MFloatVector(startDist, endDist);
    return false;
}

float skeletonStrokeNode::findEndDist(
    const MObject &dCurve,
    const MFloatVector &canvasNormal,
    float startDist,
    float endDist,
    double splitAngle,
    float splitTestInterval)
{

    float leftExtent = 0;
    float rightExtent = 0;
    float accumAngle = 0;
    float currDist = startDist;
    MFnNurbsCurve curveFn(dCurve);
    double param = curveFn.findParamFromLength(startDist);
    MVector lastTangent = curveFn.tangent(param);

    double angle;
    MVector axis;

    bool foundEnd = false;
    do
    {
        currDist += splitTestInterval;
        if (currDist > endDist)
        {
            return endDist;
        }
        param = curveFn.findParamFromLength(currDist);
        MVector tangent = curveFn.tangent(param);

        MQuaternion q(lastTangent, tangent);
        lastTangent = tangent;

        bool rotated = q.getAxisAngle(axis, angle);

        if (!rotated)
        {
            continue;
        }

        float direction = (MFloatVector(axis) * canvasNormal < 0) ? -1.0f : 1.0f;
        accumAngle += (direction * angle);
        if (accumAngle < leftExtent)
        {
            leftExtent = accumAngle;
        }
        if (accumAngle > rightExtent)
        {
            rightExtent = accumAngle;
        }
        float coil = rightExtent - leftExtent;
        foundEnd = (coil > splitAngle);

        if (coil > this->m_maxCoil)
        {
            this->m_maxCoil = coil;
        }
    } while (!foundEnd);

    return currDist;
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

void skeletonStrokeNode::getPointsAndRadii(
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

// void skeletonStrokeNode::getPointsAndWeights(
//     const skChain &chain,
//     const std::pair<int, Brush> &brush,
//     // float entryTransitionLength,
//     // float exitTransitionLength,
//     float extendEntry,
//     float extendExit,
//     MFloatPointArray &points,
//     MFloatArray &weights) const
// {
//     const float epsilon = 0.0001;

//     weights.clear();
//     points.clear();

//     const int hasExtendedEntry = (extendEntry > epsilon) ? 1 : 0;
//     const int hasExtendedExit = (extendExit > epsilon) ? 1 : 0;

//     std::vector<skPoint>::const_iterator p0, p1;
//     skPoint extendedEntryPoint, extendedExitPoint;
//     if (hasExtendedEntry)
//     {
//         p1 = chain.points().begin(); // 0
//         p0 = std::next(p1);          // 1
//         extendedEntryPoint = skPoint::extrapolate(*p0, *p1, extendEntry);
//         points.append(extendedEntryPoint.x, extendedEntryPoint.y, 0.0);
//     }

//     chain.appendPointsTo(points);

//     if (hasExtendedExit)
//     {
//         p1 = std::prev(chain.points().end()); // -1
//         p0 = std::prev(p1);                   // -2
//         extendedExitPoint = skPoint::extrapolate(*p0, *p1, extendExit);
//         points.append(extendedExitPoint.x, extendedExitPoint.y, 0.0);
//     }

//     // if (brush.second.isFlat())
//     // {
//     //     weights = MDoubleArray((chain.size() + hasExtendedEntry + hasExtendedExit), 1.0);
//     //     return;
//     // }

//     float brushWidth = brush.second.width();
//     if (brushWidth < epsilon)
//     {
//         brushWidth = 1.0;
//     }
//     float brushRadiusRecip = 2.0 / brushWidth;

//     if (hasExtendedEntry)
//     {
//         weights.append(fmin((extendedEntryPoint.radius * brushRadiusRecip), 1.0));
//     }

//     for (int i = 0; i < chain.size(); ++i)
//     {
//         weights.append(
//             fmin((chain[i].radius * brushRadiusRecip), 1.0));
//     }

//     if (hasExtendedExit)
//     {
//         weights.append(fmin((extendedExitPoint.radius * brushRadiusRecip), 1.0));
//     }
// }

// MPointArray editPoints;
// MDoubleArray contacts;
// getPointsAndWeights(
//     *current_chain,
//     selectedBrush,
//     entryTransitionLength,
//     exitTransitionLength,
//     extendEntry,
//     extendExit,
//     contacts,
//     editPoints);

// int selectedBrushId = selectedBrush.first;

// MFnNurbsCurve curveFn;
// MFnNurbsCurveData dataCreator;
// MObject curveData = dataCreator.create(&st);
// mser;

// MObject dCurve = curveFn.createWithEditPoints(
//     editPoints, 3,
//     MFnNurbsCurve::kOpen,
//     true, false, false, curveData, &st);

// MDoubleArray knotVals;
// st = curveFn.getKnots(knotVals);
// int numKnots = knotVals.length();

// double maxValRecip = 1.0 / knotVals[(numKnots - 1)];

// for (int i = 0; i < numKnots; ++i)
// {
//     knotVals[i] = knotVals[i] * maxValRecip;
// }
// curveFn.setKnots(knotVals, 0, (numKnots - 1));
// double curveLength = curveFn.length(epsilon);
// MVectorArray boundaries;
// if (!getStrokeBoundaries(
//         dCurve,
//         strokeLength,
//         overlap,
//         splitAngle,
//         splitTestInterval,
//         boundaries,
//         maxCoil))
// {
//     continue;
// }
// for (int strokeId = 0; strokeId < boundaries.length(); ++strokeId)
// {
//     const double &startDist = boundaries[strokeId].x;
//     const double &endDist = boundaries[strokeId].y;

//     unsigned strokeGroupSize = Stroke::create(
//         thisObj,
//         dCurve,
//         contacts,
//         localContact,
//         curveLength,
//         startDist,
//         endDist,
//         entryTransitionLength,
//         exitTransitionLength,
//         // transitionBlendMethod,
//         pointDensity,
//         minimumPoints,
//         rotSpec,
//         repeatSpec,
//         strokeDirection,
//         pivotParam,
//         paintFlow,
//         strokeId,
//         selectedBrushId,
//         paintId,
//         layerId,
//         // customBrushId,
//         pStrokes);
// }

// MStatus skeletonStrokeNode::createStroke(
//     const MObject &dCurve,
//     const MFloatMatrix &targetRotationMatrix,
//     const MFloatArray &contacts,
//     float startDist,
//     float endDist,
//     float entryTransitionLength,
//     float exitTransitionLength,
//     float density,
//     unsigned minimumPoints,
//     unsigned strokeId,
//     unsigned brushId,
//     unsigned paintId,
//     unsigned layerId,
//     std::vector<Stroke> *pOutStrokes) const
// {

//     MFnNurbsCurve curveFn(curveObject);
//     float strokeRange = endDist - startDist; // can be negative
//     unsigned numPoints = unsigned(density * fabs(strokeRange));

//     minimumPoints = std::max(minimumPoints, 2);
//     numPoints = std::max(numPoints, minimumPoints);

//     float gap = strokeRange / (numPoints - 1);

//     for (unsigned i = 0; i < numPoints; i++)
//     {

//         float dist = startDist + (i * gap);
//         float strokeParam = (dist - startDist) / strokeRange;
//         float curveParam = dist / curveLength;
//         double uniformParam = curveFn.findParamFromLength(dist);

//         MFloatVector tangent = curveFn.tangent(uniformParam);

//         // tangent.z = 0;
//         // tangent.normalize();
//         float contact = Stroke::interpWeight(contacts, uniformParam);

//         MPoint pt;
//         curveFn.getPointAtParam(uniformParam, pt, MSpace::kObject);

//         m_targets.push_back(
//             Target(pt, tangent, strokeParam, curveParam, contact));
//     }

//     Stroke(pts, targetRotationMatrix, strokeIndex

//     double pivotDist = startDist + (strokeRange * pivotParam);
//     m_pivot = Target(curveFn, pivotDist, startDist, strokeRange, curveLength);

//     setArcLength();
// }
