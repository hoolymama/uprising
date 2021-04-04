
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

  aBrushRotateOrder = eAttr.create("brushRotateOrder", "bro", strokeCreator::kTwistTiltBank);
  eAttr.addField("twistTiltBank", strokeCreator::kTwistTiltBank);
  eAttr.addField("tiltBankTwist", strokeCreator::kTiltBankTwist);
  eAttr.addField("bankTwistTilt", strokeCreator::kBankTwistTilt);
  eAttr.addField("tiltTwistBank", strokeCreator::kTiltTwistBank);
  eAttr.addField("twistBankTilt", strokeCreator::kTwistBankTilt);
  eAttr.addField("bankTiltTwist", strokeCreator::kBankTiltTwist);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  st = addAttribute(aBrushRotateOrder);

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

  return (MS::kSuccess);
}

MStatus strokeCreator::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes)
{
  return (MS::kUnknownParameter);
}

void strokeCreator::applyRotations(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes) const
{
  

  MDataHandle hTilt = data.inputValue(aBrushTilt);
  float tiltStart = float(hTilt.child(aBrushTiltStart).asAngle().asRadians());
  float tiltEnd = float(hTilt.child(aBrushTiltEnd).asAngle().asRadians());
  
  MDataHandle hBank = data.inputValue(aBrushBank);
  float bankStart = float(hBank.child(aBrushBankStart).asAngle().asRadians());
  float bankEnd = float(hBank.child(aBrushBankEnd).asAngle().asRadians());
  
  MDataHandle hTwist = data.inputValue(aBrushTwist);
  float twistStart = float(hTwist.child(aBrushTwistStart).asAngle().asRadians());
  float twistEnd = float(hTwist.child(aBrushTwistEnd).asAngle().asRadians());
  
  BrushRotateOrder order = BrushRotateOrder(data.inputValue(aBrushRotateOrder).asShort());
  

  switch (order)
  {
  case strokeCreator::kTwistTiltBank:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyTwist(twistAngle);
        curr_target->applyTilt(tiltAngle);
        curr_target->applyBank(bankAngle);
      }
    }
    break;
  case strokeCreator::kTiltBankTwist:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyTilt(tiltAngle);
        curr_target->applyBank(bankAngle);
        curr_target->applyTwist(twistAngle);
      }
    }
    break;
  case strokeCreator::kBankTwistTilt:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyBank(bankAngle);
        curr_target->applyTwist(twistAngle);
        curr_target->applyTilt(tiltAngle);
      }
    }
    break;
  case strokeCreator::kTiltTwistBank:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyTilt(tiltAngle);
        curr_target->applyTwist(twistAngle);
        curr_target->applyBank(bankAngle);
      }
    }
    break;
  case strokeCreator::kTwistBankTilt:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyTwist(twistAngle);
        curr_target->applyBank(bankAngle);
        curr_target->applyTilt(tiltAngle);
      }
    }
    break;
  default: // case strokeCreator::kBankTiltTwist:
    for (std::vector<Stroke>::iterator curr_stroke = pOutStrokes->begin(); curr_stroke != pOutStrokes->end(); curr_stroke++)
    {
      MFloatArray params;
      curr_stroke->calculateParams(params);
      unsigned i = 0;
      Stroke::target_iterator curr_target = curr_stroke->targets_begin();
      for (; curr_target != curr_stroke->targets_end(); curr_target++, i++)
      {
        const float &param = params[i];
        float rparam = 1.0 - param;
        float twistAngle = (twistStart * rparam) + (twistEnd * param);
        float tiltAngle = (tiltStart * rparam) + (tiltEnd * param);
        float bankAngle = (bankStart * rparam) + (bankEnd * param);

        curr_target->applyBank(bankAngle);
        curr_target->applyTilt(tiltAngle);
        curr_target->applyTwist(twistAngle);
      }
    }
    break;
  }
  
}
