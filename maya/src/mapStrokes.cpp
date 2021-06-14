
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "mapStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

// MObject mapStrokes::aStrokes;
MObject mapStrokes::aProjection;

MObject mapStrokes::aLinearSpeedTexture;
MObject mapStrokes::aAngularSpeedTexture;

// MObject mapStrokes::aOutput;

MTypeId mapStrokes::id(k_mapStrokes);

mapStrokes::mapStrokes() {}

mapStrokes::~mapStrokes() {}

void *mapStrokes::creator()
{
  return new mapStrokes();
}

void mapStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus mapStrokes::initialize()
{
  MStatus st;
  MString method("mapStrokes::initialize");

	inheritAttributesFrom("strokeMutator");



  MFnNumericAttribute nAttr;
  
  MFnMatrixAttribute mAttr;
 

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

 
  st = attributeAffects(aProjection, aOutput);

  st = attributeAffects(aLinearSpeedTexture, aOutput);
  st = attributeAffects(aAngularSpeedTexture, aOutput);

  return (MS::kSuccess);
}



MStatus mapStrokes::mutate(MDataBlock &data, std::vector<Stroke> *geom) const 
{
  MFloatArray uVals;
  MFloatArray vVals;
  uVals.clear();
  vVals.clear();
  calcUVs(data, geom, uVals, vVals);

  speedMap(data, geom, uVals, vVals);
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
