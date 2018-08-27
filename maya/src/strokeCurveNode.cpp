
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
MObject strokeCurve::aStrokeRotationTexture;
MObject strokeCurve::aStrokeTranslationTexture;
MObject strokeCurve::aStrokeTranslationSampleDistance;

// output
MObject strokeCurve::aOutTargets;
MObject strokeCurve::aOutTangents;
MObject strokeCurve::aOutPositions;

// one per stroke
MObject strokeCurve::aOutCounts;
MObject strokeCurve::aOutArcLengths;

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

  aStrokeRotationTexture = nAttr.create( "strokeRotationTexture", "srtx",
                                         MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aStrokeRotationTexture); er;

  aStrokeTranslationTexture = nAttr.create( "strokeTranslationTexture", "strx",
                              MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aStrokeTranslationTexture); er;

  aStrokeTranslationSampleDistance = nAttr.create( "strokeTranslationSampleDistance",
                                     "stsd", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setDefault(5.0);
  st = addAttribute(aStrokeTranslationSampleDistance); er;

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





  // aApproachDistanceStart = nAttr.create( "approachDistanceStart",
  //                                        "apds", MFnNumericData::kDouble);
  // aApproachDistanceMid = nAttr.create( "approachDistanceMid",
  //                                      "apdm", MFnNumericData::kDouble);
  // aApproachDistanceEnd = nAttr.create( "approachDistanceEnd",
  //                                      "apde", MFnNumericData::kDouble);
  // aApproachDistance = nAttr.create( "approachDistance",
  //                                   "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);
  // nAttr.setKeyable(true);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // st = addAttribute(aApproachDistance); er;

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




  aOutCounts  = tAttr.create("outCounts", "octs", MFnData::kIntArray, &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutCounts ); er;

  aOutTargets = tAttr.create("outTargets", "otg", MFnData::kMatrixArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutTargets ); er;

  aOutTangents = tAttr.create("outTangents", "otan", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutTangents ); er;

  aOutPositions = tAttr.create("outPositions", "opos", MFnData::kVectorArray, &st);
  er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutPositions ); er;



  aOutArcLengths = tAttr.create("outArcLengths", "oarc", MFnData::kDoubleArray,
                                &st); er;
  tAttr.setStorable(false);
  tAttr.setReadable(true);
  st = addAttribute(aOutArcLengths ); er;



  aOutput = tAttr.create("output", "out", strokeCurveData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  // tAttr.setCached(false);
  addAttribute(aOutput);

  // setAttr "strokeCurve5.brushId" 1;
  // setAttr "strokeCurve5.paintId" 1;
  // setAttr "strokeCurve5.repeatMirror" 0;
  // setAttr "strokeCurve5.repeatOscillate" 0;
  // setAttr "strokeCurve5.subcurveMax" 100;
  // setAttr "strokeCurve5.brushId" 10;
  // setAttr "strokeCurve5.overlap" 0;
  // setAttr "strokeCurve5.randomLengthFactor" 0;
  // setAttr "strokeCurve5.randomOverlapFactor" 0;
  // setAttr "strokeCurve5.approachDistanceStart" 1;
  // setAttr "strokeCurve5.approachDistanceMid" 1;
  // setAttr "strokeCurve5.approachDistanceEnd" 1;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_Position" 0;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.44;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_Position" 0;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_Position" 0;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.46;
  // setAttr "strokeCurve5.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.5;
  // setAttr "strokeCurve5.brushTiltRamp[0].brushTiltRamp_Position" 0;
  // setAttr "strokeCurve5.brushTiltRamp[0].brushTiltRamp_FloatValue" 0.14;
  // setAttr "strokeCurve5.brushTiltRamp[0].brushTiltRamp_FloatValue" 0.5;
  // setAttr "strokeCurve5.brushBankRamp[0].brushBankRamp_FloatValue" 0.5;
  // setAttr "strokeCurve5.brushTwistRamp[0].brushTwistRamp_FloatValue" 0.5;
  // setAttr "strokeCurve5.strokeLength" 30;
  // setAttr "strokeCurve5.strokeLength" 10;
  // setAttr "strokeCurve5.pointDensity" 0.5;
  // setAttr "strokeCurve5.pointDensity" 1;
  // setAttr "strokeCurve5.brushId" 1;


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
  st = attributeAffects(aBrushRampScope, aOutput);
  st = attributeAffects(aBrushTiltRamp, aOutput);
  st = attributeAffects(aBrushBankRamp, aOutput);
  st = attributeAffects(aBrushTwistRamp, aOutput);
  st = attributeAffects(aBrushFollowStroke, aOutput);
  st = attributeAffects(aPlaneMatrix, aOutput);
  st = attributeAffects(aStrokeRotationTexture, aOutput);
  st = attributeAffects(aStrokeTranslationTexture, aOutput);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutput);
  st = attributeAffects(aApproachDistance, aOutput);
  st = attributeAffects(aForceDip, aOutput);
  st = attributeAffects(aBrushId, aOutput);
  st = attributeAffects(aPaintId, aOutput);



  st = attributeAffects(aCurve, aOutTargets);
  st = attributeAffects(aSubcurve, aOutTargets);
  st = attributeAffects(aPointDensity, aOutTargets);
  st = attributeAffects(aStrokeLength, aOutTargets);
  st = attributeAffects(aRandomLengthFactor, aOutTargets);
  st = attributeAffects(aRandomOverlapFactor, aOutTargets);
  st = attributeAffects(aBackstroke, aOutTargets);
  st = attributeAffects(aRepeats, aOutTargets);
  st = attributeAffects(aRepeatOffset, aOutTargets);
  st = attributeAffects(aRepeatMirror, aOutTargets);
  st = attributeAffects(aRepeatOscillate, aOutTargets);
  st = attributeAffects(aSeed, aOutTargets);
  st = attributeAffects(aLift, aOutTargets);
  st = attributeAffects(aStrokeProfileRamp, aOutTargets);
  st = attributeAffects(aStrokeProfileScale, aOutTargets);
  st = attributeAffects(aActive, aOutTargets);
  st = attributeAffects(aStrokeCountFactor, aOutTargets);
  st = attributeAffects(aOverlap, aOutTargets);
  st = attributeAffects(aPivotFraction, aOutTargets);
  st = attributeAffects(aBrushRampScope, aOutTargets);
  st = attributeAffects(aBrushTiltRamp, aOutTargets);
  st = attributeAffects(aBrushBankRamp, aOutTargets);
  st = attributeAffects(aBrushTwistRamp, aOutTargets);
  st = attributeAffects(aBrushFollowStroke, aOutTargets);
  st = attributeAffects(aApproachDistance, aOutTargets);
  st = attributeAffects(aPlaneMatrix, aOutTargets);
  st = attributeAffects(aStrokeRotationTexture, aOutTargets);
  st = attributeAffects(aStrokeTranslationTexture, aOutTargets);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutTargets);

  st = attributeAffects(aCurve, aOutTangents);
  st = attributeAffects(aSubcurve, aOutTangents);
  st = attributeAffects(aPointDensity, aOutTangents);
  st = attributeAffects(aStrokeLength, aOutTangents);
  st = attributeAffects(aRandomLengthFactor, aOutTangents);
  st = attributeAffects(aRandomOverlapFactor, aOutTangents);
  st = attributeAffects(aBackstroke, aOutTangents);
  st = attributeAffects(aRepeats, aOutTangents);
  st = attributeAffects(aRepeatOffset, aOutTangents);
  st = attributeAffects(aRepeatMirror, aOutTangents);
  st = attributeAffects(aRepeatOscillate, aOutTangents);
  st = attributeAffects(aSeed, aOutTangents);
  st = attributeAffects(aLift, aOutTangents);
  st = attributeAffects(aStrokeProfileRamp, aOutTangents);
  st = attributeAffects(aStrokeProfileScale, aOutTangents);
  st = attributeAffects(aActive, aOutTangents);
  st = attributeAffects(aStrokeCountFactor, aOutTangents);
  st = attributeAffects(aOverlap, aOutTangents);
  st = attributeAffects(aPivotFraction, aOutTangents);
  st = attributeAffects(aBrushRampScope, aOutTangents);
  st = attributeAffects(aBrushTiltRamp, aOutTangents);
  st = attributeAffects(aBrushBankRamp, aOutTangents);
  st = attributeAffects(aBrushTwistRamp, aOutTangents);
  st = attributeAffects(aBrushFollowStroke, aOutTangents);
  st = attributeAffects(aApproachDistance, aOutTangents);
  st = attributeAffects(aPlaneMatrix, aOutTangents);
  st = attributeAffects(aStrokeRotationTexture, aOutTangents);
  st = attributeAffects(aStrokeTranslationTexture, aOutTangents);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutTangents);


  st = attributeAffects(aCurve, aOutPositions);
  st = attributeAffects(aSubcurve, aOutPositions);
  st = attributeAffects(aPointDensity, aOutPositions);
  st = attributeAffects(aStrokeLength, aOutPositions);
  st = attributeAffects(aRandomLengthFactor, aOutPositions);
  st = attributeAffects(aRandomOverlapFactor, aOutPositions);
  st = attributeAffects(aBackstroke, aOutPositions);
  st = attributeAffects(aRepeats, aOutPositions);
  st = attributeAffects(aRepeatOffset, aOutPositions);
  st = attributeAffects(aRepeatMirror, aOutPositions);
  st = attributeAffects(aRepeatOscillate, aOutPositions);
  st = attributeAffects(aSeed, aOutPositions);
  st = attributeAffects(aLift, aOutPositions);
  st = attributeAffects(aStrokeProfileRamp, aOutPositions);
  st = attributeAffects(aStrokeProfileScale, aOutPositions);
  st = attributeAffects(aActive, aOutPositions);
  st = attributeAffects(aStrokeCountFactor, aOutPositions);
  st = attributeAffects(aOverlap, aOutPositions);
  st = attributeAffects(aPivotFraction, aOutPositions);
  st = attributeAffects(aBrushRampScope, aOutPositions);
  st = attributeAffects(aBrushTiltRamp, aOutPositions);
  st = attributeAffects(aBrushBankRamp, aOutPositions);
  st = attributeAffects(aBrushTwistRamp, aOutPositions);
  st = attributeAffects(aBrushFollowStroke, aOutPositions);
  st = attributeAffects(aApproachDistance, aOutPositions);
  st = attributeAffects(aPlaneMatrix, aOutPositions);
  st = attributeAffects(aStrokeRotationTexture, aOutPositions);
  st = attributeAffects(aStrokeTranslationTexture, aOutPositions);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutPositions);



  st = attributeAffects(aCurve, aOutCounts);
  st = attributeAffects(aSubcurve, aOutCounts);
  st = attributeAffects(aPointDensity, aOutCounts);
  st = attributeAffects(aStrokeLength, aOutCounts);
  st = attributeAffects(aRandomLengthFactor, aOutCounts);
  st = attributeAffects(aRandomOverlapFactor, aOutCounts);
  st = attributeAffects(aBackstroke, aOutCounts);
  st = attributeAffects(aRepeats, aOutCounts);
  st = attributeAffects(aRepeatOffset, aOutCounts);
  st = attributeAffects(aRepeatMirror, aOutCounts);
  st = attributeAffects(aRepeatOscillate, aOutCounts);
  st = attributeAffects(aSeed, aOutCounts);
  st = attributeAffects(aLift, aOutCounts);
  st = attributeAffects(aActive, aOutCounts);
  st = attributeAffects(aStrokeCountFactor, aOutCounts);
  st = attributeAffects(aOverlap, aOutCounts);
  st = attributeAffects(aPivotFraction, aOutCounts);
  st = attributeAffects(aPlaneMatrix, aOutCounts);
  st = attributeAffects(aStrokeRotationTexture, aOutCounts);
  st = attributeAffects(aStrokeTranslationTexture, aOutCounts);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutCounts);


  st = attributeAffects(aCurve, aOutArcLengths);
  st = attributeAffects(aSubcurve, aOutArcLengths);
  st = attributeAffects(aPointDensity, aOutArcLengths);
  st = attributeAffects(aStrokeLength, aOutArcLengths);
  st = attributeAffects(aRandomLengthFactor, aOutArcLengths);
  st = attributeAffects(aRandomOverlapFactor, aOutArcLengths);
  st = attributeAffects(aBackstroke, aOutArcLengths);
  st = attributeAffects(aRepeats, aOutArcLengths);
  st = attributeAffects(aRepeatOffset, aOutArcLengths);
  st = attributeAffects(aRepeatMirror, aOutArcLengths);
  st = attributeAffects(aRepeatOscillate, aOutArcLengths);
  st = attributeAffects(aSeed, aOutArcLengths);
  st = attributeAffects(aLift, aOutArcLengths);
  st = attributeAffects(aActive, aOutArcLengths);
  st = attributeAffects(aStrokeCountFactor, aOutArcLengths);
  st = attributeAffects(aOverlap, aOutArcLengths);
  st = attributeAffects(aPivotFraction, aOutArcLengths);
  st = attributeAffects(aPlaneMatrix, aOutArcLengths);
  st = attributeAffects(aStrokeRotationTexture, aOutArcLengths);
  st = attributeAffects(aStrokeTranslationTexture, aOutArcLengths);
  st = attributeAffects(aStrokeTranslationSampleDistance, aOutArcLengths);


  return ( MS::kSuccess );

}


MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
                             const MMatrix &data) {

  MDataHandle h = block.outputValue(attribute);
  h.set(data);
  return block.setClean( attribute );
}


MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
                             const MVectorArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnVectorArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
                             const MDoubleArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnDoubleArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}

MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
                             const MMatrixArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnMatrixArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}



MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
                             const MIntArray &data) {
  MDataHandle h = block.outputValue(attribute);
  MFnIntArrayData fn;
  MObject d = fn.create(data);
  h.set(d);
  return block.setClean( attribute );
}


// MStatus strokeCurve::setData(MDataBlock &block, MObject &attribute,
//                              const std::vector<std::unique_ptr<Stroke> > &strokes) {

//   MDataHandle h = block.outputValue(attribute);
//   MFnIntArrayData fn;
//   MObject d = fn.create(data);
//   h.set(d);
//   return block.setClean( attribute );
// }

// MDataHandle hOutput = data.outputValue(aOutput);
// MFnPluginData fnOut;
// MTypeId kdid(strokeCurveData::id);
// MObject dOut = fnOut.create(kdid , &st ); er;
// strokeCurveData *newData = (strokeCurveData *)fnOut.data(&st); er;
// strokeCurveGeom *geom = newData->fGeometry;


// geom->create(strokes, planeNormal, force, brushId, paintId );


// cerr << "data is copied here in preparation for the output " << endl;

// do stuff with geomPtr


// hOutput.set( newData );
// data.setClean( plug );




MStatus strokeCurve::getData( MObject &attribute,  MIntArray &array) {
  MStatus st;
  MFnIntArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;
}


MStatus strokeCurve::getData( MObject &attribute,  MDoubleArray &array) {
  MStatus st;
  MFnDoubleArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

}

MStatus strokeCurve::getData( MObject &attribute,  MVectorArray &array) {
  MStatus st;
  MFnVectorArrayData fn;
  MPlug plug(thisMObject(), attribute);
  MObject d;
  st = plug.getValue(d); ert;
  fn.setObject(d);
  array = fn.array(&st); ert;
  return MS::kSuccess;

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
        plug == aOutTargets ||
        plug == aOutTangents ||
        plug == aOutPositions ||
        plug == aOutCounts ||
        plug == aOutArcLengths ||
        plug == aOutput
      )) { return ( MS::kUnknownParameter ); }

  MMatrixArray outTargets; // outTargets.clear();
  MVectorArray outTangents; // outTargets.clear();
  MVectorArray outPositions;
  MIntArray outCounts; // outCounts.clear();
  MDoubleArray outArcLengths; // outArcLengths.clear();

  MMatrix planeMatrix = data.inputValue(strokeCurve::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();
  double sampleDistance = data.inputValue(aStrokeTranslationSampleDistance).asDouble();

  std::vector<std::unique_ptr<Stroke> > strokes;

  st = generateStrokes(data, strokes) ;

  for (auto &citer : strokes)  // ranged based for loop
  {
    citer->appendTargets(outTargets);
    citer->appendTangents(outTangents);
    citer->appendPoints(outPositions);
    outCounts.append(citer->length());
    outArcLengths.append(citer->arcLength());
  }

  unsigned len = outTargets.length();

  st = setData(data, strokeCurve::aOutTargets, outTargets); er;
  st = setData(data, strokeCurve::aOutTangents, outTangents); er;
  st = setData(data, strokeCurve::aOutPositions, outPositions); er;

  st = setData(data, strokeCurve::aOutCounts, outCounts); er;
  st = setData(data, strokeCurve::aOutArcLengths, outArcLengths); er;

  st = setData(data, strokeCurve::aOutArcLengths, outArcLengths); er;

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


MVector strokeCurve::binormal(const MMatrix &p1, const MMatrix &p2,
                              const MVector &normal) const {
  return (   MVector(p2[3][0] - p1[3][0], p2[3][1] - p1[3][1],
                     p2[3][2] - p1[3][2]) ^ normal).normal();
}


MStatus strokeCurve::getTextureName(const MObject &attribute,
                                    MString &name) const {
  MStatus st;
  MPlugArray plugArray;
  MPlug plug(thisMObject(), attribute);
  bool hasConnection = plug.connectedTo(plugArray, 1, 0, &st); ert;
  if (! hasConnection) { return MS::kUnknownParameter; }
  name = plugArray[0].name(&st);
  return MS::kSuccess;
}



MStatus strokeCurve::sampleUVTexture(const MObject &attribute,  MFloatArray &uVals,
                                     MFloatArray &vVals, MFloatArray &result) const {

  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {   return MS::kFailure; }
  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();
  MFloatVectorArray transparencies;
  MFloatVectorArray colors;

  int n = uVals.length();

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat,
        0, &uVals, &vVals, 0, 0, 0, 0, 0, colors, transparencies); ert;

  result.setLength(n);
  for (int i = 0; i < n; ++i)
  {
    result.set(colors[i].x, i);
  }
  return MS::kSuccess;
}


MStatus strokeCurve::sampleUVGradient(const MObject &attribute, float sampleDistance,
                                      MFloatArray &uVals,
                                      MFloatArray &vVals, MFloatVectorArray &result) const {
  MStatus st;
  MString plugName;
  st = getTextureName(attribute, plugName);
  if (st.error()) {   return MS::kFailure; }

  unsigned n = uVals.length();
  result.setLength(n);

  if (sampleDistance < 0.00001) {
    sampleDistance = 0.00001;
  }
  float sampleRecip = 1.0f / sampleDistance;

  MFloatMatrix cameraMat;
  cameraMat.setToIdentity();

  MFloatVectorArray transparencies;
  MFloatVectorArray colors;


  MFloatArray uOffsetVals(n);
  MFloatArray vOffsetVals(n);

  for (unsigned i = 0; i < n; i++) {
    uOffsetVals.set( (uVals[i] + sampleDistance) , i);
    vOffsetVals.set( (vVals[i] + sampleDistance) , i);
  }


  MFloatVectorArray uOffsetColors;
  MFloatVectorArray vOffsetColors;

  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vVals, 0, 0, 0, 0, 0, colors, transparencies); ert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0,
        &uOffsetVals, &vVals, 0, 0, 0, 0, 0, uOffsetColors, transparencies); ert;
  st =  MRenderUtil::sampleShadingNetwork (plugName, n, false, false, cameraMat, 0, &uVals,
        &vOffsetVals, 0, 0, 0, 0, 0, vOffsetColors, transparencies); ert;

  for (unsigned i = 0; i < n; i++) {
    MFloatVector grad( (uOffsetColors[i].x - colors[i].x) ,
                       (vOffsetColors[i].x - colors[i].x) , 0.0);
    result.set(grad * sampleRecip, i);
  }
  return MS::kSuccess;
}


void strokeCurve::postConstructor()
{
  setExistWithoutInConnections(false);
  setExistWithoutOutConnections(true);
}




