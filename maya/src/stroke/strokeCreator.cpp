
#include <maya/MIOStream.h>
#include <vector>

#include <maya/MFnPluginData.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include "strokeData.h"
#include "strokeCreator.h"
#include "stroke.h"

#include <jMayaIds.h>
#include "errorMacros.h"

MObject strokeCreator::aTargetRotationMatrix; 
MObject strokeCreator::aPointDensity; 


MTypeId strokeCreator::id(k_strokeCreator);

strokeCreator::strokeCreator() {}

strokeCreator::~strokeCreator() {}

void *strokeCreator::creator()
{
  return new strokeCreator();
}

void strokeCreator::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus strokeCreator::initialize()
{
  MStatus st;
  MString method("strokeCreator::initialize");

  inheritAttributesFrom("strokeNodeBase");

  MFnMatrixAttribute mAttr;
  MFnNumericAttribute nAttr;

  aPointDensity = nAttr.create("pointDensity", "pd", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setDefault(1.0f);
  st = addAttribute(aPointDensity);
  mser;

  aTargetRotationMatrix= mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  
  addAttribute(aTargetRotationMatrix);

  st = attributeAffects(aPointDensity, aOutput);
  st = attributeAffects(aTargetRotationMatrix, aOutput);

  return (MS::kSuccess);
}

MStatus strokeCreator::generateStrokeGeometry(
    MDataBlock &data,
    std::vector<Stroke> *pOutStrokes) 
{
  return (MS::kUnknownParameter);
}
