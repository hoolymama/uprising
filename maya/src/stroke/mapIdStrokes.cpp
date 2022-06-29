
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


MObject mapIdStrokes::aNormalize;  
MObject mapIdStrokes::aNumberOfValues; /// normalize
MObject mapIdStrokes::aIdProperty;
MObject mapIdStrokes::aIdMap;         ///> The solid texture whose red channel will be mapped to the chosen Id property of strokes.
MObject mapIdStrokes::aSampleAt;
MObject mapIdStrokes::aStrokeParam;
MObject mapIdStrokes::aIdOffset;

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

  aNormalize = nAttr.create("setRange", "srn", MFnNumericData::kBoolean);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  addAttribute(aNormalize);

  aNumberOfValues = nAttr.create("numberOfValues", "nvl", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aNumberOfValues);

  aIdProperty = eAttr.create("idProperty", "idp", mapIdStrokes::kBrushId);
  eAttr.addField("brushId", mapIdStrokes::kBrushId);
  eAttr.addField("paintId", mapIdStrokes::kPaintId);
  eAttr.addField("layerId", mapIdStrokes::kLayerId);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  addAttribute(aIdProperty);

  aIdMap = nAttr.create("idMap", "ids", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aIdMap);

  aIdOffset = nAttr.create("idOffset", "ido", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aIdOffset);


  aSampleAt = eAttr.create("sampleAt", "sa", mapIdStrokes::kStrokeParam);
  eAttr.addField("strokeParam", mapIdStrokes::kStrokeParam);
  eAttr.addField("pivot", mapIdStrokes::kPivot);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  addAttribute(aSampleAt);

  aStrokeParam = nAttr.create("strokeParam", "skp", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aStrokeParam);

  attributeAffects(aNormalize, aOutput);
  attributeAffects(aNumberOfValues, aOutput);
  attributeAffects(aIdProperty, aOutput);
  attributeAffects(aIdOffset, aOutput);
  attributeAffects(aIdMap, aOutput);
  attributeAffects(aSampleAt, aOutput);
  attributeAffects(aStrokeParam, aOutput);
 
  return (MS::kSuccess);
}

MStatus mapIdStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  MIntArray ids;
  int idOffset = data.inputValue(aIdOffset).asInt();

  float dummy = data.inputValue(aIdMap).asFloat();
  int numValues = data.inputValue(aNumberOfValues).asInt();
  float strokeParam = data.inputValue(aStrokeParam).asFloat();
  bool normalize = data.inputValue(aNormalize).asBool();
  MObject thisObj = thisMObject();

  MFloatPointArray points;

  mapIdStrokes::SampleAt sampleAt = (mapIdStrokes::SampleAt)data.inputValue(aSampleAt).asShort();
  mapIdStrokes::Property idProperty = (mapIdStrokes::Property)data.inputValue(aIdProperty).asShort();

  if (sampleAt == mapIdStrokes::kPivot)
  {
    getPivotPoints(strokes, points);
  }
  else
  {
    getStrokeParamPoints(strokes, strokeParam, points );
  }

  int quantizeLevels = normalize ? numValues : 256; 
 
  // GET IDS
  TexUtils::sampleSolidTexture(
        thisObj,
        mapIdStrokes::aIdMap,
        quantizeLevels,
        points,
        ids);

  for (unsigned int i = 0; i < ids.length(); i++)
  {
    ids[i] += idOffset;
  }

  applyIds(strokes, ids, idProperty);
  return MS::kSuccess;
}


void mapIdStrokes::applyIds(
    std::vector<Stroke> *strokes,
    const MIntArray &ids,
    mapIdStrokes::Property idProperty) const
{
  if (ids.length() != strokes->size())
  {
    return;
  }
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;

  if (idProperty == mapIdStrokes::kBrushId)
  {
    for (; siter != strokes->end(); ++siter, ++index)
    {
       siter->setBrushId(ids[index]);
    }
    return;
  }

  if (idProperty == mapIdStrokes::kPaintId)
  {
    for (; siter != strokes->end(); ++siter, ++index)
    {
      siter->setPaintId(ids[index]);
    }
    return;
  }
  if (idProperty == mapIdStrokes::kLayerId)
  {
    for (; siter != strokes->end(); ++siter, ++index)
    {
      siter->setLayerId(ids[index]);
    }
    return;
  }
}
