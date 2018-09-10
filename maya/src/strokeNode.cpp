
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

#include "strokeGeometryData.h"
#include "strokeNode.h"
#include "stroke.h"
#include "backstroke.h"


#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

const double rad_to_deg = (180 / 3.1415927);



MObject strokeNode::aBrushId;
MObject strokeNode::aPaintId;

MObject strokeNode::aPointDensity;
MObject strokeNode::aStrokeLength;
MObject strokeNode::aRandomLengthFactor;
MObject strokeNode::aBackstroke;
MObject strokeNode::aRepeats;
MObject strokeNode::aRepeatOffset;
MObject strokeNode::aRepeatMirror;
MObject strokeNode::aRepeatOscillate;

MObject strokeNode::aSeed;

MObject strokeNode::aLiftLength;
MObject strokeNode::aLiftBias;
MObject strokeNode::aLiftHeight;
MObject strokeNode::aLift;

MObject strokeNode::aStrokeProfileRamp;
MObject strokeNode::aStrokeProfileScaleMin;
MObject strokeNode::aStrokeProfileScaleMax;
MObject strokeNode::aStrokeProfileScale;


MObject strokeNode::aActive;
MObject strokeNode::aStrokeCountFactor;

MObject strokeNode::aPivotFraction;
MObject strokeNode::aRepeatPivot;

MObject strokeNode::aBrushRampScope;
MObject strokeNode::aBrushTiltRamp;
MObject strokeNode::aBrushBankRamp;
MObject strokeNode::aBrushTwistRamp;

MObject strokeNode::aBrushFollowStroke;


MObject strokeNode::aApproachDistanceStart;
MObject strokeNode::aApproachDistanceMid;
MObject strokeNode::aApproachDistanceEnd;
MObject strokeNode::aApproachDistance;

MObject strokeNode::aPlaneMatrix;

MObject strokeNode::aOutput;


MTypeId strokeNode::id( k_strokeNode );

strokeNode::strokeNode() {}

strokeNode::~strokeNode() {}

void *strokeNode::creator() {
  return new strokeNode();
}

const double epsilon = 0.0001;


MStatus strokeNode::initialize()
{
  MStatus st;
  MString method("strokeNode::initialize");

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




  aOutput = tAttr.create("output", "out", strokeGeometryData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  // tAttr.setCached(false);
  addAttribute(aOutput);


  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aStrokeLength, aOutput);
  st = attributeAffects(aRandomLengthFactor, aOutput);

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
  st = attributeAffects(aPivotFraction, aOutput);
  st = attributeAffects(aRepeatPivot, aOutput);

  st = attributeAffects(aBrushRampScope, aOutput);
  st = attributeAffects(aBrushTiltRamp, aOutput);
  st = attributeAffects(aBrushBankRamp, aOutput);
  st = attributeAffects(aBrushTwistRamp, aOutput);
  st = attributeAffects(aBrushFollowStroke, aOutput);
  st = attributeAffects(aPlaneMatrix, aOutput);

  st = attributeAffects(aApproachDistance, aOutput);
  st = attributeAffects(aBrushId, aOutput);
  st = attributeAffects(aPaintId, aOutput);




  return ( MS::kSuccess );

}

MStatus strokeNode::generateStrokeGeometry(MDataBlock &data,
    std::vector<strokeGeom> *geom) const
{
  return MS::kSuccess;
}

MStatus strokeNode::compute(const MPlug &plug, MDataBlock &data )
{
  MStatus st;
  if (plug != aOutput) {
    return (MS::kUnknownParameter );
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeGeometryData:: id);

  MObject dOut = fnOut.create(kdid, & st);
  strokeGeometryData *newData = (strokeGeometryData * )fnOut.data(&st); er;
  std::vector < strokeGeom > *geom = newData->fGeometry;
  st = generateStrokeGeometry(data, geom); er;
  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}

// void strokeNode::postConstructor()
// {
//   // setExistWithoutInConnections(false);
//   setExistWithoutOutConnections(true);
// }




