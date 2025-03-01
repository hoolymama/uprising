
#include <maya/MIOStream.h>
#include <vector>

#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>

#include "strokeData.h"
#include "strokeCreator.h"
#include "stroke.h"

#include <jMayaIds.h>
#include "errorMacros.h"

MObject strokeCreator::aCoats;

MObject strokeCreator::aLayerId;
MObject strokeCreator::aPointDensity;
MObject strokeCreator::aBrushTiltStart;
MObject strokeCreator::aBrushTiltEnd;
MObject strokeCreator::aBrushTilt;
MObject strokeCreator::aBrushTwistStart;
MObject strokeCreator::aBrushTwistEnd;
MObject strokeCreator::aBrushTwist;
MObject strokeCreator::aBrushBankStart;
MObject strokeCreator::aBrushBankEnd;
MObject strokeCreator::aBrushBank;
MObject strokeCreator::aBrushRotateOrder;

MObject strokeCreator::aLinearSpeed;           // cm/sec
MObject strokeCreator::aAngularSpeed;          // per sec
MObject strokeCreator::aApproximationDistance; // cm

MTypeId strokeCreator::id(k_strokeCreator);

strokeCreator::strokeCreator() {}

strokeCreator::~strokeCreator() {}

void *strokeCreator::creator()
{
  return new strokeCreator();
}

void strokeCreator::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus strokeCreator::initialize()
{
  MStatus st;
  MString method("strokeCreator::initialize");

  inheritAttributesFrom("strokeNodeBase");

  MFnNumericAttribute nAttr;
  MFnUnitAttribute uAttr;
  MFnEnumAttribute eAttr;

  aLinearSpeed = nAttr.create("linearSpeed", "lnsp", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(200.0f);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);

  aAngularSpeed = uAttr.create("angularSpeed", "agsp", MFnUnitAttribute::kAngle);
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  uAttr.setKeyable(true);
  addAttribute(aAngularSpeed);

  aApproximationDistance = nAttr.create("approximationDistance", "apxd",
                                        MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.0f);
  nAttr.setSoftMax(20.0f);
  nAttr.setDefault(5.0f);
  nAttr.setKeyable(true);
  addAttribute(aApproximationDistance);

  aCoats = nAttr.create("coats", "cts", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1);
  st = addAttribute(aCoats);
  mser;

  aLayerId = nAttr.create("layerId", "lid", MFnNumericData::kInt);
  mser;
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  st = addAttribute(aLayerId);
  mser;

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(1.0f);
  st = addAttribute(aPointDensity);
  mser;

  aBrushTwistStart = uAttr.create("brushTwistStart", "btws", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushTwistEnd = uAttr.create("brushTwistEnd", "btwe", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushTwist = nAttr.create("brushTwist", "btw", aBrushTwistStart, aBrushTwistEnd);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushTwist);
  mser;

  aBrushTiltStart = uAttr.create("brushTiltStart", "btlrs", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushTiltEnd = uAttr.create("brushTiltEnd", "btlre", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushTilt = nAttr.create("brushTilt", "btlr", aBrushTiltStart, aBrushTiltEnd);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushTilt);
  mser;

  aBrushBankStart = uAttr.create("brushBankStart", "bbkrs", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushBankEnd = uAttr.create("brushBankEnd", "bbkre", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  aBrushBank = nAttr.create("brushBank", "bbkr", aBrushBankStart, aBrushBankEnd);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  st = addAttribute(aBrushBank);
  mser;

  aBrushRotateOrder = eAttr.create("brushRotateOrder", "bro", PaintingEnums::kTwistTiltBank);
  eAttr.addField("twistTiltBank", PaintingEnums::kTwistTiltBank);
  eAttr.addField("tiltBankTwist", PaintingEnums::kTiltBankTwist);
  eAttr.addField("bankTwistTilt", PaintingEnums::kBankTwistTilt);
  eAttr.addField("tiltTwistBank", PaintingEnums::kTiltTwistBank);
  eAttr.addField("twistBankTilt", PaintingEnums::kTwistBankTilt);
  eAttr.addField("bankTiltTwist", PaintingEnums::kBankTiltTwist);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);

  st = addAttribute(aBrushRotateOrder);
  st = attributeAffects(aCoats, aOutput);
  st = attributeAffects(aLayerId, aOutput);
  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aBrushRotateOrder, aOutput);
  st = attributeAffects(aBrushTiltStart, aOutput);
  st = attributeAffects(aBrushTiltEnd, aOutput);
  st = attributeAffects(aBrushTilt, aOutput);
  st = attributeAffects(aBrushTwistStart, aOutput);
  st = attributeAffects(aBrushTwistEnd, aOutput);
  st = attributeAffects(aBrushTwist, aOutput);
  st = attributeAffects(aBrushBankStart, aOutput);
  st = attributeAffects(aBrushBankEnd, aOutput);
  st = attributeAffects(aBrushBank, aOutput);
  st = attributeAffects(aLinearSpeed, aOutput);
  st = attributeAffects(aAngularSpeed, aOutput);
  st = attributeAffects(aApproximationDistance, aOutput);

  return (MS::kSuccess);
}

MStatus strokeCreator::generateStrokeGeometry(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{
  MFnDependencyNode depFn(this->thisMObject());
  MString name(depFn.name());

  applyCoats(data, pOutStrokes);
  applySpeeds(data, pOutStrokes);
  applyLayerId(data, pOutStrokes);
  applyCreator(name, pOutStrokes);

  strokeNodeBase::generateStrokeGeometry(plug, data, pOutStrokes);

  return MS::kSuccess;
}

void strokeCreator::applySpeeds(
    MDataBlock &data,
    std::vector<Stroke> *geom) const
{
  float linSpeed = data.inputValue(aLinearSpeed).asFloat();
  float approxDist = data.inputValue(aApproximationDistance).asFloat();
  float angSpeed = float(data.inputValue(aAngularSpeed).asAngle().asRadians());
  std::vector<Stroke>::iterator iter = geom->begin();
  for (; iter != geom->end(); iter++)
  {
    iter->setLinearSpeed(linSpeed);
    iter->setAngularSpeed(angSpeed);
    iter->setApproximationDistance(approxDist);
  }
}

void strokeCreator::applyLayerId(
    MDataBlock &data,
    std::vector<Stroke> *geom) const
{
  int layerId = data.inputValue(aLayerId).asInt();
  std::vector<Stroke>::iterator iter = geom->begin();
  for (; iter != geom->end(); iter++)
  {
    iter->setLayerId(layerId);
  }
}

void strokeCreator::applyCreator(
    const MString &name,
    std::vector<Stroke> *geom) const
{
  std::vector<Stroke>::iterator iter = geom->begin();
  int cid = 0;
  for (; iter != geom->end(); iter++, cid++)
  {
    iter->setCreator(name, cid);
  }
}

void strokeCreator::applyCoats(
    MDataBlock &data,
    std::vector<Stroke> *geom) const
{
  int coats = data.inputValue(aCoats).asInt();

  if (coats < 2)
  {
    return;
  }

  std::vector<Stroke> sourceStrokes(*geom);

  for (unsigned repeatId = 1; repeatId < coats; repeatId++)
  {
    std::vector<Stroke>::iterator iter = sourceStrokes.begin();
    for (; iter != sourceStrokes.end(); iter++)
    {
      iter->setRepeatId(repeatId);
      geom->push_back(*iter);
    }
  }
}

void strokeCreator::subsample(
    const MFloatPointArray &inPoints,
    float density,
    MFloatPointArray &outPoints) const
{
  int lastIndex = inPoints.length() - 1;
  if (density < 0.00001 || lastIndex < 1)
  {
    outPoints = inPoints;
    return;
  }

  float gap = 1.0 / density;

  for (size_t i = 0; i < lastIndex; i++)
  {
    const MFloatPoint &start = inPoints[i];
    const MFloatPoint &end = inPoints[i + 1];

    float dist = start.distanceTo(end);
    int numGaps = int(ceilf(dist / gap));

    for (size_t i = 0; i < numGaps; i++)
    {

      float param = i / float(numGaps);
      outPoints.append((start * (1.0 - param)) + (end * param));
    }
  }
  outPoints.append(inPoints[lastIndex]);
}



void strokeCreator::subsample(
    const MPointArray &inPoints,
    float density,
    MFloatPointArray &outPoints) const
{
  MFloatPointArray floatPoints;
  floatPoints.setLength(inPoints.length());
  for (unsigned i = 0; i < inPoints.length(); i++)
  {
    floatPoints[i] = inPoints[i];
  }
  subsample(floatPoints, density, outPoints);
}

/// @brief Given a set of points and colors, insert new points in between successive each pair of points
/// @param inPoints The input points
/// @param inColors The input colors
/// @param density How many points per centimeter
/// @param outPoints Reference to the output points
/// @param outColors Reference to the output colors
void strokeCreator::subsample(
    const MFloatPointArray &inPoints,
    const MColorArray &inColors,
    float density,
    MFloatPointArray &outPoints,
    MColorArray &outColors) const
{
  int lastIndex = inPoints.length() - 1;
  if (density < 0.00001 || lastIndex < 1)
  {
    outPoints = inPoints;
    outColors = inColors;
    return;
  }

  float gap = 1.0 / density;

  for (size_t i = 0; i < lastIndex; i++)
  {
    const MFloatPoint &start = inPoints[i];
    const MFloatPoint &end = inPoints[i + 1];

    const MColor &startColor = inColors[i];
    const MColor &endColor = inColors[i + 1];

    float dist = start.distanceTo(end);
    int numGaps = int(ceilf(dist / gap));

    for (size_t i = 0; i < numGaps; i++)
    {
      float param = i / float(numGaps);
      outPoints.append((start * (1.0 - param)) + (end * param));
      outColors.append((startColor * (1.0 - param)) + (endColor * param));
    }
  }
  outPoints.append(inPoints[lastIndex]);
  outColors.append(inColors[lastIndex]);
}