
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
#include "stroke.h"

#include "brushData.h"

#include <jMayaIds.h>
#include "errorMacros.h"

MObject curveStrokeNode::aCurves;
MObject curveStrokeNode::aBrushId;
MObject curveStrokeNode::aBrush;
MObject curveStrokeNode::aSplitAngle;
MObject curveStrokeNode::aPivot;

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

    aCurves = tAttr.create("curves", "crvs", MFnNurbsCurveData::kNurbsCurve);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    tAttr.setArray(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
    st = addAttribute(aCurves);
    mser;

    aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kInt);
    mser;
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    st = addAttribute(aBrushId);
    mser;

    aBrush = tAttr.create("brush", "bsh", brushData::id);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setConnectable(true);
    tAttr.setDisconnectBehavior(MFnAttribute::kReset);
    addAttribute(aBrush);

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
    attributeAffects(aBrushId, aOutput);
    attributeAffects(aBrush, aOutput);
    attributeAffects(aPivot, aOutput);

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
    pointDensity = fmax(pointDensity, 0.001);
    int minimumPoints = data.inputValue(aMinimumPoints).asInt();
    float entryTransitionLength = data.inputValue(aEntryTransitionLength).asFloat();
    float exitTransitionLength = data.inputValue(aExitTransitionLength).asFloat();
    float extendEntry = data.inputValue(aExtendEntry).asFloat();
    float extendExit = data.inputValue(aExtendExit).asFloat();
    bool followStroke = data.inputValue(aBrushFollowStroke).asBool();
    bool applyBrushBias = data.inputValue(aApplyBrushBias).asBool();

    int layerId = data.inputValue(aLayerId).asInt();
    int paintId = data.inputValue(aPaintId).asInt();
    int potId = data.inputValue(aPotId).asInt();

    float splitAngle = float(data.inputValue(aSplitAngle).asAngle().asRadians());
    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float minimumStrokeAdvance = data.inputValue(aMinimumStrokeAdvance).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();

    curveStrokeNode::Pivot pivot =
        (curveStrokeNode::Pivot)data.inputValue(aPivot).asShort();

    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();

    //////////////////////////////////////////////////////////////

    int brushId = data.inputValue(aBrushId).asInt();

    MDataHandle hBrush = data.inputValue(aBrush, &st);
    msert;
    MObject dBrush = hBrush.data();

    MFnPluginData fnBrush(dBrush, &st);
    msert;

    brushData *bData = (brushData *)fnBrush.data();
    std::pair<int, Brush> brushPair = std::make_pair(brushId, *(bData->fGeometry));

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

        MDoubleArray knotVals;
        st = curveFn.getKnots(knotVals);
        int numKnots = knotVals.length();
        double recip = 1.0 / knotVals[(numKnots - 1)];
        for (int i = 0; i < numKnots; ++i)
        {
            knotVals[i] = knotVals[i] * recip;
        }
        curveFn.setKnots(knotVals, 0, (numKnots - 1));

        MFloatVectorArray boundaries;
        unsigned num = getStrokeBoundaries(
            dCurve,
            canvasNormal,
            strokeLength,
            minimumStrokeAdvance,
            overlap,
            extendEntry,
            extendExit,
            splitAngle,
            splitTestInterval,
            boundaries);

        // cerr << "num boundaries: " << num << endl;
        if (!num)
        {
            return MS::kUnknownParameter;
        }

        MPoint curveStart;

        st = curveFn.getPointAtParam (0.0, curveStart);
        mser;
        MFloatPoint fCurveStart(curveStart);
        // cerr << "fCurveStart" << fCurveStart << endl;



        for (int i = 0; i < num; ++i)
        {
            const float &startDist = boundaries[i].x;
            const float &endDist = boundaries[i].y;
            const float &coil = boundaries[i].z;

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
                brushPair,
                // canvasMatrix,
                curveParams,
                followStroke,
                applyBrushBias,
                entryTransitionLength,
                exitTransitionLength);

            if (stroke.valid())
            {
                stroke.setParentId(0);
                stroke.setCoil(coil);
                if (pivot == curveStrokeNode::kCurveStart)
                {
                    // cerr << "set pivot to curveStart" << endl;
                    stroke.setPivotPosition(fCurveStart);
                }
                pOutStrokes->push_back(stroke);
            }
        }
    }

    strokeCreator::applyRotations(data, pOutStrokes);

    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
        curr_stroke->setPaintId(paintId);
        curr_stroke->setPotId(potId);
        curr_stroke->setLayerId(layerId);
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
    const std::pair<int, Brush> &brushPair,
    // const MFloatMatrix &canvasMatrix,
    const MDoubleArray &curveParams,
    bool followStroke,
    bool applyBrushBias,
    float entryTransitionLength,
    float exitTransitionLength) const
{
    MStatus st;
    const double epsilon = 0.0001;
    unsigned len = curveParams.length();

    const int &brushId = brushPair.first;
    const Brush &brush = brushPair.second;

    MFnNurbsCurve curveFn(dCurve);
    double curveLength = curveFn.length(epsilon);

    // MFloatMatrix brushMatrix(mayaMath::rotationOnly(brush.matrix() * canvasMatrix));
    const MFloatMatrix &brushMatrix = brush.matrix();

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
        double distanceOnCurve = curveFn.findLengthFromParam(curveParam, &st);

        float weight = calculateTargetWeight(
            distanceOnCurve, entryDistance, exitDistance,
            entryTransitionDistance, exitTransitionDistance,
            entryTransitionLength, exitTransitionLength);

        /////////////////////////////////// calcuate biased point
        float forwardBias = (forwardBias1 * weight) + (forwardBias0 * (1.0f - weight));
        mser;
        double biasedDist = distanceOnCurve + forwardBias;
        double biasedCurveParam = curveFn.findParamFromLength(biasedDist, &st);

        MPoint point;
        curveFn.getPointAtParam(biasedCurveParam, point, MSpace::kObject);
        // tangent will possibly be used for followStroke
        MVector tangent = curveFn.tangent(biasedCurveParam);

        float extraDist = biasedDist - curveLength;
        if (extraDist > 0.0)
        {
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

void curveStrokeNode::postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
