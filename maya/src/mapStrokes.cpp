
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MString.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "mapStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapStrokes::aStrokes;
MObject mapStrokes::aProjection;

MObject mapStrokes::aLinearSpeedTexture;
MObject mapStrokes::aAngularSpeedTexture;

MObject mapStrokes::aOutput;

MTypeId mapStrokes::id(k_mapStrokes);

mapStrokes::mapStrokes() {}

mapStrokes::~mapStrokes() {}

void *mapStrokes::creator()
{
  return new mapStrokes();
}

/// Post constructor
void mapStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

// const double epsilon = 0.0001;

MStatus mapStrokes::initialize()
{
  MStatus st;
  MString method("mapStrokes::initialize");

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

  aLinearSpeedTexture = nAttr.createColor("linearSpeedTexture", "lst");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aLinearSpeedTexture);

  aAngularSpeedTexture = nAttr.createColor("angularSpeedTexture", "ast");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aAngularSpeedTexture);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aProjection, aOutput);

  st = attributeAffects(aLinearSpeedTexture, aOutput);
  st = attributeAffects(aAngularSpeedTexture, aOutput);

  return (MS::kSuccess);
}

MStatus mapStrokes::compute(const MPlug &plug, MDataBlock &data)
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

  MFloatArray uVals;
  MFloatArray vVals;
  uVals.clear();
  vVals.clear();
  calcUVs(data, geom, uVals, vVals);

  speedMap(data, geom, uVals, vVals);

  hOutput.set(newData);
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus mapStrokes::copyStrokes(MDataBlock &data, std::vector<Stroke> *geom)
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

void mapStrokes::calcUVs(MDataBlock &data, std::vector<Stroke> *geom, MFloatArray &u, MFloatArray &v) const
{
  MMatrix inverseProjMat = data.inputValue(
                                   mapStrokes::aProjection)
                               .asMatrix()
                               .inverse();
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    MPoint p = ((iter->pivot().position() * inverseProjMat) * 0.5) + MVector(0.5, 0.5, 0.0);
    u.append(p.x);
    v.append(p.y);
  }
}

bool mapStrokes::speedMap(
    MDataBlock &data,
    std::vector<Stroke> *geom,
    MFloatArray &uVals,
    MFloatArray &vVals) const
{

  MStatus st;
  MObject thisObj = thisMObject();
  bool result = false;
  if (TexUtils::hasTexture(thisObj, mapStrokes::aLinearSpeedTexture))
  {
    MFloatArray vals;

    st = TexUtils::sampleUVTexture(
        thisObj,
        mapStrokes::aLinearSpeedTexture,
        uVals,
        vVals,
        0.0f, 1.0f,
        vals);
    if (!st.error())
    {
      std::vector<Stroke>::iterator iter = geom->begin();
      for (unsigned i = 0; iter != geom->end(); iter++, i++)
      {
        iter->setLinearSpeed(vals[i]);
      }
      result = true;
    }
  }

  if (TexUtils::hasTexture(thisObj, mapStrokes::aAngularSpeedTexture))
  {

    MFloatArray vals;

    st = TexUtils::sampleUVTexture(
        thisObj,
        mapStrokes::aAngularSpeedTexture,
        uVals,
        vVals, 0.0f, 1.0f,
        vals);
    if (!st.error())
    {

      std::vector<Stroke>::iterator iter = geom->begin();
      for (unsigned i = 0; iter != geom->end(); iter++, i++)
      {
        iter->setAngularSpeed(vals[i]);
      }
      result = true;
    }
  }

  return result;
}
