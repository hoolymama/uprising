
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
#include "paletteData.h"
#include "brushShopData.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

MObject mapIdStrokes::aSampleParam;
MObject mapIdStrokes::aDoPaintId;
MObject mapIdStrokes::aPaintIdMap;
MObject mapIdStrokes::aPaintIdMapQuantizeLevel; // 256 is normal
MObject mapIdStrokes::aPaintIdOffset;
MObject mapIdStrokes::aPalette;
MObject mapIdStrokes::aDoBrushModelId;
MObject mapIdStrokes::aBrushShop;

MObject mapIdStrokes::aDoWidthBandLayerIds;
MObject mapIdStrokes::aWidthBandLevel;

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

  aSampleParam = nAttr.create("sampleParam", "smp", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aSampleParam);

  aDoPaintId = nAttr.create("doPaintId", "ptid", MFnNumericData::kBoolean);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  addAttribute(aDoPaintId);

  aPaintIdMap = nAttr.create("paintIdMap", "pmap", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPaintIdMap);

  aPaintIdMapQuantizeLevel = nAttr.create("paintIdMapQuantizeLevel", "pql", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aPaintIdMapQuantizeLevel);

  aPaintIdOffset = nAttr.create("paintIdOffset", "pof", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aPaintIdOffset);

  aPalette = tAttr.create("palette", "plt", paletteData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aPalette);

  aDoBrushModelId = nAttr.create("doBrushModelId", "bmid", MFnNumericData::kBoolean);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(true);
  addAttribute(aDoBrushModelId);

  aBrushShop = tAttr.create("brushShop", "shop", brushShopData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aBrushShop);


  aDoWidthBandLayerIds = nAttr.create("doWidthBandLayerIds", "wbid", MFnNumericData::kBoolean);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(false);
  addAttribute(aDoWidthBandLayerIds);
 
;
  aWidthBandLevel = nAttr.create("widthBandLevel", "wblv", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setArray(true);
  addAttribute(aWidthBandLevel);

  attributeAffects(aSampleParam, aOutput);
  attributeAffects(aDoPaintId, aOutput);
  attributeAffects(aPaintIdMap, aOutput);
  attributeAffects(aPaintIdMapQuantizeLevel, aOutput);
  attributeAffects(aPaintIdOffset, aOutput);
  attributeAffects(aPalette, aOutput);
  attributeAffects(aDoBrushModelId, aOutput);
  attributeAffects(aBrushShop, aOutput);
  
  attributeAffects(aDoWidthBandLayerIds, aOutput);
  attributeAffects(aWidthBandLevel, aOutput);
  



  return (MS::kSuccess);
}

MStatus mapIdStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  // FOR PAINTS
  bool doPaintId = data.inputValue(aDoPaintId).asBool();
  bool doBrushModelId = data.inputValue(aDoBrushModelId).asBool();
  bool doWidthBandLayerIds = data.inputValue(aDoWidthBandLayerIds).asBool();

  if (doPaintId)
  {
    st = assignPaintIds(data, strokes);
  }
  if (doBrushModelId)
  {
    st = assignBrushModelIds(data, strokes);
  }
  if (doWidthBandLayerIds)
  {
    st = assignWidthBandLayerIds(data, strokes);
  }
  return MS::kSuccess;
}

MStatus mapIdStrokes::assignWidthBandLayerIds(MDataBlock &data, std::vector<Stroke> *strokes) const
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aWidthBandLevel);
  int numInputs = ha.elementCount();
  std::vector<float> levels;
  for (unsigned i=0;  i < numInputs; i++, ha.next())
  {
    levels.push_back(ha.inputValue().asFloat());
  }
  std::sort(levels.begin(), levels.end(), std::greater<float>());

  std::vector<Stroke>::iterator iter = strokes->begin();
  for (; iter != strokes->end(); iter++)
  {
    int layerId = levels.size(); // possibly 0 if no levels
    float strokeWidth = iter->maxRadius() *2.0f;
    std::vector<float>::const_iterator levelIter = levels.begin();
    for (; levelIter != levels.end(); levelIter++)
    {
      if (strokeWidth < *levelIter)
      {
        layerId --;
      } else {
        break;
      }
    }
    iter->setLayerId(layerId);
  }
  return MS::kSuccess;
}


MStatus mapIdStrokes::assignPaintIds(MDataBlock &data,
                                           std::vector<Stroke> *strokes) const
{
  MStatus st;
  int numStrokes = strokes->size();
  float sampleParam = data.inputValue(aSampleParam).asFloat();
  float paintIdMap = data.inputValue(aPaintIdMap).asFloat();
  int quantizeLevels = data.inputValue(aPaintIdMapQuantizeLevel).asInt();
  int paintIdOffset = data.inputValue(aPaintIdOffset).asInt();

  msert;

  MIntArray paintIds;

  MObject thisObj = thisMObject();
  MFloatPointArray points;
  getStrokeParamPoints(strokes, sampleParam, points);

  // GET IDS FROM TEXTURE
  TexUtils::sampleSolidTexture(
      thisObj,
      mapIdStrokes::aPaintIdMap,
      quantizeLevels,
      points,
      paintIds);

  MIntArray potIds(numStrokes, 0);
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;

  for (; siter != strokes->end(); ++siter, ++index)
  {
    int &paintId = paintIds[index];

    paintId += paintIdOffset;

    siter->setPaintId(paintId);

  }
  return MS::kSuccess;
}

MStatus mapIdStrokes::assignBrushModelIds(MDataBlock &data,
                                          std::vector<Stroke> *strokes) const
{
  MStatus st;
  BrushShop brushShop;
  st = getBrushShop(data, brushShop); msert;
  std::vector<Stroke>::iterator stroke = strokes->begin();
  for (; stroke != strokes->end(); stroke++)
  {
    float width = stroke->maxRadius() * 2.0f;
    Brush::Shape shape = stroke->brushStrokeSpec().shape;
    int modelId = brushShop.findModelId(width, shape, &st) ;mser;
    stroke->setBrushModelId(modelId);
  }
  return MS::kSuccess;
}

MStatus mapIdStrokes::getBrushShop(
    MDataBlock &data, BrushShop &brushShop) const
{
  MStatus st;
  MDataHandle h = data.inputValue(aBrushShop, &st);
  msert;
  MObject d = h.data();
  MFnPluginData fnP(d, &st);
  if (st.error())
  {
    return st;
  }
  brushShopData *bData = (brushShopData *)fnP.data(&st);
  msert;
  brushShop = *(bData->fGeometry);
  if (brushShop.racks.size() == 0)
  {
    return MS::kFailure;
  }

  return MS::kSuccess;
}
