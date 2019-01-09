
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
// const double deg_to_rad = (3.1415927 / 180);






MObject strokeNode::aBrushId;
MObject strokeNode::aPaintId;
MObject strokeNode::aLayerId;
MObject strokeNode::aPointDensity;
MObject strokeNode::aStrokeLength;
MObject strokeNode::aRandomLengthFactor;
MObject strokeNode::aStrokeDirection;
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


MObject strokeNode::aBrushTiltRamp;
MObject strokeNode::aBrushTiltRangeMin;
MObject strokeNode::aBrushTiltRangeMax;
MObject strokeNode::aBrushTiltRange;

MObject strokeNode::aBrushBankRamp;
MObject strokeNode::aBrushBankRangeMin;
MObject strokeNode::aBrushBankRangeMax;
MObject strokeNode::aBrushBankRange;

MObject strokeNode::aBrushTwistRamp;
MObject strokeNode::aBrushTwistRangeMin;
MObject strokeNode::aBrushTwistRangeMax;
MObject strokeNode::aBrushTwistRange;




MObject strokeNode::aBrushFollowStroke;


MObject strokeNode::aPlaneNormal;

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
  // MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;




  aPlaneNormal = nAttr.create( "planeNormal", "pnml",  MFnNumericData::k3Double );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault( 0.0, 0.0, 1.0 );
  st = addAttribute(aPlaneNormal); mser;

  aStrokeCountFactor = nAttr.create( "strokeCountFactor",
                                     "stcf", MFnNumericData::kDouble);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setMin(0.00);
  nAttr.setMax(1.0);
  nAttr.setDefault(1.0);
  st = addAttribute(aStrokeCountFactor); mser;


  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kDouble);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  nAttr.setStorable(true);
  nAttr.setDefault( 1.0 );
  st = addAttribute(aPointDensity); mser;

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



  // aStrokeDirection = nAttr.create( "reverseDirection", "revd", MFnNumericData::kBoolean);
  // nAttr.setHidden(false);
  // nAttr.setStorable(true);
  // nAttr.setReadable(true);
  // nAttr.setKeyable(true);
  // nAttr.setDefault(false);
  // st = addAttribute(aStrokeDirection); mser;


  aStrokeDirection = eAttr.create("reverseDirection", "revd", Stroke::kForwards);
  eAttr.addField("forwards", Stroke::kForwards);
  eAttr.addField("backwards", Stroke::kBackwards);
  eAttr.addField("startUppermost", Stroke::kStartUppermost);
  eAttr.addField("endUppermost", Stroke::kEndUppermost);

  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aStrokeDirection);
  mser;


  aRepeats = nAttr.create("repeats", "rpts", MFnNumericData::kShort);
  nAttr.setHidden( false );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aRepeats); mser;

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
  st = addAttribute(aSeed); mser;


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




  aBrushId = nAttr.create("brushId", "brid", MFnNumericData::kShort); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aBrushId); mser;

  aPaintId = nAttr.create("paintId", "ptid", MFnNumericData::kShort); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aPaintId); mser;

  aLayerId = nAttr.create("layerId", "lyid", MFnNumericData::kInt); mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aLayerId); mser;


  aActive = nAttr.create( "active", "act", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aActive); mser;


  aBrushFollowStroke = nAttr.create( "followStroke", "fst", MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(true);
  st = addAttribute(aBrushFollowStroke); mser;



  aLiftLength = nAttr.create( "liftLength", "llg", MFnNumericData::kDouble, 2.0 );
  aLiftHeight = nAttr.create( "liftHeight", "lht", MFnNumericData::kDouble, 2.0 );
  aLiftBias = nAttr.create( "liftBias", "lbi", MFnNumericData::kDouble, 0.0);
  aLift = nAttr.create( "lift", "lft", aLiftLength, aLiftHeight,
                        aLiftBias );
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aLift); mser;


  aStrokeProfileRamp = MRampAttribute::createCurveRamp("strokeProfileRamp", "spr");
  st = addAttribute( aStrokeProfileRamp ); mser;



  aStrokeProfileScaleMin = nAttr.create( "strokeProfileScaleMin", "spscn",
                                         MFnNumericData::kDouble, -1.0 );
  aStrokeProfileScaleMax = nAttr.create( "strokeProfileScaleMax", "spscx",
                                         MFnNumericData::kDouble, 1.0 );
  aStrokeProfileScale  = nAttr.create("strokeProfileScale", "spsc",
                                      aStrokeProfileScaleMin, aStrokeProfileScaleMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aStrokeProfileScale); mser;

  aBrushTiltRamp  = MRampAttribute::createCurveRamp("brushTiltRamp", "brtl");
  st = addAttribute( aBrushTiltRamp ); mser;
  aBrushBankRamp  = MRampAttribute::createCurveRamp("brushBankRamp", "brbk");
  st = addAttribute( aBrushBankRamp ); mser;
  aBrushTwistRamp  = MRampAttribute::createCurveRamp("brushTwistRamp", "brtw");
  st = addAttribute( aBrushTwistRamp ); mser;


  aBrushTiltRangeMin = uAttr.create( "brushTiltRangeMin", "btlrn",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushTiltRangeMax = uAttr.create( "brushTiltRangeMax", "btlrx",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushTiltRange = nAttr.create("brushTiltRange", "btlr", aBrushTiltRangeMin,
                                 aBrushTiltRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushTiltRange); mser;



  aBrushBankRangeMin = uAttr.create( "brushBankRangeMin", "bbnrn",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-90, MAngle::kDegrees));
  aBrushBankRangeMax = uAttr.create( "brushBankRangeMax", "bbnrx",
                                     MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(90, MAngle::kDegrees));
  aBrushBankRange = nAttr.create("brushBankRange", "bbnr", aBrushBankRangeMin,
                                 aBrushBankRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushBankRange); mser;



  aBrushTwistRangeMin = uAttr.create( "brushTwistRangeMin", "btwrn",
                                      MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(-180, MAngle::kDegrees));
  aBrushTwistRangeMax = uAttr.create( "brushTwistRangeMax", "btwrx",
                                      MFnUnitAttribute::kAngle );
  uAttr.setDefault(MAngle(180, MAngle::kDegrees));
  aBrushTwistRange = nAttr.create("brushTwistRange", "btwr", aBrushTwistRangeMin,
                                  aBrushTwistRangeMax);
  nAttr.setHidden( false );
  nAttr.setKeyable( true );
  st = addAttribute(aBrushTwistRange); mser;


  aOutput = tAttr.create("output", "out", strokeGeometryData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);


  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aStrokeLength, aOutput);
  st = attributeAffects(aRandomLengthFactor, aOutput);

  st = attributeAffects(aStrokeDirection, aOutput);
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

  st = attributeAffects(aBrushTiltRamp, aOutput);
  st = attributeAffects(aBrushBankRamp, aOutput);
  st = attributeAffects(aBrushTwistRamp, aOutput);

  st = attributeAffects(aBrushTiltRange, aOutput);
  st = attributeAffects(aBrushBankRange, aOutput);
  st = attributeAffects(aBrushTwistRange, aOutput);

  st = attributeAffects(aBrushFollowStroke, aOutput);
  st = attributeAffects(aPlaneNormal, aOutput);


  st = attributeAffects(aBrushId, aOutput);
  st = attributeAffects(aPaintId, aOutput);
  st = attributeAffects(aLayerId, aOutput);


  return ( MS::kSuccess );

}


///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////


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
  strokeGeometryData *newData = (strokeGeometryData * )fnOut.data(&st); mser;

  std::vector < strokeGeom > *geom = newData->fGeometry;
  st = generateStrokeGeometry(data, geom); mser;
  hOutput.set(newData);
  data.setClean(plug);

  return MS:: kSuccess;
}






// void strokeNode::postConstructor()
// {
//   // setExistWithoutInConnections(false);
//   setExistWithoutOutConnections(true);
// }




