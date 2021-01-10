
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "tiltStrokes.h"
#include "stroke.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
// #include "mayaMath.h"
#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

MObject tiltStrokes::aProjection;
MObject tiltStrokes::aGlobalTiltTexture;
MObject tiltStrokes::aGlobalTiltSampleDistance;
MObject tiltStrokes::aGlobalTiltAmount;


MTypeId tiltStrokes::id(k_tiltStrokes);

tiltStrokes::tiltStrokes() {}

tiltStrokes::~tiltStrokes() {}

void *tiltStrokes::creator()
{
  return new tiltStrokes();
}

/// Post constructor
void tiltStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus tiltStrokes::initialize()
{
  MStatus st;
  MString method("tiltStrokes::initialize");
	inheritAttributesFrom("strokeMutator");


  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  MMatrix identity;
  identity.setToIdentity();
  aProjection = mAttr.create("projection", "prj", MFnMatrixAttribute::kDouble);
  mAttr.setStorable(false);
  mAttr.setHidden(true);
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aProjection);

  aGlobalTiltTexture = nAttr.createColor("globalTiltTexture", "gtx");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aGlobalTiltTexture);

  aGlobalTiltSampleDistance = nAttr.create("globalTiltSampleDistance",
                                           "gtsd", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(0.01);
  addAttribute(aGlobalTiltSampleDistance);

  aGlobalTiltAmount = nAttr.create("aGlobalTiltAmount",
                                   "gta", MFnNumericData::kDouble);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aGlobalTiltAmount);

  st = attributeAffects(aProjection, aOutput);
  st = attributeAffects(aGlobalTiltTexture, aOutput);
  st = attributeAffects(aGlobalTiltSampleDistance, aOutput);
  st = attributeAffects(aGlobalTiltAmount, aOutput);

  return (MS::kSuccess);
}

MStatus tiltStrokes::mutate(MDataBlock &data, std::vector<Stroke> *geom) const 
{
  MFloatArray uVals;
  MFloatArray vVals;
  uVals.clear();
  vVals.clear();
  appendTargetUVsTo(uVals, vVals, geom);



  tilt(uVals, vVals, data, geom);
  return MS::kSuccess;
}



void tiltStrokes::assignTargetUVs(MDataBlock &data, std::vector<Stroke> *geom)
{
  MMatrix inverseProjMat = data.inputValue(
                                   tiltStrokes::aProjection)
                               .asMatrix()
                               .inverse();
  std::vector<Stroke>::iterator iter;
  iter = geom->begin();
  for (iter = geom->begin(); iter != geom->end(); iter++)
  {
    iter->assignTargetUVs(inverseProjMat);
  }
}

int tiltStrokes::countTargets(const std::vector<Stroke> *geom) const
{
  int result = 0;
  std::vector<Stroke>::const_iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++)
  {
    result += iter->size(true);
  }
  return result;
}

void tiltStrokes::appendTargetUVsTo(
    MFloatArray &uVals,
    MFloatArray &vVals,
    const std::vector<Stroke> *geom) const
{
  std::vector<Stroke>::const_iterator iter = geom->begin();
  for (unsigned i = 0; iter != geom->end(); iter++)
  {
    iter->appendTargetUVsTo(uVals, vVals);
  }
}

bool tiltStrokes::tilt(
    MFloatArray &uVals,
    MFloatArray &vVals,
    MDataBlock &data,
    std::vector<Stroke> *geom) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  if (!TexUtils::hasTexture(thisObj, tiltStrokes::aGlobalTiltTexture))
  {
    return false;
  }

  float sampleDist = data.inputValue(aGlobalTiltSampleDistance).asDouble();
  float amount = data.inputValue(aGlobalTiltAmount).asDouble();

  if (sampleDist == 0.0 || amount == 0.0)
  {
    return false;
  }

  MFloatVectorArray gradients;
  st = TexUtils::sampleUVGradient(
      thisObj,
      tiltStrokes::aGlobalTiltTexture,
      sampleDist,
      amount,
      uVals,
      vVals,
      gradients,
      sampleDist);
  if (st.error())
  {
    return false;
  }

  std::vector<Stroke>::iterator iter = geom->begin();
  int index = 0;
  for (unsigned i = 0; iter != geom->end(); iter++)
  {
    index = iter->applyGlobalTilt(gradients, index);
  }
  return true;
}
