
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
  // nAttr.setMin(0.00f);
  nAttr.setSoftMax(200.0f);
  // nAttr.setDefault(100.00f);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeed);

  // const double minAngSpeed = mayaMath::single_pi / 900.00;
  // const double defaultAngSpeed = mayaMath::single_pi / 9.00;
  aAngularSpeed = uAttr.create("angularSpeed", "agsp", MFnUnitAttribute::kAngle);
  uAttr.setStorable(true);
  uAttr.setReadable(true);
  // uAttr.setMin(minAngSpeed);
  uAttr.setKeyable(true);
  // uAttr.setMax((mayaMath::single_pi));
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

  applyCoats(data, pOutStrokes);
  applySpeeds(data, pOutStrokes);
  strokeNodeBase::generateStrokeGeometry(plug,data,pOutStrokes);

  return MS::kSuccess;
}

void strokeCreator::applySpeeds(
  MDataBlock &data,
  std::vector<Stroke> *geom) const
{
  float linSpeed =  data.inputValue(aLinearSpeed).asFloat();
  float approxDist =  data.inputValue(aApproximationDistance).asFloat();
  float angSpeed =  float(data.inputValue(aAngularSpeed).asAngle().asRadians());
  std::vector<Stroke>::iterator iter = geom->begin();
  for (;iter != geom->end() ; iter++)
  {
    iter->setLinearSpeed(linSpeed);
    iter->setAngularSpeed(angSpeed);
    iter->setApproximationDistance(approxDist);
  }
}


void strokeCreator::applyCoats(
  MDataBlock &data,
  std::vector<Stroke> *geom) const
{
  int coats =  data.inputValue(aCoats).asInt();

  if (coats < 2) {
    return;
  }

  std::vector<Stroke> sourceStrokes(*geom);

  for (unsigned repeatId = 1; repeatId < coats; repeatId++)
  {
    std::vector<Stroke>::iterator iter = sourceStrokes.begin();
    for (;iter != sourceStrokes.end() ; iter++)
    {
      iter->setRepeatId(repeatId);
      geom->push_back(*iter);
    }
  }
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
