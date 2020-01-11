
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

#include <maya/MFnGenericAttribute.h>
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

#include "strokeData.h"
#include "tiltStrokes.h"
#include "stroke.h"
#include "cImgUtils.h"

#include "filterDefinition.h"
#include "sortDefinition.h"

#include <jMayaIds.h>
#include "mayaMath.h"
#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

MObject tiltStrokes::aStrokes;
MObject tiltStrokes::aProjection;
MObject tiltStrokes::aGlobalTiltTexture;
MObject tiltStrokes::aGlobalTiltSampleDistance;
MObject tiltStrokes::aGlobalTiltAmount;

MObject tiltStrokes::aOutput;

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

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);

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

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aProjection, aOutput);
  st = attributeAffects(aGlobalTiltTexture, aOutput);
  st = attributeAffects(aGlobalTiltSampleDistance, aOutput);
  st = attributeAffects(aGlobalTiltAmount, aOutput);

  return (MS::kSuccess);
}

MStatus tiltStrokes::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData::id);

  MObject dOut = fnOut.create(kdid, &st);
  strokeData *newData = (strokeData *)fnOut.data(&st);
  mser;
  std::vector<Stroke> *geom = newData->fGeometry;
  geom->clear();

  copyStrokes(data, geom);

  assignTargetUVs(data, geom);

  // int count = countTargets(geom);

  MFloatArray uVals;
  MFloatArray vVals;
  uVals.clear();
  vVals.clear();
  appendTargetUVsTo(uVals, vVals, geom);

  // cerr << "target count/uvCount " << count << " / " << uVals.length() << endl;

  tilt(uVals, vVals, data, geom);

  hOutput.set(newData);
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus tiltStrokes::copyStrokes(MDataBlock &data, std::vector<Stroke> *geom)
{
  MStatus st;
  MDataHandle hStrokes = data.inputValue(aStrokes, &st);
  msert;

  MObject dStrokes = hStrokes.data();
  MFnPluginData fnStrokes(dStrokes, &st);
  msert;

  strokeData *sData = (strokeData *)fnStrokes.data();
  const std::vector<Stroke> *strokeGeom = sData->fGeometry;

  std::vector<Stroke>::const_iterator citer;
  for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
  {
    geom->push_back(*citer);
  }

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
