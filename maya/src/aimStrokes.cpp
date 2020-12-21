
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
#include "aimStrokes.h"
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

MObject aimStrokes::aStrokes;
MObject aimStrokes::aPoint;
MObject aimStrokes::aOutput;

MTypeId aimStrokes::id(k_aimStrokes);

aimStrokes::aimStrokes() {}

aimStrokes::~aimStrokes() {}

void *aimStrokes::creator()
{
  return new aimStrokes();
}

/// Post constructor
void aimStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus aimStrokes::initialize()
{
  MStatus st;
  MString method("aimStrokes::initialize");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aStrokes);

 


  aPoint = nAttr.createPoint("point", "pt");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPoint);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aPoint, aOutput);

  return (MS::kSuccess);
}

MStatus aimStrokes::compute(const MPlug &plug, MDataBlock &data)
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

  aim(data, geom);

  hOutput.set(newData);
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus aimStrokes::copyStrokes(MDataBlock &data, std::vector<Stroke> *geom)
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

void aimStrokes::aim(
    MDataBlock &data,
    std::vector<Stroke> *geom) const
{

  MStatus st;
  float3 & fpoint = data.inputValue(aPoint).asFloat3();
  MPoint point(fpoint[0], fpoint[1], fpoint[2]);

  std::vector<Stroke>::iterator iter = geom->begin();

  for (unsigned i = 0; iter != geom->end(); iter++)
  {
    iter->applyGlobalAim(point);
  }
}
