
#include <maya/MIOStream.h>
#include <vector>

#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include "strokeData.h"
#include "strokeMutator.h"
#include "stroke.h"

#include <jMayaIds.h>
#include "errorMacros.h"

MObject strokeMutator::aStrokes;

MObject strokeMutator::aTrigger;

MTypeId strokeMutator::id(k_strokeMutator);

strokeMutator::strokeMutator() {}

strokeMutator::~strokeMutator() {}

void *strokeMutator::creator()
{
  return new strokeMutator();
}

void strokeMutator::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus strokeMutator::initialize()
{
  MStatus st;
  MString method("strokeMutator::initialize");

  inheritAttributesFrom("strokeNodeBase");

  MFnTypedAttribute tAttr;
  MFnNumericAttribute nAttr;

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aStrokes);

  aTrigger = nAttr.create("trigger", "trg", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setStorable(false);
  nAttr.setReadable(false);
  nAttr.setKeyable(true);
  nAttr.setArray(true);
  nAttr.setIndexMatters(false);
  addAttribute(aTrigger);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aTrigger, aOutput);

  return (MS::kSuccess);
}




MStatus strokeMutator::generateStrokeGeometry(
  const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes) 
{

  MStatus st;

  short int nodeState = data.inputValue( state).asShort();

  // Pull on the trigger plug
  //////////////////////////////////////////////////
  MArrayDataHandle hTrigger = data.inputArrayValue(aTrigger);
  unsigned nt = hTrigger.elementCount();
  for (unsigned j = 0; j < nt; j++, hTrigger.next())
  {
    MDataHandle hTriggerEl = hTrigger.inputValue();
    hTrigger.inputValue().asFloat();
  }

  // Get the input strokes and copy them into the output
  //////////////////////////////////////////////////
  MDataHandle hInStrokes = data.inputValue(aStrokes, &st);
  msert;

  MObject dInStrokes = hInStrokes.data();
  MFnPluginData fnInStrokes(dInStrokes, &st);
  msert;

  strokeData *sData = (strokeData *)fnInStrokes.data();
  const std::vector<Stroke> *pInStrokes = sData->fGeometry;
 
  std::vector<Stroke>::const_iterator citer;
  for (citer = pInStrokes->begin(); citer != pInStrokes->end(); citer++)
  {
    pOutStrokes->push_back(*citer);
  }
 
	if (nodeState == 0)  {
    mutate(plug, data, pOutStrokes);
	}
  strokeNodeBase::generateStrokeGeometry(plug,data,pOutStrokes);
  return (MS::kSuccess);
}

MStatus strokeMutator::mutate(
  const MPlug &plug, 
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  return MS::kUnknownParameter;
}
