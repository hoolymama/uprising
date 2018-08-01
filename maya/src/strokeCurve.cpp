
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>
#include <map>


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
#include "strokeCurve.h"
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
  nAttr.setSoftMax(20.0);
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
  nAttr.setDefault( 0.1 );
  st = addAttribute(aPointDensity); er;

  aStrokeLength = nAttr.create( "strokeLength", "stl", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(20.0);
  nAttr.setDefault(5.0);
  st = addAttribute(aStrokeLength); er;

  aRandomLengthFactor = nAttr.create( "randomLengthFactor", "rlfc",
                                      MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.2);
  st = addAttribute(aRandomLengthFactor); er;


  aOverlap  = nAttr.create("overlap", "ovlp", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.1 );
  st = addAttribute(aOverlap); er;

  aRandomOverlapFactor = nAttr.create( "randomOverlapFactor", "rofc",
                                       MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(0.2);
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
  nAttr.setDefault(0);
  st = addAttribute(aRepeats); er;

  aRepeatOffset = nAttr.create("repeatOffset", "rpof", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setDefault( 0.0 );
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
  nAttr.setDefault(true);
  st = addAttribute(aForceDip); er;


  aSubcurveMin = nAttr.create( "subcurveMin", "scvn", MFnNumericData::kDouble );
  aSubcurveMax = nAttr.create( "subcurveMax", "scvx", MFnNumericData::kDouble );
  aSubcurve = nAttr.create( "subcurve", "scv", aSubcurveMin, aSubcurveMax);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aSubcurve); er;






  aApproachDistanceStart = nAttr.create( "approachDistanceStart",
                                         "apds", MFnNumericData::kDouble);
  aApproachDistanceMid = nAttr.create( "approachDistanceMid",
                                       "apdm", MFnNumericData::kDouble);
  aApproachDistanceEnd = nAttr.create( "approachDistanceEnd",
                                       "apde", MFnNumericData::kDouble);
  aApproachDistance = nAttr.create( "approachDistance",
                                    "apd", aApproachDistanceStart, aApproachDistanceMid, aApproachDistanceEnd);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  st = addAttribute(aApproachDistance); er;

  aLiftLength = nAttr.create( "liftLength", "llg", MFnNumericData::kDouble );
  aLiftHeight = nAttr.create( "liftHeight", "lht", MFnNumericData::kDouble );
  aLiftBias = nAttr.create( "liftBias", "lbi", MFnNumericData::kDouble);
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
  const MObject &curve,
  double strokeLength,
  double randomLengthFactor,
  double overlap,
  double randomOverlapFactor,
  double subcurveMin,
  double subcurveMax,
  double countFactor,
  MVectorArray &result
) const  {

  /*
    generate an array of start and end param for strokes
  */

  MStatus st = MS::kSuccess;

  MFnNurbsCurve curveFn(curve, &st);
  if (st.error()) { return 0; }
  double curveLen = curveFn.length(epsilon);

  clampSubCurveValues(curveLen, subcurveMin, subcurveMax);

  cerr << "subcurveMin: " << subcurveMin <<  " -  subcurveMax: " << subcurveMax << endl;
  cerr << "strokeLength: " << strokeLength << endl;

  randomLengthFactor = clamp(randomLengthFactor);
  randomOverlapFactor = clamp(randomOverlapFactor);

  double startDist = subcurveMin;
  double endDist;

  do {
    endDist = calculateEndDist(startDist, strokeLength, randomLengthFactor, subcurveMax);
    cerr << "START -> END: "  << startDist << ", " << endDist << endl;
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

// MStatus strokeCurve::getBrushes(MDataBlock &data,
//                                 std::map<short, Brush> &brushes ) const {

//   /*
//   Generate a std::map of Brushes
//   */
//   MStatus st;
//   MArrayDataHandle hBrushes = data.inputArrayValue(aBrushes, &st ); ert;
//   unsigned nPlugs = hBrushes.elementCount();
//   for (unsigned i = 0; i < nPlugs; i++, hBrushes.next()) {
//     short index = short(hBrushes.elementIndex(&st));
//     if (st.error()) {
//       continue;
//     }
//     MDataHandle hComp = hBrushes.inputValue(&st );

//     double width =  hComp.child(strokeCurve::aBrushWidth).asDouble() ;
//     double retention = hComp.child(strokeCurve::aBrushRetention).asDouble() ;


//     MDataHandle hLift = hComp.child(aBrushLift);
//     double liftLength = hLift.child( aBrushLiftLength).asDouble();
//     double liftHeight = hLift.child( aBrushLiftHeight).asDouble();
//     double liftBias = hLift.child( aBrushLiftBias).asDouble();

//     mayaMath::axis frontAxis, upAxis;

//     brushes[index] = Brush(index, width, retention, liftLength , liftHeight , liftBias);
//   }
//   return MS::kSuccess;
// }

// MStatus strokeCurve::getPaints(MDataBlock &data,
//                                std::map<short, Paint> &paints ) const {
//   MStatus st;
//   MArrayDataHandle hPaints = data.inputArrayValue(aPaints, &st ); ert;
//   unsigned nPlugs = hPaints.elementCount();

//   for (unsigned i = 0; i < nPlugs; i++, hPaints.next()) {

//     short index = short(hPaints.elementIndex(&st));
//     if (st.error()) {
//       continue;
//     }

//     MDataHandle hComp = hPaints.inputValue(&st );

//     MFloatVector vc =  hComp.child(strokeCurve::aPaintColor).asFloatVector() ;
//     MColor color(vc.x, vc.y, vc.z);

//     double opacity = hComp.child(strokeCurve::aPaintOpacity).asDouble() ;
//     MString name = hComp.child(strokeCurve::aPaintName).asString();

//     paints[index] = Paint(index, color, opacity, name);

//   }
//   return MS::kSuccess;
// }


MStatus strokeCurve::generateStrokes(MDataBlock &data,
                                     std::vector<std::unique_ptr<Stroke> > &strokes ) const {

  MStatus st;

  if (! data.inputValue(aActive).asBool()) {
    return MS::kSuccess;
  }

  MDataHandle hCurve = data.inputValue(aCurve, &st ); ert;
  double countFactor = data.inputValue(aStrokeCountFactor).asDouble();
  MMatrix planeMatrix = data.inputValue(strokeCurve::aPlaneMatrix).asMatrix();
  MMatrix inversePlaneMatrix = planeMatrix.inverse();
  MVector planeNormal = (MVector::zAxis * planeMatrix).normal();
  int seed = data.inputValue(aSeed).asInt();
  srand48(seed);

  MObject  dCurve =  data.inputValue(aCurve).asNurbsCurveTransformed();
  MFnNurbsCurve curveFn(dCurve, &st) ; ert;
  double curveLength = curveFn.length(epsilon);
  double pointDensity =  data.inputValue(aPointDensity).asDouble() ;
  if (pointDensity < 0.001) { pointDensity = 0.001; }

  double strokeLength = data.inputValue( aStrokeLength).asDouble();
  double randomLengthFactor = data.inputValue( aRandomLengthFactor).asDouble();
  double randomOverlapFactor = data.inputValue( aRandomOverlapFactor).asDouble();
  double overlap  =  data.inputValue( aOverlap).asDouble();

  MDataHandle hLift = data.inputValue(aLift);
  double liftLength = hLift.child( aLiftLength).asDouble();
  double liftHeight = hLift.child( aLiftHeight).asDouble();
  double liftBias = hLift.child( aLiftBias).asDouble();
  MVector lift(liftLength, liftHeight, liftBias);
  MDataHandle hSubcurve = data.inputValue(aSubcurve);
  double subcurveMin = hSubcurve.child( aSubcurveMin).asDouble();
  double subcurveMax = hSubcurve.child( aSubcurveMax).asDouble();

  // MDataHandle hRotate = hComp.child(aBrush);

  // double tilt = hRotate.child( aBrushTilt).asAngle().value();
  // double bank = hRotate.child( aBrushBank).asAngle().value();
  // double twist = hRotate.child( aBrushTwist).asAngle().value();
  // MVector brush(tilt, bank, twist);

  MDataHandle hApproachDistance = data.inputValue(aApproachDistance);
  double apStart = hApproachDistance.child( aApproachDistanceStart).asDouble();
  double apMid = hApproachDistance.child( aApproachDistanceMid).asDouble();
  double apEnd = hApproachDistance.child( aApproachDistanceEnd).asDouble();
  MVector approachDistance(apStart, apMid, apEnd);

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

  short brushId =  data.inputValue( aBrushId).asShort();
  short paintId =  data.inputValue( aPaintId).asShort();

  // double rotation = hComp.child(aStrokeRotation).asAngle().value();
  // double translation = hComp.child(aStrokeTranslation).asDouble();
  double pivotFraction = data.inputValue(aPivotFraction).asDouble();

  bool follow =  data.inputValue( aBrushFollowStroke).asBool();
  // bool forceDip =  hComp.child( aForceDip).asBool();

  Stroke::Scope brushRampScope =
    Stroke::Scope( data.inputValue(aBrushRampScope).asShort());


  MVectorArray boundaries;
  unsigned count =  getStrokeBoundaries(
                      dCurve,
                      strokeLength,
                      randomLengthFactor,
                      overlap,
                      randomOverlapFactor,
                      subcurveMin,
                      subcurveMax,
                      countFactor,
                      boundaries);

  // cerr << "Count : " << count << endl;

  if (! count) {
    return MS::kSuccess;
  }

  // JPMDBG;
  // std::vector<Stroke> curveStrokes;


  // bool first = true;

  cerr << "Stroke Count: " << count << endl;
  // if (count > 20) {
  //   count = 20;
  // }
  for (int i = 0; i < count; ++i) {
    const double &startDist = boundaries[i].x;
    const double &endDist = boundaries[i].y;

    // // cerr << "stroke : " << i << endl;
    // std::unique_ptr<Stroke>  motherStroke = std::make_unique<ForwardStroke>();
    // // ForwardStroke motherStroke = ForwardStroke();

    // motherStroke->initializeTargets(
    //   dCurve,
    //   curveLength,
    //   startDist,
    //   endDist,
    //   pointDensity,
    //   liftLength,
    //   liftBias
    // );

    if (i % 2 == 0) {
      strokes.push_back( std::make_unique<Stroke>() );
    }
    else {
      strokes.push_back( std::make_unique<BackStroke>() );
    }

    strokes.back()->initializeTargets(
      dCurve,
      curveLength,
      startDist,
      endDist,
      pointDensity,
      liftLength,
      liftBias
    );

    strokes.back()->setHeights(
      thisMObject(),
      strokeCurve::aStrokeProfileRamp,
      strokeProfileScaleMin,
      strokeProfileScaleMax,
      liftHeight
    );

    strokes.back()->setRotations(
      thisMObject(),
      strokeCurve::aBrushTiltRamp,
      strokeCurve::aBrushBankRamp,
      strokeCurve::aBrushTwistRamp,
      brushRampScope,
      follow
    );



    // Stroke motherStroke = Stroke(
    //                         dCurve,
    //                         thisMObject(),
    //                         boundaries[i],
    //                         planeNormal,
    //                         lift,
    //                         aBrushTiltRamp,
    //                         aBrushBankRamp,
    //                         aBrushTwistRamp,
    //                         brushRampScope,
    //                         aStrokeProfileRamp,
    //                         curveLength,
    //                         pointDensity,
    //                         pivotFraction,
    //                         follow,
    //                         backstroke
    //                       );

    // cerr << "Stroke targets length: " << motherStroke.length() << endl;
    // if (motherStroke.overlapsPlane(inversePlaneMatrix)) {
    // strokes.push_back(motherStroke);
    // }
  }
  JPMDBG;

  // brush,
  // follow,
  // backstroke
  //     if (motherStroke.overlapsPlane(inversePlaneMatrix)) {
  //       curveStrokes.push_back(motherStroke);
  //     }

  //     for (int j = 0; j < repeats; ++j)
  //     {
  //       bool reverse = repeatOscillate &&  ((j % 2) == 0);

  //       double offset = repeatOffset * (j + 1);
  //       Stroke offsetStroke = Stroke(motherStroke, offset, repeatAdvance, reverse, planeNormal);
  //       if (offsetStroke.overlapsPlane(inversePlaneMatrix)) {
  //         curveStrokes.push_back(offsetStroke);
  //       }
  //       if (repeatMirror) {
  //         // cerr << "mirror: " << repeatMirror << endl;
  //         Stroke mirrorOffsetStroke = Stroke(motherStroke, -offset, repeatAdvance, reverse,
  //                                            planeNormal);
  //         if (mirrorOffsetStroke.overlapsPlane(inversePlaneMatrix)) {
  //           curveStrokes.push_back(mirrorOffsetStroke);
  //         }
  //       }
  //     }
  //   }
  // }

  // std::vector<Stroke>::iterator  iter = curveStrokes.begin();
  // for (; iter != curveStrokes.end(); iter++) {
  //   double strokeApproachStart = apMid, strokeApproachEnd = apMid;
  //   if (iter == curveStrokes.begin()) {
  //     strokeApproachStart = apStart;
  //   }
  //   if (std::next(iter) == curveStrokes.end()) {
  //     strokeApproachEnd = apEnd;
  //   }
  //   iter->setApproach(strokeApproachStart, strokeApproachEnd);
  // }


  // strokes.insert(strokes.end(), curveStrokes.begin(), curveStrokes.end());

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
        plug == aOutArcLengths
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

  // std::vector<ForwardStroke>  strokes;
  st = generateStrokes(data, strokes) ;
  // JPMDBG;
  // std::vector<std::unique_ptr<Stroke> >::const_iterator citer;
  // std::vector<ForwardStroke>::const_iterator citer;
  // citer = strokes.begin();
  JPMDBG;
  // for (citer = strokes.begin() ; citer != strokes.end(); citer++) {
  for (auto &citer : strokes)  // ranged based for loop
  {
    citer->appendTargets(planeNormal, outTargets);
    citer->appendTangents(outTangents);
    citer->appendPoints(outPositions);
    outCounts.append(citer->length());
    outArcLengths.append(citer->arcLength());
  }
  JPMDBG;

  // std::vector<Stroke>::const_iterator citer;
  // std::vector<Stroke>::iterator iter;
  // // // JPMDBG;

  // unsigned nStrokes = strokes.size();
  // MFloatArray uVals(nStrokes);
  // MFloatArray vVals(nStrokes);
  // // // JPMDBG;

  // citer = strokes.begin();
  // for (unsigned i = 0; citer != strokes.end(); citer++, i++) {
  //   float &u = uVals[i];
  //   float &v = vVals[i];
  //   citer->getPivotUVs(inversePlaneMatrix, u, v);
  // }
  // MFloatArray rotations;
  // MFloatVectorArray translations;
  // // // JPMDBG;


  // st = sampleUVTexture(strokeCurve::aStrokeRotationTexture, uVals, vVals, rotations);
  // if (! st.error()) {
  //   iter = strokes.begin();
  //   for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
  //     iter->rotate(rotations[i], planeNormal);
  //   }
  // }
  // // // JPMDBG;

  // st = sampleUVGradient(strokeCurve::aStrokeTranslationTexture, sampleDistance, uVals,
  //                       vVals,
  //                       translations);
  // if (! st.error()) {
  //   iter = strokes.begin();
  //   for (unsigned i = 0; iter != strokes.end(); iter++, i++) {
  //     iter->translate(translations[i], planeNormal);
  //   }
  // }


  // citer = strokes.begin();
  // for (; citer != strokes.end(); citer++) {
  //   const MMatrixArray &targets = citer->targets();
  //   const MVectorArray &tangents = citer->tangents();
  //   const unsigned len = targets.length();

  //   outCounts.append(len);
  //   outBrushIds.append(citer->brush().id);
  //   outPaintIds.append(citer->paint().id);
  //   outArcLengths.append(citer->arcLength());
  //   outBrushWidths.append(citer->brush().width);
  //   const MColor &c = citer->paint().color;
  //   outPaintColors.append(MVector(c.r, c.g, c.b));
  //   outPaintOpacities.append(citer->paint().opacity);
  //   outForceDips.append(citer->forceDip());
  //   outCurveIds.append(citer->curveId());
  //   outApproachStarts.append(citer->approachStart());
  //   outApproachEnds.append(citer->approachEnd());

  //   for (int i = 0; i < len; ++i)
  //   {
  //     outTargets.append(targets[i]);
  //     outTangents.append(tangents[i]);
  //     // outNormals.append(nrm[i]);
  //   }
  // }


  unsigned len = outTargets.length();


  st = setData(data, strokeCurve::aOutTargets, outTargets); er;
  st = setData(data, strokeCurve::aOutTangents, outTangents); er;
  st = setData(data, strokeCurve::aOutPositions, outPositions); er;

  st = setData(data, strokeCurve::aOutCounts, outCounts); er;
  st = setData(data, strokeCurve::aOutArcLengths, outArcLengths); er;

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
  // MFnDependencyNode nodeFn(thisMObject());
  // nodeFn.setName("strokeCurveShape#");
  // setExistWithoutInConnections(true);
  // setExistWithoutOutConnections(true);
}



