
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnPluginData.h>
#include <maya/MFnNurbsCurveData.h>

#include "curveStrokeNode.h"

#include "curveBoundaries.h"
#include "stroke.h"

#include "brushData.h"

#include <jMayaIds.h>
#include "errorMacros.h"

MObject curveStrokeNode::aCurves;

MObject curveStrokeNode::aWidth;
MObject curveStrokeNode::aSplitAngle;
MObject curveStrokeNode::aPivot;
MObject curveStrokeNode::aLinearCurve;

MTypeId curveStrokeNode::id(k_curveStrokeNode);

curveStrokeNode::curveStrokeNode() {}

curveStrokeNode::~curveStrokeNode() {}

void *curveStrokeNode::creator()
{
    return new curveStrokeNode();
}

MStatus curveStrokeNode::initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnUnitAttribute uAttr;
    MFnEnumAttribute eAttr;
    inheritAttributesFrom("paintStrokeCreator");

    aLinearCurve = nAttr.create("linearCurve", "lcrv", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(false);
    st = addAttribute(aLinearCurve);

    aCurves = tAttr.create("curves", "crvs", MFnNurbsCurveData::kNurbsCurve);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    tAttr.setArray(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aCurves);
    mser;

    aWidth = nAttr.create("width", "wid", MFnNumericData::kFloat);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setDefault(1.0f);
    st = addAttribute(aWidth);

    aSplitAngle = uAttr.create("splitAngle", "span",
                               MFnUnitAttribute::kAngle);
    uAttr.setHidden(false);
    uAttr.setKeyable(true);
    uAttr.setStorable(true);
    st = addAttribute(aSplitAngle);
    mser;

    aPivot = eAttr.create("pivot", "pvt", curveStrokeNode::kFirstTarget);
    eAttr.addField("firstTarget", curveStrokeNode::kFirstTarget);
    eAttr.addField("curveStart", curveStrokeNode::kCurveStart);
    eAttr.setHidden(false);
    eAttr.setKeyable(true);
    st = addAttribute(aPivot);

    attributeAffects(aSplitAngle, aOutput);
    attributeAffects(aCurves, aOutput);
    attributeAffects(aWidth, aOutput);
    attributeAffects(aPivot, aOutput);
    attributeAffects(aLinearCurve, aOutput);

    return (MS::kSuccess);
}

MStatus curveStrokeNode::generateStrokeGeometry(
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
    bool linearCurve = data.inputValue(aLinearCurve).asBool();
    pointDensity = fmax(pointDensity, 0.001);
    int minimumPoints = data.inputValue(aMinimumPoints).asInt();
    float entryTransitionLength = data.inputValue(aEntryTransitionLength).asFloat();
    float exitTransitionLength = data.inputValue(aExitTransitionLength).asFloat();
    bool followStroke = data.inputValue(aBrushFollowStroke).asBool();
    float radius = data.inputValue(aWidth).asFloat() * 0.5;

    // int layerId = data.inputValue(aLayerId).asInt();
    // int brushId = data.inputValue(aBrushId).asInt();
    // int paintId = data.inputValue(aPaintId).asInt();

    float splitAngle = float(data.inputValue(aSplitAngle).asAngle().asRadians());
    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float minimumStrokeAdvance = data.inputValue(aMinimumStrokeAdvance).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();

    curveStrokeNode::Pivot pivot =
        (curveStrokeNode::Pivot)data.inputValue(aPivot).asShort();

    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();

    //////////////////////////////////////////////////////////////

    MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());
    MArrayDataHandle hCurves = data.inputArrayValue(aCurves, &st);
    unsigned nInputs = hCurves.elementCount();

    for (unsigned i = 0; i < nInputs; i++, hCurves.next())
    {

        int elIndex = hCurves.elementIndex();
        MDataHandle hCurve = hCurves.inputValue(&st);
        mser;
        if (st.error())
        {
            MGlobal::displayWarning("Skipping " + nodeName + " curve plug" + elIndex);
            continue;
        }
        MObject dCurve = hCurve.asNurbsCurveTransformed();
        MFnNurbsCurve curveFn(dCurve, &st);
        mser;

        if (st.error())
        {
            MGlobal::displayWarning("Skipping " + nodeName + " curve plug" + elIndex);
            continue;
        }

        if (linearCurve)
        {
            MPointArray points;
            curveFn.getCVs(points, MSpace::kObject);

            MFloatPointArray sampledPts;
            strokeCreator::subsample(points, pointDensity, sampledPts);

            MFloatMatrix targetRotationMatrix = mayaMath::rotationOnly(canvasMatrix);
            Stroke stroke = Stroke(sampledPts, targetRotationMatrix);
            stroke.setParentId(i);
            if (pivot == curveStrokeNode::kCurveStart)
            {
                MPoint curveStart;
                st = curveFn.getPointAtParam(0.0, curveStart);
                MFloatPoint fCurveStart(curveStart);
                stroke.setPivotPosition(fCurveStart);
            }
            pOutStrokes->push_back(stroke);
        }
        else
        {

            MDoubleArray knotVals;
            st = curveFn.getKnots(knotVals);
            int numKnots = knotVals.length();
            double recip = 1.0 / knotVals[(numKnots - 1)];
            for (int i = 0; i < numKnots; ++i)
            {
                knotVals[i] = knotVals[i] * recip;
            }
            curveFn.setKnots(knotVals, 0, (numKnots - 1));

            std::vector<Boundary> boundaries;
            boundaries.clear();
            CurveBoundaries cb = CurveBoundaries(dCurve);
            cb.boundaries(
                strokeLength,
                minimumStrokeAdvance,
                overlap,
                splitAngle,
                splitTestInterval,
                canvasNormal,
                boundaries);
            int num = boundaries.size();

            if (!num)
            {
                return MS::kUnknownParameter;
            }

            MPoint curveStart;

            st = curveFn.getPointAtParam(0.0, curveStart);
            mser;
            MFloatPoint fCurveStart(curveStart);

            for (int j = 0; j < num; ++j)
            {
                const float &startDist = boundaries[j].start;
                const float &endDist = boundaries[j].end;
                const float &coil = boundaries[j].maxCoil;

                MDoubleArray curveParams;

                unsigned numPoints = createStrokeData(
                    dCurve,
                    startDist,
                    endDist,
                    pointDensity,
                    minimumPoints,
                    curveParams);

                Stroke stroke = createStroke(
                    dCurve,
                    curveParams,
                    radius);

                if (stroke.valid())
                {
                    stroke.setParentId(i);
                    stroke.setCoil(coil);
                    stroke.setSegmentId(j);
                    if (pivot == curveStrokeNode::kCurveStart)
                    {
                        stroke.setPivotPosition(fCurveStart);
                    }
                    pOutStrokes->push_back(stroke);
                }
            }
        }
    }

    paintStrokeCreator::generateStrokeGeometry(plug, data, pOutStrokes);

    return MS::kSuccess;
}

unsigned curveStrokeNode::createStrokeData(
    const MObject &dCurve,
    float startDist,
    float endDist,
    float density,
    int minimumPoints,
    MDoubleArray &curveParams) const
{
    MFnNurbsCurve curveFn(dCurve);
    float strokeRange = endDist - startDist;
    int numPoints = int(density * fabs(strokeRange));
    minimumPoints = std::max(minimumPoints, 2);
    numPoints = std::max(numPoints, minimumPoints);
    float gap = strokeRange / (numPoints - 1);

    for (unsigned i = 0; i < numPoints; i++)
    {
        float curveDist = startDist + (i * gap);
        double uniformParam = curveFn.findParamFromLength(curveDist);
        curveParams.append(uniformParam);
        ;
    }
    return numPoints;
}

Stroke curveStrokeNode::createStroke(
    const MObject &dCurve,
    const MDoubleArray &curveParams,
    float radius

) const
{
    MStatus st;
    const double epsilon = 0.0001;
    unsigned len = curveParams.length();

    MFnNurbsCurve curveFn(dCurve);
    double curveLength = curveFn.length(epsilon);

    MFloatPointArray points;
    std::vector<MFloatMatrix> targetTransforms;

    double entryDistance = curveFn.findLengthFromParam(curveParams[0]);
    double exitDistance = curveFn.findLengthFromParam(curveParams[(len - 1)]);
    double totalLength = exitDistance - entryDistance;

    for (unsigned i = 0; i < len; i++)
    {
        const double &curveParam = curveParams[i];

        MPoint point;
        curveFn.getPointAtParam(curveParam, point, MSpace::kObject);
        MFloatMatrix mat;
        mat[3][0] = point.x;
        mat[3][1] = point.y;
        mat[3][2] = point.z;
        targetTransforms.push_back(mat);
    }
    Stroke stroke(targetTransforms);
    stroke.setMaxRadius(radius);
    Stroke::target_iterator it = stroke.targets_begin();
    for (unsigned i = 0; it != stroke.targets_end(); i++, it++)
    {
        it->setRadius(radius);
    }
    return stroke;
}

void curveStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
