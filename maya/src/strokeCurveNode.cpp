
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>

#include <maya/MFnPluginData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>

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
#include "strokeCurveGeom.h"

#include "strokeCurveData.h"
#include "strokeCurveNode.h"
#include "stroke.h"
#include "backstroke.h"


#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);

// per-curve, pass through
MObject strokeCurve::aForceDip;
MObject strokeCurve::aBrushId;
MObject strokeCurve::aPaintId;

MObject strokeCurve::aCurve;
MObject strokeCurve::aSubcurveMin;
MObject strokeCurve::aSubcurveMax;
MObject strokeCurve::aSubcurve;
MObject strokeCurve::aPointDensity;
MObject strokeCurve::aStrokeLength;
MObject strokeCurve::aRandomLengthFactor;
MObject strokeCurve::aRandomOverlapFactor;
MObject strokeCurve::aBackstroke;
MObject strokeCurve::aRepeats;
MObject strokeCurve::aRepeatOffset;
MObject strokeCurve::aRepeatMirror;
MObject strokeCurve::aRepeatOscillate;

MObject strokeCurve::aSeed;

MObject strokeCurve::aLiftLength;
MObject strokeCurve::aLiftBias;
MObject strokeCurve::aLiftHeight;
MObject strokeCurve::aLift;

MObject strokeCurve::aStrokeProfileRamp;
MObject strokeCurve::aStrokeProfileScaleMin;
MObject strokeCurve::aStrokeProfileScaleMax;
MObject strokeCurve::aStrokeProfileScale;


MObject strokeCurve::aActive;
MObject strokeCurve::aStrokeCountFactor;

MObject strokeCurve::aOverlap;
MObject strokeCurve::aPivotFraction;
MObject strokeCurve::aRepeatPivot;

MObject strokeCurve::aBrushRampScope;
MObject strokeCurve::aBrushTiltRamp;
MObject strokeCurve::aBrushBankRamp;
MObject strokeCurve::aBrushTwistRamp;

MObject strokeCurve::aBrushFollowStroke;


MObject strokeCurve::aApproachDistanceStart;
MObject strokeCurve::aApproachDistanceMid;
MObject strokeCurve::aApproachDistanceEnd;
MObject strokeCurve::aApproachDistance;

MObject strokeCurve::aPlaneMatrix;

MObject strokeCurve::aOutput;


MTypeId strokeCurve::id( k_strokeCurve );

strokeCurve::strokeCurve() {}

strokeCurve::~strokeCurve() {}

void *strokeCurve::creator() {
  return new strokeCurve();
}

const double epsilon = 0.0001;


MStatus strokeCurve::initialize()
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

  aPlaneMatrix = mAttr.create( "planeMatrix", "pmat",  MFnMatrixAttribute::kDouble );
  mAttr.setStorable( false );
  mAttr.setHidden( true );
  mAttr.setDefault(identity);
  st = addAttribute(aPlaneMatrix); er;

  aStrokeCountFactor = nAttr.create( "strokeCountFactor",
                                     "stcf", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(1.0);
  st = addAttribute(aStrokeCountFactor); er;


  aCurve = tAttr.create( "curve", "crv", MFnNurbsCurveData::kNurbsCurve, &st); er;
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  st = addAttribute(aCurve); er;

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  nAttr.setDefault( 1.0 );
  st = addAttribute(aPointDensity); er;

  aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.01);
  nAttr.setDefault(10.0);
  st = addAttribute(aStrokeLength); er;

  aRandomLengthFactor = nAttr.create( "randomLengthFactor", "rlfc",
                                      MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  st = addAttribute(aRandomLengthFactor); er;


  aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aOverlap); er;

  aRandomOverlapFactor = nAttr.create( "randomOverlapFactor", "rofc",
                                       MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  st = addAttribute(aRandomOverlapFactor); er;



  aBackstroke = nAttr.create( "reverseDirection", "revd", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(false);
  st = addAttribute(aBackstroke); er;



  aRepeats = nAttr.create("repeats", "rpts", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeats); er;

  aRepeatOffset = nAttr.create("repeatOffset", "rpof", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  addAttribute(aRepeatOffset);


  aRepeatMirror = nAttr.create( "repeatMirror", "rpmr", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatMirror);



  aRepeatOscillate = nAttr.create( "repeatOscillate", "rpoc", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatOscillate);




  aSeed = nAttr.create( "seed", "sd", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aSeed); er;


  aPivotFraction = nAttr.create("pivotFraction", "pfrc", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setMin(0.0);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.5);
  addAttribute(aPivotFraction);


  aRepeatPivot = nAttr.create( "repeatPivot", "rpiv", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  addAttribute(aRepeatPivot);




  aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kShort); er;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aBrushId); er;

  aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kShort); er;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aPaintId); er;

  aActive = nAttr.create( "active", "act", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aActive); er;


  aBrushFollowStroke = nAttr.create( "followStroke", "fst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aBrushFollowStroke); er;

  aForceDip = nAttr.create( "forceDip", "fcdp", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(false);
  st = addAttribute(aForceDip); er;


  aSubcurveMin = nAttr.create( "subcurveMin", "scvn", MFnNumericData::kDouble, 0.0 );
  nAttr.setDefault(0.0 );
  aSubcurveMax = nAttr.create( "subcurveMax", "scvx", MFnNumericData::kDouble, 99999.0 );
  aSubcurve = nAttr.create( "subcurve", "scv", aSubcurveMin, aSubcurveMax);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aSubcurve); er;


  aApproachDistanceStart = nAttr.create( "approachDistanceStart",
                                         "apds", MFnNumericData::kDouble);
  nAttr.setDefault(5.0);
  aApproachDistanceMid = nAttr.create( "approachDistanceMid",
                                       "apdm", MFnNumericData::kDouble);
  nAttr.setDefault(3.0);
  aApproachDistanceEnd = nAttr.create( "approachDistanceEnd",
                                       "apde", MFnNumericData::kDouble);
  nAttr.setDefault(5.0);
  aApproachDistance = nAttr.create( "approachDistance",
                                    "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);

  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aApproachDistance);


  aLiftLength = nAttr.create( "liftLength", "llg", MFnNumericData::kDouble, 2.0 );
  aLiftHeight = nAttr.create( "liftHeight", "lht", MFnNumericData::kDouble, 2.0 );
  aLiftBias = nAttr.create( "liftBias", "lbi", MFnNumericData::kDouble, 0.0);
  aLift = nAttr.create( "lift", "lft", aLiftLength, aLiftHeight,
                        aLiftBias );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aLift); er;


  aStrokeProfileRamp = MRampAttribute::createCurveRamp("strokeProfileRamp", "spr");
  st = addAttribute( aStrokeProfileRamp ); er;



  aStrokeProfileScaleMin = nAttr.create( "strokeProfileScaleMin", "spscn",
                                         MFnNumericData::kDouble, -1.0 );
  aStrokeProfileScaleMax = nAttr.create( "strokeProfileScaleMax", "spscx",
                                         MFnNumericData::kDouble, 1.0 );
  aStrokeProfileScale  = nAttr.create("strokeProfileScale", "spsc",
                                      aStrokeProfileScaleMin, aStrokeProfileScaleMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aStrokeProfileScale); er;




  aBrushRampScope = eAttr.create( "brushRampScope", "brsc",
                                  Stroke::kStroke);
  eAttr.addField("curve", Stroke::kCurve);
  eAttr.addField("stroke", Stroke::kStroke);
  eAttr.addField("travelStroke", Stroke::kTravelStroke);


  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute( aBrushRampScope ); er;

  aBrushTiltRamp  = MRampAttribute::createCurveRamp("brushTiltRamp", "brtl");
  st = addAttribute( aBrushTiltRamp ); er;
  aBrushBankRamp  = MRampAttribute::createCurveRamp("brushBankRamp", "brbk");
  st = addAttribute( aBrushBankRamp ); er;
  aBrushTwistRamp  = MRampAttribute::createCurveRamp("brushTwistRamp", "brtw");
  st = addAttribute( aBrushTwistRamp ); er;




  aOutput = tAttr.create("output", "out", strokeCurveData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  // tAttr.setCached(false);
  addAttribute(aOutput);


  st = attributeAffects(aCurve, aOutput);
  st = attributeAffects(aSubcurve, aOutput);
  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aStrokeLength, aOutput);
  st = attributeAffects(aRandomLengthFactor, aOutput);
  st = attributeAffects(aRandomOverlapFactor, aOutput);
  st = attributeAffects(aBackstroke, aOutput);
  st = attributeAffects(aRepeats, aOutput);
  st = attributeAffects(aRepeatOffset, aOutput);
  st = attributeAffects(aRepeatMirror, aOutput);
  st = attributeAffects(aRepeatOscillate, aOutput);
  st = attributeAffects(aSeed, aOutput);
  st = attributeAffects(aLift, aOutput);
  st = attributeAffects(aStrokeProfileRamp, aOutput);
  st = attributeAffects(aStrokeProfileScale, aOutput);
  st = attributeAffects(aActive, aOutput);
  st = attributeAffects(aStrokeCountFactor, aOutput);
  st = attributeAffects(aOverlap, aOutput);
  st = attributeAffects(aPivotFraction, aOutput);
  st = attributeAffects(aRepeatPivot, aOutput);



  st = attributeAffects(aBrushRampScope, aOutput);
  st = attributeAffects(aBrushTiltRamp, aOutput);
  st = attributeAffects(aBrushBankRamp, aOutput);
  st = attributeAffects(aBrushTwistRamp, aOutput);
  st = attributeAffects(aBrushFollowStroke, aOutput);
  st = attributeAffects(aPlaneMatrix, aOutput);

  st = attributeAffects(aApproachDistance, aOutput);
  st = attributeAffects(aForceDip, aOutput);
  st = attributeAffects(aBrushId, aOutput);
  st = attributeAffects(aPaintId, aOutput);




  return ( MS::kSuccess );

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



MStatus strokeCurve::generateStrokes(MDataBlock &data,
                                     std::vector<std::unique_ptr<Stroke> > &strokes ) const {

  MStatus st;

  if (! data.inputValue(aActive).asBool()) {
    return MS::kSuccess;
  }

  MVectorArray boundaries;
  unsigned numStrokeGroups =  getStrokeBoundaries( data, boundaries);

  if (! numStrokeGroups) {
    return MS::kSuccess;
  }


  MDataHandle hCurve = data.inputValue(aCurve, &st ); ert;
  MObject  dCurve =  data.inputValue(aCurve).asNurbsCurveTransformed();

  MMatrix planeMatrix = data.inputValue(strokeCurve::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();


  MFnNurbsCurve curveFn(dCurve, &st) ; ert;
  double curveLength = curveFn.length(epsilon);
  double pointDensity =  data.inputValue(aPointDensity).asDouble() ;
  if (pointDensity < 0.001) { pointDensity = 0.001; }


  MDataHandle hLift = data.inputValue(aLift);
  double liftLength = hLift.child( aLiftLength).asDouble();
  double liftHeight = hLift.child( aLiftHeight).asDouble();
  double liftBias = hLift.child( aLiftBias).asDouble();
  MVector lift(liftLength, liftHeight, liftBias);

  MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
  double approachStart = hApproachDistance.child( aApproachDistanceStart).asDouble();
  double approachMid = hApproachDistance.child( aApproachDistanceMid).asDouble();
  double approachEnd = hApproachDistance.child( aApproachDistanceEnd).asDouble();

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

  bool follow =  data.inputValue( aBrushFollowStroke).asBool();



  Stroke::Scope brushRampScope =
    Stroke::Scope( data.inputValue(aBrushRampScope).asShort());

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
                                 strokeCurve::aStrokeProfileRamp,
                                 strokeProfileScaleMin,
                                 strokeProfileScaleMax,
                                 strokeCurve::aBrushTiltRamp,
                                 strokeCurve::aBrushBankRamp,
                                 strokeCurve::aBrushTwistRamp,
                                 brushRampScope,
                                 follow,
                                 backstroke,
                                 repeats,
                                 repeatOffset,
                                 repeatMirror,
                                 repeatOscillate,
                                 pivotFraction,
                                 /*                   brushId,
                                                    paintId,*/
                                 strokes);

  }

  setApproach(
    strokes, approachStart, approachMid, approachEnd);

  return MS::kSuccess;
}

MStatus strokeCurve::compute( const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  MString method("strokeCurve::compute");
  if (!(
        plug == aOutput
      )) { return ( MS::kUnknownParameter ); }



  MMatrix planeMatrix = data.inputValue(strokeCurve::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();

  std::vector<std::unique_ptr<Stroke> > strokes;

  st = generateStrokes(data, strokes) ;

  bool force = data.inputValue(aForceDip).asBool();
  short brushId = data.inputValue(aBrushId).asShort();
  short paintId = data.inputValue(aPaintId).asShort();


  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeCurveData::id);

  MObject dOut = fnOut.create(kdid , &st );
  strokeCurveData *newData = (strokeCurveData *)fnOut.data(&st); er;
  strokeCurveGeom *geom = newData->fGeometry;

  geom->create(strokes, force, brushId, paintId );

  hOutput.set( newData );
  data.setClean( plug );

  return MS::kSuccess;

}

void strokeCurve::postConstructor()
{
  setExistWithoutInConnections(false);
  setExistWithoutOutConnections(true);
}




