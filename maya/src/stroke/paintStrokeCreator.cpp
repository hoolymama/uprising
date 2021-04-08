
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

#include "paintStrokeCreator.h"
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

MObject paintStrokeCreator::aCanvasMatrix;
MObject paintStrokeCreator::aStrokeLength;
MObject paintStrokeCreator::aOverlap;
MObject paintStrokeCreator::aPaintId;
MObject paintStrokeCreator::aBrushFollowStroke;
MObject paintStrokeCreator::aSplitAngle;
MObject paintStrokeCreator::aSplitTestInterval;
MObject paintStrokeCreator::aEntryTransitionLength;
MObject paintStrokeCreator::aExitTransitionLength;
MObject paintStrokeCreator::aExtendEntry;
MObject paintStrokeCreator::aExtendExit;
MObject paintStrokeCreator::aMinimumPoints;

MTypeId paintStrokeCreator::id(k_paintStrokeCreator);

paintStrokeCreator::paintStrokeCreator() {}

paintStrokeCreator::~paintStrokeCreator() {}

void *paintStrokeCreator::creator()
{
    return new paintStrokeCreator();
}

MStatus paintStrokeCreator::initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnUnitAttribute uAttr;

    inheritAttributesFrom("strokeCreator");


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

    aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kInt);
    mser;
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    st = addAttribute(aPaintId);
    mser;

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

    attributeAffects(aStrokeLength, aOutput);
    attributeAffects(aOverlap, aOutput);
    attributeAffects(aPaintId, aOutput);
    attributeAffects(aBrushFollowStroke, aOutput);
    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aSplitTestInterval, aOutput);
    attributeAffects(aEntryTransitionLength, aOutput);
    attributeAffects(aExitTransitionLength, aOutput);
    attributeAffects(aExtendEntry, aOutput);
    attributeAffects(aExtendExit, aOutput);
    attributeAffects(aCanvasMatrix, aOutput);
    attributeAffects(aMinimumPoints, aOutput);

    return (MS::kSuccess);
}

MStatus paintStrokeCreator::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{
    return MS::kUnknownParameter;
}

unsigned int paintStrokeCreator::getStrokeBoundaries(
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

bool paintStrokeCreator::getBoundary(
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

float paintStrokeCreator::findEndDist(
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

void paintStrokeCreator::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
