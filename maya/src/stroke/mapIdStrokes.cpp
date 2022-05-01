
#include <maya/MIOStream.h>
#include <math.h>
#include <algorithm>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MMeshIntersector.h>

#include "strokeData.h"
#include "mapIdStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapIdStrokes::aBrushId;
MObject mapIdStrokes::aNumberOfBrushes;
MObject mapIdStrokes::aSampleAt;

MTypeId mapIdStrokes::id(k_mapIdStrokes);

mapIdStrokes::mapIdStrokes() {}

mapIdStrokes::~mapIdStrokes() {}

void *mapIdStrokes::creator()
{
  return new mapIdStrokes();
}

/// Post constructor
void mapIdStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus mapIdStrokes::initialize()
{
  MStatus st;
  MString method("mapIdStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnEnumAttribute eAttr;

  aBrushId = nAttr.create("brushIdMap", "bid", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aBrushId);

  aNumberOfBrushes = nAttr.create("numberOfBrushes", "nb", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aNumberOfBrushes);

  aSampleAt = eAttr.create("sampleAt", "sa", mapIdStrokes::kFirstTarget);
  eAttr.addField("firstTarget", mapIdStrokes::kFirstTarget);
  eAttr.addField("curveStart", mapIdStrokes::kPivot);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  addAttribute(aSampleAt);

  attributeAffects(aBrushId, aOutput);
  attributeAffects(aNumberOfBrushes, aOutput);
  attributeAffects(aSampleAt, aOutput);

  return (MS::kSuccess);
}

MStatus mapIdStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  MIntArray brushIds;
  float dummy = data.inputValue(aBrushId).asFloat();
  int numBrushes = data.inputValue(aNumberOfBrushes).asInt();
  MFloatPointArray points;

  mapIdStrokes::SampleAt sampleAt = (mapIdStrokes::SampleAt)data.inputValue(aSampleAt).asShort();
  if (sampleAt == mapIdStrokes::kFirstTarget)
  {
    getFirstTargetPoints(strokes, points);
  }
  else
  {
    getPivotPoints(strokes, points);
  }

  getBrushIds(data, points, numBrushes, brushIds);
  applyBrushIds(strokes, brushIds);
  return MS::kSuccess;
}

void mapIdStrokes::getBrushIds(
    MDataBlock &data,
    MFloatPointArray &points,
    int numBrushes,
    MIntArray &brushIds) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  unsigned len = points.length();
  bool isMapped = false;

  if (TexUtils::hasTexture(thisObj, mapIdStrokes::aBrushId))
  {
    st = TexUtils::sampleSolidTexture(
        thisObj,
        mapIdStrokes::aBrushId,
        numBrushes,
        points,
        brushIds);

    if (!st.error())
    {
      isMapped = true;
    }
  }
  if (!isMapped)
  {
    float val = data.inputValue(aBrushId).asFloat();
    int clampedVal = fmax(0, fmin( ((val * numBrushes) + 0.5), numBrushes-1) );
    brushIds = MIntArray(len, clampedVal);
  }
}

void mapIdStrokes::applyBrushIds(
    std::vector<Stroke> *strokes,
    const MIntArray &brushIds) const
{
  if (brushIds.length() != strokes->size())
  {
    return;
  }
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;
  for (unsigned i = 0; siter != strokes->end(); siter++, i++)
  {
    siter->setBrushId(brushIds[i]);
  }
}
