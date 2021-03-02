
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
MObject strokeMutator::aOutput;
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

  MFnTypedAttribute tAttr;
  MFnNumericAttribute nAttr;

  aStrokes = tAttr.create("strokes", "stks", strokeData::id);
  tAttr.setReadable(false);
  tAttr.setStorable(false);
  tAttr.setKeyable(true);
  tAttr.setDisconnectBehavior(MFnAttribute::kReset);
  addAttribute(aStrokes);


	aTrigger = nAttr.create( "trigger", "trg",MFnNumericData::kFloat);
	nAttr.setHidden(false);
	nAttr.setStorable(false);
	nAttr.setReadable(false);
  nAttr.setKeyable(true);
	nAttr.setArray(true);
	nAttr.setIndexMatters(false);
  addAttribute(aTrigger);

  aOutput = tAttr.create("output", "out", strokeData::id);
  tAttr.setReadable(true);
  tAttr.setStorable(false);
  addAttribute(aOutput);

  st = attributeAffects(aStrokes, aOutput);
  st = attributeAffects(aTrigger, aOutput);


  return (MS::kSuccess);
}

MStatus strokeMutator::compute(const MPlug &plug, MDataBlock &data)
{
  MStatus st;
  if (plug != aOutput)
  {
    return (MS::kUnknownParameter);
  }

  // Pull on the trigger plug
  //////////////////////////////////////////////////
	MArrayDataHandle hTrigger = data.inputArrayValue(aTrigger);
  unsigned nt = hTrigger.elementCount();
  for(unsigned j = 0;j < nt; j++, hTrigger.next()) {
    MDataHandle hTriggerEl = hTrigger.inputValue();
    hTrigger.inputValue().asFloat();
  }


  // Get the geometry from the out plug and clear it
  //////////////////////////////////////////////////
  
  MDataHandle hOutput = data.outputValue(aOutput);
  MFnPluginData fnOut;
  MTypeId kdid(strokeData::id);

  MObject dOut = fnOut.create(kdid, &st);
  strokeData *newData = (strokeData *)fnOut.data(&st);
  mser;
  std::vector<Stroke> *geom = newData->fGeometry;
  geom->clear();
//////////////////////////////////////////////////

  // Get the input strokes and copy them into the output
  //////////////////////////////////////////////////
  MDataHandle hInStrokes = data.inputValue(aStrokes, &st);
  msert;

  MObject dInStrokes = hInStrokes.data();
  MFnPluginData fnInStrokes(dInStrokes, &st);
  msert;

  strokeData *sData = (strokeData *)fnInStrokes.data();
  const std::vector<Stroke> *strokeGeom = sData->fGeometry;

  std::vector<Stroke>::const_iterator citer;
  for (citer = strokeGeom->begin(); citer != strokeGeom->end(); citer++)
  {
    geom->push_back(*citer);
  }
  //////////////////////////////////////////////////

  // Call the overridden mutate function
  //////////////////////////////////////////////////
  st = mutate(data, geom); 
  mser;
    //////////////////////////////////////////////////


  hOutput.set(newData);
  data.setClean(plug);

  return MS::kSuccess;
}

MStatus strokeMutator::mutate(MDataBlock &data, std::vector<Stroke> *geom) const
{
  return MS::kUnknownParameter;
}

void strokeMutator::getPoints(
  MDataBlock &data, 
  std::vector<Stroke> *geom, 
  MFloatPointArray &points) const
{
  for (std::vector<Stroke>::const_iterator iter = geom->begin(); iter != geom->end(); iter++)
  {
    const std::vector<Target> targets = iter->targets();
    for (std::vector<Target>::const_iterator targetIter = targets.begin(); targetIter != targets.end(); targetIter++)
    {
      const MMatrix &mat = targetIter->matrix();
      points.append(MFloatPoint(mat[3][0],mat[3][1],mat[3][2]));
    }
  }
}