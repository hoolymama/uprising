
#include <vector>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include "strokeData.h"
#include "collectStrokes.h"
#include "stroke.h"
#include <jMayaIds.h>
#include "errorMacros.h"

 
MObject collectStrokes::aStrokes;

MTypeId collectStrokes::id(k_collectStrokes);

collectStrokes::collectStrokes() {}

collectStrokes::~collectStrokes() {}

void *collectStrokes::creator()
{
  return new collectStrokes();
}

void collectStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus collectStrokes::initialize()
{
  MStatus st;
  MString method("collectStrokes::initialize");

  inheritAttributesFrom("strokeNodeBase");

 
  MFnTypedAttribute tAttr;
 
  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setKeyable(true);
  tAttr.setIndexMatters(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aStrokes);

  st = attributeAffects(aStrokes, aOutput);

  return (MS::kSuccess);
}

MStatus collectStrokes::generateStrokeGeometry(const MPlug &plug, MDataBlock &data, std::vector<Stroke> *pStrokes) 
{
  MStatus st;
  MArrayDataHandle hStrokes = data.inputValue(aStrokes, &st);
  msert;
  unsigned nInputs = hStrokes.elementCount();

  for (unsigned i = 0; i < nInputs; i++, hStrokes.next())
  {
    int index = hStrokes.elementIndex(&st);
    MDataHandle hStrokeInput = hStrokes.inputValue(&st);
    if (st.error())
    {
      continue;
    }
    MObject dStrokeInput = hStrokeInput.data();
    MFnPluginData fnStrokeInput(dStrokeInput, &st);
    if (st.error())
    {
      continue;
    }
    strokeData *sData = (strokeData *)fnStrokeInput.data();
    const std::vector<Stroke> *strokeGeom = sData->fGeometry;

    std::vector<Stroke>::const_iterator citer;
    for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
    {
      pStrokes->push_back(*citer);
      pStrokes->back().setParentId(index);
    }
  }
  strokeNodeBase::generateStrokeGeometry(plug,data,pStrokes);
  return MS::kSuccess;
}
