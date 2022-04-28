
#include <maya/MIOStream.h>
#include <math.h>
// #include <algorithm>
#include <map>
// #include <utility>
// #include <tuple>
#include <string>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPoint.h>
#include <maya/MFnPluginData.h>
#include <maya/MRenderUtil.h>
#include <maya/MString.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnMessageAttribute.h>

#include <maya/MArrayDataHandle.h>
#include <maya/MAngle.h>

#include <maya/MPlugArray.h>

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

// #include "paintingGeom.h"
// #include "strokeData.h"
// #include "paintingNode.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"

#include "brushData.h"
#include "brushNode.h"
#include "nodeUtils.h"

const double rad_to_deg = (180 / 3.1415927);

const int LEAD_COLOR = 18;
const int ACTIVE_COLOR = 15;
const int ACTIVE_AFFECTED_COLOR = 8;
const int DORMANT_COLOR = 4;
const int HILITE_COLOR = 17;
const int RED_COLOR = 12;

MTypeId brushNode::id(k_brushNode);

brushNode::brushNode()
{
}

brushNode::~brushNode()
{
}

void *brushNode::creator()
{
  return new brushNode();
}

const double epsilon = 0.0001;

MObject brushNode::aPhysicalId;
MObject brushNode::aWidth;
MObject brushNode::aTip;
MObject brushNode::aBristleHeight;
MObject brushNode::aPaintingParam;
MObject brushNode::aDipParam;
MObject brushNode::aWipeParam;
MObject brushNode::aRetention;
MObject brushNode::aShape;
MObject brushNode::aTransHeightParam;
MObject brushNode::aContactPower;

MObject brushNode::aInitialWait;
MObject brushNode::aInitialWater;

MObject brushNode::aInitialDips;
MObject brushNode::aRetardant;

MObject brushNode::aForwardBias;
MObject brushNode::aGravityBias;
MObject brushNode::aWipeBarPosition;

MObject brushNode::aLineLength;
MObject brushNode::aLineThickness;
MObject brushNode::aDmx;



MObject brushNode::aOutPaintBrush;
MObject brushNode::aOutDipBrush;
MObject brushNode::aOutWipeBrush;

// MObject brushNode::aCustomId;

MObject brushNode::aModel;

MStatus brushNode::initialize()
{
  MStatus st;
  MString method("brushNode::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnUnitAttribute uAttr;
  MFnCompoundAttribute cAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  MFnMessageAttribute msgAttr;

  MFloatMatrix identity;
  identity.setToIdentity();

  aPhysicalId = nAttr.create("physicalId", "pid", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  addAttribute(aPhysicalId);

  aTip = nAttr.create("tip", "tip", MFnNumericData::k3Float);
  nAttr.setStorable(true);
  nAttr.setHidden(false);
  nAttr.setWritable(true);
  nAttr.setKeyable(true);
  st = addAttribute(aTip);
  mser;

  aBristleHeight = nAttr.create("bristleHeight", "bht", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aBristleHeight);

  aPaintingParam = nAttr.create("paintingParam", "ppm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aPaintingParam);

  aDipParam = nAttr.create("dipParam", "dpm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aDipParam);

  aWipeParam = nAttr.create("wipeParam", "wpm", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aWipeParam);

  aWidth = nAttr.create("width", "wid", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aWidth);

  aRetention = nAttr.create("retention", "ret", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);

  nAttr.setKeyable(true);
  addAttribute(aRetention);

  aShape = eAttr.create("shape", "shp", Brush::kRound);
  eAttr.addField("flat", Brush::kFlat);
  eAttr.addField("round", Brush::kRound);
  eAttr.setKeyable(true);
  eAttr.setHidden(false);
  st = addAttribute(aShape);
  mser;

  aTransHeightParam = nAttr.create("transHeightParam", "thp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aTransHeightParam);

  aContactPower = nAttr.create("contactPower", "ctp", MFnNumericData::kFloat);
  nAttr.setMin(0.00f);
  nAttr.setDefault(1.0);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aContactPower);

  aInitialWait = nAttr.create("initialWait", "inwi", MFnNumericData::kInt);
  nAttr.setMin(-1);
  nAttr.setDefault(0);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aInitialWait);

  aInitialWater = nAttr.create("initialWater", "inwa", MFnNumericData::kBoolean);
  nAttr.setDefault(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aInitialWater);

  aInitialDips = nAttr.create("initialDips", "indp", MFnNumericData::kInt);
  nAttr.setMin(-1);
  nAttr.setDefault(0);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aInitialDips);

  
  aDmx = nAttr.create("dmxId", "dmx", MFnNumericData::kInt);
  nAttr.setMin(0);
  nAttr.setMax(5);
  nAttr.setDefault(0);
  nAttr.setReadable(true);
  nAttr.setStorable(true);
  nAttr.setKeyable(true);
  addAttribute(aDmx);


  aRetardant = nAttr.create("retardant", "rtd", MFnNumericData::kBoolean);
  nAttr.setDefault(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aRetardant);

  aLineLength = nAttr.create("lineLength", "lln", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aLineLength);

  aLineThickness = nAttr.create("lineThickness", "ltk", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  addAttribute(aLineThickness);

  aOutPaintBrush = tAttr.create("outPaintBrush", "opb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutPaintBrush);

  aOutDipBrush = tAttr.create("outDipBrush", "odb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutDipBrush);

  aOutWipeBrush = tAttr.create("outWipeBrush", "owb", brushData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutWipeBrush);

  aForwardBias = nAttr.create("forwardBias", "fbs", MFnNumericData::k2Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aForwardBias);

  aGravityBias = nAttr.create("gravityBias", "gbs", MFnNumericData::k2Float);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aGravityBias);

  aWipeBarPosition = nAttr.create("wipeBarPosition", "wbp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWipeBarPosition);

  aModel = tAttr.create("model", "mdl", MFnData::kString);
  tAttr.setStorable(true);
  tAttr.setKeyable(true);
  addAttribute(aModel);

  ////////////

  attributeAffects(aPhysicalId, aOutPaintBrush);
  attributeAffects(aWidth, aOutPaintBrush);
  attributeAffects(aTip, aOutPaintBrush);
  attributeAffects(aBristleHeight, aOutPaintBrush);
  attributeAffects(aPaintingParam, aOutPaintBrush);
  attributeAffects(aDipParam, aOutPaintBrush);
  attributeAffects(aWipeParam, aOutPaintBrush);
  attributeAffects(aRetention, aOutPaintBrush);
  attributeAffects(aShape, aOutPaintBrush);
  attributeAffects(aTransHeightParam, aOutPaintBrush);
  attributeAffects(aContactPower, aOutPaintBrush);
  attributeAffects(aForwardBias, aOutPaintBrush);
  attributeAffects(aGravityBias, aOutPaintBrush);



  attributeAffects(aPhysicalId, aOutDipBrush);
  attributeAffects(aWidth, aOutDipBrush);
  attributeAffects(aTip, aOutDipBrush);
  attributeAffects(aBristleHeight, aOutDipBrush);
  attributeAffects(aPaintingParam, aOutDipBrush);
  attributeAffects(aDipParam, aOutDipBrush);
  attributeAffects(aWipeParam, aOutDipBrush);
  attributeAffects(aRetention, aOutDipBrush);
  attributeAffects(aShape, aOutDipBrush);
  attributeAffects(aTransHeightParam, aOutDipBrush);
  attributeAffects(aContactPower, aOutDipBrush);

  

  attributeAffects(aPhysicalId, aOutWipeBrush);
  attributeAffects(aWidth, aOutWipeBrush);
  attributeAffects(aTip, aOutWipeBrush);
  attributeAffects(aBristleHeight, aOutWipeBrush);
  attributeAffects(aPaintingParam, aOutWipeBrush);
  attributeAffects(aDipParam, aOutWipeBrush);
  attributeAffects(aWipeParam, aOutWipeBrush);
  attributeAffects(aRetention, aOutWipeBrush);
  attributeAffects(aShape, aOutWipeBrush);
  attributeAffects(aTransHeightParam, aOutWipeBrush);
  attributeAffects(aContactPower, aOutWipeBrush);

  return (MS::kSuccess);
}

MStatus brushNode::outputData(MDataBlock &data, MObject &attribute, const Brush &brush)
{
  MStatus st;
  MDataHandle hOutput = data.outputValue(attribute);
  MFnPluginData fnOut;
  MTypeId kdid(brushData::id);

  MObject dOut = fnOut.create(kdid, &st);
  mser;
  brushData *newData = (brushData *)fnOut.data(&st);
  mser;

  *(newData->fGeometry) = brush;

  hOutput.set(newData);
  hOutput.setClean();
  return MS::kSuccess;
}

MStatus brushNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;

  if (!(
          (plug == aOutPaintBrush) ||
          (plug == aOutDipBrush) ||
          (plug == aOutWipeBrush)))
  {
    return (MS::kUnknownParameter);
  }

  MFloatVector tip = data.inputValue(aTip).asFloatVector();

  int physicalId = data.inputValue(aPhysicalId).asInt();

  float width = data.inputValue(aWidth).asFloat();
  float bristleHeight = data.inputValue(aBristleHeight).asFloat();

  float paintingParam = data.inputValue(aPaintingParam).asFloat();
  float dipParam = data.inputValue(aDipParam).asFloat();
  float wipeParam = data.inputValue(aWipeParam).asFloat();

  float retention = data.inputValue(aRetention).asFloat();
  Brush::Shape shape = Brush::Shape(data.inputValue(aShape).asShort());
  float transHeightParam = data.inputValue(aTransHeightParam).asFloat();
  float contactPower = data.inputValue(aContactPower).asFloat();
  const float2 &forwardBias = data.inputValue(aForwardBias).asFloat2();
  const float2 &gravityBias = data.inputValue(aGravityBias).asFloat2();

  float forwardBias0 = forwardBias[0];
  float forwardBias1 = forwardBias[1];

  float gravityBias0 = gravityBias[0];
  float gravityBias1 = gravityBias[1];

  MFloatMatrix fmat =  MFloatMatrix(NodeUtils::firstWorldMatrix(thisMObject()).matrix);

  Brush paintingBrush(fmat,
                      physicalId,
                      tip,
                      bristleHeight,
                      paintingParam,
                      width,
                      shape,
                      retention,
                      transHeightParam,
                      contactPower,
                      forwardBias0,
                      forwardBias1,
                      gravityBias0,
                      gravityBias1);

  Brush dipBrush(fmat, physicalId,
                 tip,
                 bristleHeight,
                 dipParam,
                 width,
                 shape,
                 99999999.0,
                 transHeightParam,
                 contactPower,
                 0.0, 0.0,
                 0.0, 0.0);

  Brush wipeBrush(fmat, physicalId,
                  tip,
                  bristleHeight,
                  wipeParam,
                  width,
                  shape,
                  99999999.0,
                  transHeightParam,
                  contactPower,
                  0.0, 0.0,
                  0.0, 0.0);

  outputData(data, aOutPaintBrush, paintingBrush);
  outputData(data, aOutDipBrush, dipBrush);
  outputData(data, aOutWipeBrush, wipeBrush);

  return MS::kSuccess;
}

MStatus brushNode::getBrush(MObject &attribute, Brush &brush)
{
  MStatus st;
  MObject thisObj = thisMObject();
  MPlug plug(thisObj, attribute);
  MObject d;
  st = plug.getValue(d);
  MFnPluginData fnP(d);
  brushData *bdata = (brushData *)fnP.data();
  if (!bdata)
  {
    return MS::kFailure;
  }
  brush = *(bdata->fGeometry);
  return MS::kSuccess;
}

void brushNode::draw(M3dView &view,
                     const MDagPath &path,
                     M3dView::DisplayStyle style,
                     M3dView::DisplayStatus status)
{
}

bool brushNode::isBounded() const
{
  return false;
}

MBoundingBox brushNode::boundingBox() const
{
  return MBoundingBox();
}

void brushNode::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());
  nodeFn.setName("brushShape#");
  setExistWithoutInConnections(true);
  setExistWithoutOutConnections(true);
}
