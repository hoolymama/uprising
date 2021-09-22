
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MRampAttribute.h>

#include <maya/MFnCompoundAttribute.h>

#include "strokeData.h"
#include "mapStrokes.h"
#include "stroke.h"

#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"


MObject mapStrokes::aPaintId;
MObject mapStrokes::aPotId;
MObject mapStrokes::aPaintPot;
MObject mapStrokes::aPaintPotTexture;


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

const double epsilon = 0.0001;

MStatus mapStrokes::initialize()
{
  MStatus st;
  MString method("mapStrokes::initialize");
  inheritAttributesFrom("strokeMutator");

	MFnNumericAttribute nAttr;
  MFnCompoundAttribute cAttr;

  aPaintPotTexture = nAttr.createColor("paintPotTexture", "pptx");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPaintPotTexture);

  aPaintId = nAttr.create("paintId", "pnid", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(0);

  aPotId = nAttr.create("potId", "poid", MFnNumericData::kInt);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setDefault(0);

  aPaintPot = cAttr.create("paintPot", "pp");
  cAttr.addChild(aPaintId);
  cAttr.addChild(aPotId);
  cAttr.setArray(true);
  st = addAttribute(aPaintPot);
  mser;

  st = attributeAffects(aPaintPotTexture, aOutput);
  st = attributeAffects(aPaintId, aOutput);
  st = attributeAffects(aPotId, aOutput);
  st = attributeAffects(aPaintPot, aOutput);
  

  return (MS::kSuccess);
}

MStatus mapStrokes::mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  if (!TexUtils::hasTexture(thisObj, mapStrokes::aPaintPotTexture))
  {
    return MS::kUnknownParameter;
  }

  std::vector<std::pair<int,int>> paintsPots;
  getPaintPotIds(data,paintsPots);
  
  float greyRange = float(paintsPots.size());
  int maxPaintId = paintsPots.size() - 1;

  // Points halfway along strokes
  MFloatPointArray points;
  std::vector<Stroke>::const_iterator csiter = strokes->begin();
  for (; csiter != strokes->end(); csiter++)
  {
    int halfIndex = int(csiter->targets().size() / 2);
    points.append(csiter->targets()[halfIndex].position());
  }

  MFloatArray vals;
  st = TexUtils::sampleSolidTexture(
      thisObj,
      mapStrokes::aPaintPotTexture,
      greyRange,
      points,
      vals);
  if (st.error())
  {
    return MS::kUnknownParameter;
  }

  if (! (strokes->size() == vals.length()))
  {
    return MS::kUnknownParameter;
  }
  int i = 0;
  std::vector<Stroke>::iterator siter = strokes->begin();
  for (;siter != strokes->end(); siter++,i++ )
  {
    int index = std::min(int(vals[i]), maxPaintId);
    siter->setPaintId(paintsPots[index].first);
    siter->setPotId(paintsPots[index].second);
  }
  return MS::kSuccess;
}



void mapStrokes::getPaintPotIds( MDataBlock &data, std::vector<std::pair<int,int>> &paintsPots) const 
{
  MStatus st;
  MArrayDataHandle hPaintPotArray = data.inputArrayValue(aPaintPot, &st);
  if (st.error())
  {
    return;
  }
  unsigned nInputs = hPaintPotArray.elementCount();
  for (unsigned i = 0; i < nInputs; i++, hPaintPotArray.next())
  {
    MDataHandle hPaintPot = hPaintPotArray.inputValue(&st);
    if (st.error())
    {
      continue;
    }
    int paintId = hPaintPot.child(aPaintId).asInt();
    int potId = hPaintPot.child(aPotId).asInt();
    paintsPots.push_back(std::make_pair(paintId,potId));
  }
}
