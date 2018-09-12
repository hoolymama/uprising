
# include <maya/MIOStream.h>
# include <math.h>
# include <algorithm>
# include <map>

# include <maya/MFnPluginData.h>
# include <maya/MDoubleArray.h>
# include <maya/MFloatVectorArray.h>
# include <maya/MFloatArray.h>
# include <maya/MFloatPointArray.h>
# include <maya/MPoint.h>

#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>


#include <maya/MRampAttribute.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixArrayData.h>

#include <maya/MFnVectorArrayData.h>

#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTransformationMatrix.h>


#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>

#include "strokeGeometryData.h"
#include "strokeCurveNode.h"
#include "stroke.h"
#include "backstroke.h"
#include "strokeRotationSpec.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);

// per - curve, pass through
MObject strokeCurve::aForceDip;


MObject strokeCurve::aCurve;
MObject strokeCurve::aSubcurveMin;
MObject strokeCurve::aSubcurveMax;
MObject strokeCurve::aSubcurve;

MObject strokeCurve::aRandomOverlapFactor;
MObject strokeCurve::aOverlap;
MObject strokeCurve::aBrushRampScope;


MTypeId strokeCurve:: id(k_strokeCurve );

strokeCurve:: strokeCurve() {}

strokeCurve:: ~strokeCurve() {}

void *strokeCurve::creator() {
    return new strokeCurve();
}

const double epsilon = 0.0001;


MStatus strokeCurve:: initialize()
{
    MStatus st;
    MString method("strokeCurve::initialize");

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnUnitAttribute uAttr;
    MFnCompoundAttribute cAttr;
    MFnMatrixAttribute mAttr;
    MFnEnumAttribute eAttr;


    MMatrix identity;
    identity.setToIdentity();


    inheritAttributesFrom("strokeNode");




    aCurve = tAttr.create("curve", "crv", MFnNurbsCurveData::kNurbsCurve, & st); er;
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    st = addAttribute(aCurve); er;


    aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData:: kDouble);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aOverlap);
    er;

    aRandomOverlapFactor = nAttr.create("randomOverlapFactor", "rofc",
                                        MFnNumericData::kDouble);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.00);
    nAttr.setMax(1.0);
    st = addAttribute(aRandomOverlapFactor);
    er;



    aForceDip = nAttr.create("forceDip", "fcdp", MFnNumericData::kBoolean);
    nAttr.setHidden(false);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(false);
    st = addAttribute(aForceDip);
    er;


    aSubcurveMin = nAttr.create("subcurveMin", "scvn", MFnNumericData::kDouble, 0.0 );
    nAttr.setDefault(0.0);
    aSubcurveMax = nAttr.create("subcurveMax", "scvx", MFnNumericData::kDouble, 99999.0 );
    aSubcurve = nAttr.create("subcurve", "scv", aSubcurveMin, aSubcurveMax);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aSubcurve);
    er;




    aBrushRampScope = eAttr.create("brushRampScope", "brsc",
                                   StrokeRotationSpec::kStroke);
    eAttr.addField("curve", StrokeRotationSpec::kCurve);
    eAttr.addField("stroke", StrokeRotationSpec::kStroke);
    eAttr.addField("travelStroke", StrokeRotationSpec::kTravelStroke);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aBrushRampScope);
    er;

    st = attributeAffects(aCurve, aOutput);
    st = attributeAffects(aSubcurve, aOutput);
    st = attributeAffects(aRandomOverlapFactor, aOutput);
    st = attributeAffects(aOverlap, aOutput);

    st = attributeAffects(aBrushRampScope, aOutput);
    st = attributeAffects(aForceDip, aOutput);

    return (MS::kSuccess );
}

double clamp(double n, double lower = 0.0, double upper = 1.0) {
    return std::max(lower, std::min(n, upper));
}

void clampSubCurveValues(double curveLen, double &subcurveMin,  double &subcurveMax ) {
    subcurveMin = clamp(subcurveMin, 0.0, curveLen);
    subcurveMax =  clamp(subcurveMax, subcurveMin, curveLen);
}

double randomize(double n, double factor) {
    return n + (n * factor * 2.0 * (drand48() - 0.5));
}

double calculateEndDist(double startDist, double strokeLength, double randomLengthFactor,
                        double subcurveMax, double clip = 0.1) {
    double length = randomize(strokeLength, randomLengthFactor);
    double maxlen = (strokeLength * 2) - clip;
    length = clamp(length, clip, maxlen);
    return fmin((startDist + length), subcurveMax);
}
double calculateStartDist( double startDist, double endDist, double overlap,
                           double randomOverlapFactor, double clip = 0.1) {
    double thisOverlap = randomize(overlap, randomOverlapFactor);
    double result = endDist - thisOverlap;
    return fmax(result, (startDist + clip));
}


unsigned int strokeCurve::getStrokeBoundaries(
    MDataBlock &data,
    MVectorArray &result
) const  {

    /*
      generate an array of start and end param for strokes
    */


    MStatus st = MS::kSuccess;



    MDataHandle hCurve = data.inputValue(aCurve, &st ); ert;
    MObject  dCurve =  data.inputValue(aCurve).asNurbsCurveTransformed();
    double countFactor = data.inputValue(aStrokeCountFactor).asDouble();
    int seed = data.inputValue(aSeed).asInt();
    srand48(seed);

    double strokeLength = data.inputValue( aStrokeLength).asDouble();
    double randomLengthFactor = data.inputValue( aRandomLengthFactor).asDouble();
    double randomOverlapFactor = data.inputValue( aRandomOverlapFactor).asDouble();
    double overlap  =  data.inputValue( aOverlap).asDouble();

    MDataHandle hSubcurve = data.inputValue(aSubcurve);
    double subcurveMin = hSubcurve.child( aSubcurveMin).asDouble();
    double subcurveMax = hSubcurve.child( aSubcurveMax).asDouble();

    MFnNurbsCurve curveFn(dCurve, &st);
    if (st.error()) { return 0; }
    double curveLen = curveFn.length(epsilon);

    clampSubCurveValues(curveLen, subcurveMin, subcurveMax);

    randomLengthFactor = clamp(randomLengthFactor);
    randomOverlapFactor = clamp(randomOverlapFactor);

    double startDist = subcurveMin;
    double endDist;

    do {
        endDist = calculateEndDist(startDist, strokeLength, randomLengthFactor, subcurveMax);
        if (drand48() < countFactor) {
            result.append(MVector(startDist, endDist));
        }
        if (endDist >= subcurveMax) { break; }
        startDist = calculateStartDist(startDist, endDist, overlap, randomOverlapFactor);
        if (startDist >= subcurveMax) { break; }
    }
    while ( true );

    return result.length();
}

void strokeCurve::setApproach(std::vector<std::unique_ptr<Stroke> > &strokes,
                              double approachStart, double approachMid, double approachEnd) const {
    for (auto iter = strokes.begin(); iter != strokes.end(); iter++) {
        if (iter == strokes.begin() && std::next(iter) == strokes.end()) {
            (*iter)->setApproach(approachStart, approachEnd);
        }
        else if (iter == strokes.begin() ) {
            (*iter)->setApproach(approachStart, approachMid);
        }
        else if (std::next(iter) == strokes.end()) {
            (*iter)->setApproach(approachMid, approachEnd);
        }
        else {
            (*iter)->setApproach(approachMid, approachMid);
        }
    }
}



MStatus strokeCurve::generateStrokeGeometry(MDataBlock &data,
        std::vector < strokeGeom > *geom) const
{

    MStatus st;

    std::vector<std::unique_ptr <Stroke> > strokes;

    if (! data.inputValue(aActive).asBool()) {
        return MS:: kSuccess;
    }

    MVectorArray boundaries;
    unsigned numStrokeGroups = getStrokeBoundaries(data, boundaries);

    if (! numStrokeGroups) {
        return MS::kSuccess;
    }


    MDataHandle hCurve = data.inputValue(aCurve, & st); ert;
    MObject  dCurve = data.inputValue(aCurve).asNurbsCurveTransformed();

    MMatrix planeMatrix = data.inputValue(strokeCurve:: aPlaneMatrix).asMatrix();
    MMatrix inversePlaneMatrix = planeMatrix.inverse();
    MVector planeNormal = (MVector:: zAxis * planeMatrix).normal();


    MFnNurbsCurve curveFn(dCurve, & st); ert;
    double curveLength = curveFn.length(epsilon);
    double pointDensity = data.inputValue(aPointDensity).asDouble();
    if (pointDensity < 0.001) {
        pointDensity = 0.001;
    }


    MDataHandle hLift = data.inputValue(aLift);
    double liftLength = hLift.child(aLiftLength).asDouble();
    double liftHeight = hLift.child(aLiftHeight).asDouble();
    double liftBias = hLift.child(aLiftBias).asDouble();
    MVector lift(liftLength, liftHeight, liftBias);

    MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
    double approachStart = hApproachDistance.child(aApproachDistanceStart).asDouble();
    double approachMid = hApproachDistance.child(aApproachDistanceMid).asDouble();
    double approachEnd = hApproachDistance.child(aApproachDistanceEnd).asDouble();

    bool backstroke = data.inputValue(aBackstroke).asBool();

    short repeats = data.inputValue(aRepeats).asShort();
    double repeatOffset = data.inputValue(aRepeatOffset).asDouble();


    MDataHandle hStrokeProfileScale = data.inputValue(aStrokeProfileScale);

    double strokeProfileScaleMin = hStrokeProfileScale.child(
                                       aStrokeProfileScaleMin).asDouble();
    double strokeProfileScaleMax = hStrokeProfileScale.child(
                                       aStrokeProfileScaleMax).asDouble();

    bool repeatMirror = data.inputValue(aRepeatMirror).asBool();
    bool repeatOscillate = data.inputValue(aRepeatOscillate).asBool();

    double pivotFraction = data.inputValue(aPivotFraction).asDouble();

    // bool follow = data.inputValue(aBrushFollowStroke).asBool();


    StrokeRotationSpec rotSpec;
    rotSpec.tiltRampAtt =  strokeCurve::aBrushTiltRamp;
    rotSpec.bankRampAtt =  strokeCurve::aBrushBankRamp;
    rotSpec.twistRampAtt =  strokeCurve::aBrushTwistRamp;

    MDataHandle hRangeHandle = data.inputValue(aBrushTiltRange);
    rotSpec.tiltRampMin = hRangeHandle.child(aBrushTiltRangeMin).asAngle().asRadians();
    rotSpec.tiltRampMax = hRangeHandle.child(aBrushTiltRangeMax).asAngle().asRadians();

    hRangeHandle = data.inputValue(aBrushBankRange);
    rotSpec.bankRampMin = hRangeHandle.child(aBrushBankRangeMin).asAngle().asRadians();
    rotSpec.bankRampMax = hRangeHandle.child(aBrushBankRangeMax).asAngle().asRadians();

    hRangeHandle = data.inputValue(aBrushTwistRange);
    rotSpec.twistRampMin = hRangeHandle.child(aBrushTwistRangeMin).asAngle().asRadians();
    rotSpec.twistRampMax = hRangeHandle.child(aBrushTwistRangeMax).asAngle().asRadians();
    rotSpec.rampScope = StrokeRotationSpec::Scope(data.inputValue(aBrushRampScope).asShort());
    rotSpec.followStroke =  data.inputValue(aBrushFollowStroke).asBool();


    MObject thisObj = thisMObject();

    for (int i = 0; i < numStrokeGroups; ++i) {
        const double &startDist = boundaries[i].x;
        const double &endDist = boundaries[i].y;

        unsigned strokeGroupSize = Stroke::factory(
                                       thisObj,
                                       dCurve,
                                       inversePlaneMatrix,
                                       planeNormal,
                                       curveLength,
                                       startDist,
                                       endDist,
                                       pointDensity,
                                       liftLength,
                                       liftHeight,
                                       liftBias,
                                       strokeCurve:: aStrokeProfileRamp,
                                       strokeProfileScaleMin,
                                       strokeProfileScaleMax,
                                       rotSpec,
                                       backstroke,
                                       repeats,
                                       repeatOffset,
                                       repeatMirror,
                                       repeatOscillate,
                                       pivotFraction,
                                       strokes);
    }

    setApproach(
        strokes, approachStart, approachMid, approachEnd);



    bool force = data.inputValue(aForceDip).asBool();
    short brushId = data.inputValue(aBrushId).asShort();
    short paintId = data.inputValue(aPaintId).asShort();


    bool first = true;
    for (auto &citer : strokes)
    {
        bool doForce = force && first;
        geom ->push_back(strokeGeom(*citer, brushId, paintId, doForce));
        first = false;
    }


    return MS::kSuccess;
}

void strokeCurve:: postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
