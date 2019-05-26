
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include <maya/MFnNurbsCurveData.h>

#include "curveStrokeNode.h"
#include "stroke.h"
#include "strokeRotationSpec.h"
#include "strokeRepeatSpec.h"

#include <jMayaIds.h>
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);


MObject curveStrokeNode::aCurve;
MObject curveStrokeNode::aSubcurveMin;
MObject curveStrokeNode::aSubcurveMax;
MObject curveStrokeNode::aSubcurve;
MObject curveStrokeNode::aSubcurveMethod;
MObject curveStrokeNode::aStrokeLength;
MObject curveStrokeNode::aRandomLengthFactor;
MObject curveStrokeNode::aRandomOverlapFactor;
MObject curveStrokeNode::aOverlap;
MObject curveStrokeNode::aBrushRampScope;

MObject curveStrokeNode::aContactRamp;
MObject curveStrokeNode::aContactRampResolution;

MTypeId curveStrokeNode:: id(k_curveStrokeNode );

curveStrokeNode:: curveStrokeNode() {}

curveStrokeNode:: ~curveStrokeNode() {}

void *curveStrokeNode::creator() {
    return new curveStrokeNode();
}

const double epsilon = 0.0001;


MStatus curveStrokeNode:: initialize()
{
    MStatus st;

    MFnNumericAttribute nAttr;
    MFnTypedAttribute tAttr;
    MFnEnumAttribute eAttr;

    MMatrix identity;
    identity.setToIdentity();

    inheritAttributesFrom("strokeNode");

    aCurve = tAttr.create("curve", "crv", MFnNurbsCurveData::kNurbsCurve, & st); mser;
    tAttr.setReadable(false);
    tAttr.setStorable(false);
    tAttr.setKeyable(true);

    st = addAttribute(aCurve); mser;


    aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kDouble);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.01);
    nAttr.setDefault(10.0);
    st = addAttribute(aStrokeLength); mser;

    aRandomLengthFactor = nAttr.create( "randomLengthFactor", "rlfc",
                                        MFnNumericData::kDouble);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.00);
    nAttr.setMax(1.0);
    st = addAttribute(aRandomLengthFactor); mser;

    aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData:: kDouble);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aOverlap);
    mser;

    aRandomOverlapFactor = nAttr.create("randomOverlapFactor", "rofc",
                                        MFnNumericData::kDouble);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setKeyable(true);
    nAttr.setMin(0.00);
    nAttr.setMax(1.0);
    st = addAttribute(aRandomOverlapFactor);
    mser;

    aSubcurveMin = nAttr.create("subcurveMin", "scvn", MFnNumericData::kDouble, 0.0 );
    nAttr.setDefault(0.0);
    aSubcurveMax = nAttr.create("subcurveMax", "scvx", MFnNumericData::kDouble, 99999.0 );
    aSubcurve = nAttr.create("subcurve", "scv", aSubcurveMin, aSubcurveMax);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    st = addAttribute(aSubcurve);
    mser;

    aSubcurveMethod = eAttr.create("subcurveMethod", "scmt",
                                   curveStrokeNode::kLength);
    eAttr.addField("length", curveStrokeNode::kLength);
    eAttr.addField("bookends", curveStrokeNode::kBookends);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aSubcurveMethod);
    mser;

    aBrushRampScope = eAttr.create("brushRampScope", "brsc",
                                   StrokeRotationSpec::kStroke);
    eAttr.addField("stroke", StrokeRotationSpec::kStroke);
    eAttr.addField("travelStroke", StrokeRotationSpec::kTravelStroke);
    eAttr.addField("curve", StrokeRotationSpec::kCurve);
    eAttr.setKeyable(true);
    eAttr.setHidden(false);
    st = addAttribute(aBrushRampScope);
    mser;


    aContactRamp  = MRampAttribute::createCurveRamp("contactRamp", "crmp");
    st = addAttribute( aContactRamp ); mser;

    aContactRampResolution = nAttr.create("contactRampResolution", "crrs",
                                          MFnNumericData::kInt); mser;
    nAttr.setDefault(8);
    nAttr.setHidden(false);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setWritable(true);
    st = addAttribute(aContactRampResolution); mser;

    st = attributeAffects(aCurve, aOutput);
    st = attributeAffects(aSubcurveMin, aOutput);
    st = attributeAffects(aSubcurveMax, aOutput);
    st = attributeAffects(aSubcurve, aOutput);


    st = attributeAffects(aStrokeLength, aOutput);
    st = attributeAffects(aRandomLengthFactor, aOutput);

    st = attributeAffects(aRandomOverlapFactor, aOutput);
    st = attributeAffects(aOverlap, aOutput);
    st = attributeAffects(aSubcurveMethod, aOutput);

    st = attributeAffects(aBrushRampScope, aOutput);
    st = attributeAffects(aContactRamp, aOutput);
    st = attributeAffects(aContactRampResolution, aOutput);

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


unsigned int curveStrokeNode::getStrokeBoundaries(
    MDataBlock &data,
    MVectorArray &result
) const  {

    /*
      generate an array of start and end param for strokes
    */


    MStatus st = MS::kSuccess;



    MDataHandle hCurve = data.inputValue(aCurve, &st ); msert;
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
    SubcurveMethod scMethod = SubcurveMethod(data.inputValue(aSubcurveMethod).asShort());

    MFnNurbsCurve curveFn(dCurve, &st);
    if (st.error()) { return 0; }
    double curveLen = curveFn.length(epsilon);

    if (scMethod == curveStrokeNode::kBookends) {
        subcurveMax = curveLen - subcurveMax;
    }
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


MStatus curveStrokeNode::generateStrokeGeometry(MDataBlock &data,
        std::vector < Stroke > *pStrokes) const
{

    MStatus st;

    if (! data.inputValue(aActive).asBool()) {
        return MS:: kSuccess;
    }

    MVectorArray boundaries;
    if (! getStrokeBoundaries(data, boundaries)) {
        return MS::kSuccess;
    }

    MDataHandle hCurve = data.inputValue(aCurve, & st); msert;
    MObject  dCurve = data.inputValue(aCurve).asNurbsCurveTransformed();

    MFnNurbsCurve curveFn(dCurve, & st); msert;
    double curveLength = curveFn.length(epsilon);
    double pointDensity = data.inputValue(aPointDensity).asDouble();
    if (pointDensity < 0.001) {
        pointDensity = 0.001;
    }
    int minimumPoints = data.inputValue(aMinimumPoints).asInt();

    double entryLength = data.inputValue(aEntryLength).asDouble();
    double exitLength = data.inputValue(aExitLength).asDouble();
    bool localContact = data.inputValue(aLocalContact).asBool();


    Stroke::TransitionBlendMethod transitionBlendMethod = Stroke::TransitionBlendMethod(
                data.inputValue(
                    aTransitionBlendMethod).asShort());

    Stroke::DirectionMethod strokeDirection = Stroke::DirectionMethod(data.inputValue(
                aStrokeDirection).asShort());

    double pivotParam = data.inputValue(aPivotFraction).asDouble();

    StrokeRotationSpec rotSpec;
    rotSpec.tiltRampAtt =  strokeNode::aBrushTiltRamp;
    rotSpec.bankRampAtt =  strokeNode::aBrushBankRamp;
    rotSpec.twistRampAtt =  strokeNode::aBrushTwistRamp;

    MRampAttribute contactRampAttr( thisMObject(), aContactRamp, &st ); msert;

    int contactRampResolution = data.inputValue(aContactRampResolution).asInt();


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

    StrokeRepeatSpec repeatSpec;
    repeatSpec.repeats = data.inputValue(aRepeats).asShort();
    repeatSpec.offset = data.inputValue(aRepeatOffset).asDouble();
    repeatSpec.mirror = data.inputValue(aRepeatMirror).asBool();
    repeatSpec.oscillate = data.inputValue(aRepeatOscillate).asBool();
    repeatSpec.fan = data.inputValue(aRepeatFan).asAngle().asRadians();

    short brushId = data.inputValue(aBrushId).asShort();
    short paintId = data.inputValue(aPaintId).asShort();
    int layerId = data.inputValue(aLayerId).asInt();


    //////////////////////////
    MDoubleArray contacts;
    if (contactRampResolution < 2)
    {
        contactRampResolution = 2;
    }
    float gap = 1.0f / float(contactRampResolution - 1);

    for (int i = 0; i < contactRampResolution; ++i)
    {
        float sample = float(i) * gap;
        float res;
        contactRampAttr.getValueAtPosition( sample, res, &st ); mser;
        // cerr <<endl sample << ", ";
        /* We want the ramp to visually agree with the motion of the brush
        1.0 on the ramp is high - so 0 is full contact. Therefore, reverse
        the value.
        */
        contacts.append(double(1.0 - res));
    }
    // cerr << endl << "contacts" << contacts << endl;
    //////////////////////////

    MObject thisObj = thisMObject();
    for (int strokeId = 0; strokeId < boundaries.length(); ++strokeId) {
        const double &startDist = boundaries[strokeId].x;
        const double &endDist = boundaries[strokeId].y;

        unsigned strokeGroupSize = Stroke::create(
                                       thisObj,
                                       dCurve,
                                       contacts,
                                       localContact,
                                       curveLength,
                                       startDist,
                                       endDist,
                                       entryLength,
                                       exitLength,
                                       transitionBlendMethod,
                                       pointDensity,
                                       minimumPoints,
                                       rotSpec,
                                       repeatSpec,
                                       strokeDirection,
                                       pivotParam,
                                       strokeId,
                                       brushId,
                                       paintId,
                                       layerId,
                                       brushId,
                                       pStrokes);
    }





    return MS::kSuccess;
}

void curveStrokeNode:: postConstructor()
{
    setExistWithoutInConnections(false);
    setExistWithoutOutConnections(true);
}
