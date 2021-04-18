#include <map>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include "brushLifter.h"
#include <jMayaIds.h>
#include "errorMacros.h"
// #include "texUtils.h"

#include "brushData.h"

MObject brushLifter::aBrushes;

MTypeId brushLifter::id(k_brushLifter);

brushLifter::brushLifter() {}

brushLifter::~brushLifter() {}

void *brushLifter::creator()
{
  return new brushLifter();
}

void brushLifter::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus brushLifter::initialize()
{
  MStatus st;
  MString method("brushLifter::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnTypedAttribute tAttr;

  aBrushes = tAttr.create("brushes", "bsh", brushData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aBrushes);

  st = attributeAffects(aBrushes, aOutput);

  return (MS::kSuccess);
}

MStatus brushLifter::mutate(const MPlug&plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{

  MStatus st;
  std::map<int, Brush> brushes;
  st = collectBrushes(data, brushes);
  msert;
  std::map<int, Brush>::const_iterator brushIter;
  const float epsilon = 0.0001f;
  std::vector<Stroke>::iterator currentStroke = strokes->begin();
  
  for (unsigned i = 0; currentStroke != strokes->end(); currentStroke++)
  {
    
    int brushId = currentStroke->brushId();

    brushIter = brushes.find(brushId);
    if (brushIter == brushes.end())
    {
      brushIter = brushes.find(-1);
    }
    const Brush &brush = brushIter->second;
    float height = brush.transitionHeight();
    float power = brush.contactPower();

    Stroke::target_iterator currentTarget = currentStroke->targets_begin();
    for (; currentTarget != currentStroke->targets_end(); currentTarget++)
    {
      float lift = (1.0 - currentTarget->weight());
      if (lift > epsilon)
      {
        lift = pow(lift, power) * height;
        MFloatVector offset = currentTarget->zAxis() * -lift;
        currentTarget->offsetBy(offset);
      }
    }
  }

  return MS::kSuccess;
}

MStatus brushLifter::collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
  msert;

  brushes[-1] = Brush();

  unsigned nPlugs = ha.elementCount();
  for (unsigned i = 0; i < nPlugs; i++, ha.next())
  {
    int index = ha.elementIndex(&st);
    if (st.error())
    {
      continue;
    }
    MDataHandle h = ha.inputValue(&st);
    if (st.error())
    {
      continue;
    }

    MObject d = h.data();
    MFnPluginData fnP(d, &st);
    if (st.error())
    {
      continue;
    }
    brushData *bData = (brushData *)fnP.data();

    brushes[index] = *(bData->fGeometry);
  }
  return MS::kSuccess;
}
