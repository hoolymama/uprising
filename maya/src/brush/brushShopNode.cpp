#include <map>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPluginData.h>
#include <jMayaIds.h>
#include "errorMacros.h"

#include "brushShopNode.h"
#include "brushData.h"
#include "brushShopData.h"
#include "brushShop.h"

MObject brushShopNode::aBrushes;
MObject brushShopNode::aOutput;

MTypeId brushShopNode::id(k_brushShopNode);

brushShopNode::brushShopNode() {}

brushShopNode::~brushShopNode() {}

void *brushShopNode::creator()
{
  return new brushShopNode();
}

void brushShopNode::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus brushShopNode::initialize()
{
  MStatus st;
  MString method("brushShopNode::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnTypedAttribute tAttr;

  aBrushes = tAttr.create("brushes", "bsh", brushData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setArray(true);
  tAttr.setIndexMatters(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
  addAttribute(aBrushes);

  aOutput = tAttr.create("output", "out", brushShopData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aBrushes, aOutput);

  return (MS::kSuccess);
}

MStatus brushShopNode::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;

  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  std::map<int, Brush> brushes;
  st = collectBrushes(data, brushes);

  BrushShop shop;

  shop.insertRack(std::make_pair(Brush::kRound, BrushRack(brushes, Brush::kRound)));
  shop.insertRack(std::make_pair(Brush::kFlat, BrushRack(brushes, Brush::kFlat)));

  // cerr << "shop: " << shop << endl;

  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(brushShopData::id);
  MObject dOut = fnOut.create(kdid, &st);
  mser;
  brushShopData *newData = (brushShopData *)fnOut.data(&st);
  mser;
  *(newData->fGeometry) = shop;
  hOutput.set(newData);
  hOutput.setClean();

  return (MS::kSuccess);
}

MStatus brushShopNode::collectBrushes(MDataBlock &data, std::map<int, Brush> &brushes) const
{
  MStatus st;
  MArrayDataHandle ha = data.inputArrayValue(aBrushes, &st);
  msert;
 
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

    Brush b = *(bData->fGeometry);
    if (b.inService())
    {
      brushes[index] = b;
    }
    // brushes[index] = *(bData->fGeometry);
  }
  return MS::kSuccess;
}
