
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

  attributeAffects(aSampleParam, aOutput);
  attributeAffects(aDoPaintId, aOutput);
  attributeAffects(aPaintIdMap, aOutput);
  attributeAffects(aPaintIdMapQuantizeLevel, aOutput);
  attributeAffects(aPaintIdOffset, aOutput);
  attributeAffects(aPalette, aOutput);
  attributeAffects(aDoBrushModelId, aOutput);
  attributeAffects(aBrushShop, aOutput);

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
  if (doPaintId)
  {
    st = assignPaintAndPotIds(data, strokes);
  }
  if (doBrushModelId)
  {
    st = assignBrushModelAndShapeIds(data, strokes);
  }

  return MS::kSuccess;
}

MStatus mapIdStrokes::assignPaintAndPotIds(MDataBlock &data,
                                           std::vector<Stroke> *strokes) const
{
  MStatus st;
  int numStrokes = strokes->size();
  float sampleParam = data.inputValue(aSampleParam).asFloat();
  float paintIdMap = data.inputValue(aPaintIdMap).asFloat();
  int quantizeLevels = data.inputValue(aPaintIdMapQuantizeLevel).asInt();
  int paintIdOffset = data.inputValue(aPaintIdOffset).asInt();
  std::map<int, Paint> palette;
  st = getPalette(data, palette);
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
  std::map<int, Paint>::const_iterator paletteIter;
  std::map<int, Paint>::const_iterator paletteEndIter = palette.end();
  std::vector<Stroke>::iterator siter = strokes->begin();
  unsigned index = 0;

  for (; siter != strokes->end(); ++siter, ++index)
  {
    int &paintId = paintIds[index];
    int &potId = potIds[index];

    paintId += paintIdOffset;
    paletteIter = palette.find(paintId);
    if (paletteIter != paletteEndIter)
    {
      potId = paletteIter->second.pot();
    }
    siter->setPaintId(paintId);
    siter->setPotId(potId);
  }
  return MS::kSuccess;
}

MStatus mapIdStrokes::assignBrushModelAndShapeIds(MDataBlock &data,
                                          std::vector<Stroke> *strokes) const
{
  MStatus st;
  BrushShop brushShop;
  st = getBrushShop(data, brushShop); msert;
  std::vector<Stroke>::iterator stroke = strokes->begin();
  for (; stroke != strokes->end(); stroke++)
  {
    // Get the atts we need.
    float width = stroke->maxRadius() * 2.0f;
    Brush::Shape shape = stroke->brushStrokeSpec().shape;
    int modelId = brushShop.findModelId(width, shape, &st) ;
    stroke->setBrushModelId(modelId);
  }
  return MS::kSuccess;
}

MStatus mapIdStrokes::getPalette(MDataBlock &data, std::map<int, Paint> &palette) const
{
  MStatus st;
  MDataHandle h = data.inputValue(aPalette, &st);
  msert;
  MObject d = h.data();
  MFnPluginData fnP(d, &st);
  if (st.error())
  {
    return st;
  }
  paletteData *pData = (paletteData *)fnP.data(&st);
  msert;
  palette = *(pData->fGeometry);
  if (palette.size() == 0)
  {
    return MS::kFailure;
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
