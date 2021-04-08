
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MFnPluginData.h>
#include <maya/MFnNurbsCurveData.h>

#include "curveStrokeNode.h"
#include "stroke.h"
 
 #include "brushData.h"

#include <jMayaIds.h>
#include "errorMacros.h"

 
MObject curveStrokeNode::aCurve;
MObject curveStrokeNode::aBrushId;
MObject curveStrokeNode::aBrush;

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
 
    inheritAttributesFrom("paintStrokeCreator");

    aCurve = tAttr.create("curve", "crv", MFnNurbsCurveData::kNurbsCurve);
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);
    st = addAttribute(aCurve);
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

    st = attributeAffects(aCurve, aOutput);
    st = attributeAffects(aBrushId, aOutput);
    st = attributeAffects(aBrush, aOutput);
     
    return (MS::kSuccess);
}



MStatus curveStrokeNode::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes )
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
    double splitAngle = data.inputValue(aSplitAngle).asAngle().asRadians();
    float splitTestInterval = data.inputValue(aSplitTestInterval).asFloat();
    float strokeLength = data.inputValue(aStrokeLength).asFloat();
    float overlap = data.inputValue(aOverlap).asFloat();
    MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
    m_maxCoil = 0.0;
    //////////////////////////////////////////////////////////////

    int brushId = data.inputValue(aBrushId).asInt();

    MDataHandle hBrush = data.inputValue(aBrush, &st); msert;
    MObject dBrush = hBrush.data();

    MFnPluginData fnBrush(dBrush, &st);msert;
 
    brushData *bData = (brushData *)fnBrush.data();

    std::pair<int, Brush> brushPair = std::make_pair(brushId, *(bData->fGeometry));

    MDataHandle hCurve = data.inputValue(aCurve, &st);
    msert;
    MObject dCurve = data.inputValue(aCurve).asNurbsCurveTransformed();
    MFnNurbsCurve curveFn(dCurve, &st);
    msert;

    MDoubleArray knotVals;
    st = curveFn.getKnots(knotVals);
    int numKnots = knotVals.length();
    double recip = 1.0 / knotVals[(numKnots - 1)];
    for (int i = 0; i < numKnots; ++i)
    {
        knotVals[i] = knotVals[i] * recip;
    }
    curveFn.setKnots(knotVals, 0, (numKnots - 1));

    MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());
    MFloatVectorArray boundaries;
    unsigned num = getStrokeBoundaries(
        dCurve,
        canvasNormal,
        strokeLength,
        overlap,
        extendEntry,
        extendExit,
        splitAngle,
        splitTestInterval,
        boundaries);
    if (!num)
    {
        return MS::kUnknownParameter;
    }
    for (int i = 0; i < num; ++i)
    {
        const float &startDist = boundaries[i].x;
        const float &endDist = boundaries[i].y;
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
            canvasMatrix,
            curveParams,
            followStroke,
            entryTransitionLength,
            exitTransitionLength);

        if (stroke.valid())
        {
            stroke.setParentId(0);
            pOutStrokes->push_back(stroke);
        }
    }

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
        curveParams.append(uniformParam);;
    }
    return numPoints;
}

Stroke curveStrokeNode::createStroke(
    const MObject &dCurve,
    const std::pair<int, Brush> &brushPair,
    const MFloatMatrix &canvasMatrix,
    const MDoubleArray &curveParams,
    bool followStroke,
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

    MFloatMatrix brushMatrix(mayaMath::rotationOnly(brush.matrix() * canvasMatrix));

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
   
        if (biasedCurveParam >=1.0) // If we are past the end of the curve, push along the tangent
        {
            float extraDist = biasedDist - curveLength;
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
