
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include "aimStrokes.h"
#include <jMayaIds.h>
#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

// MObject aimStrokes::aStrokes;
MObject aimStrokes::aPoint;
// MObject aimStrokes::aOutput;

MTypeId aimStrokes::id(k_aimStrokes);

aimStrokes::aimStrokes() {}

aimStrokes::~aimStrokes() {}

void *aimStrokes::creator()
{
  return new aimStrokes();
}

/// Post constructor
void aimStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus aimStrokes::initialize()
{
  MStatus st;
  MString method("aimStrokes::initialize");

	inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;

  aPoint = nAttr.createPoint("point", "pt");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aPoint);

  st = attributeAffects(aPoint, aOutput);

  return (MS::kSuccess);
}


MStatus aimStrokes::mutate(MDataBlock &data, std::vector<Stroke> *geom) const 
{
  float3 & fpoint = data.inputValue(aPoint).asFloat3();
  MPoint point(fpoint[0], fpoint[1], fpoint[2]);

  std::vector<Stroke>::iterator iter = geom->begin();

  for (unsigned i = 0; iter != geom->end(); iter++)
  {
    iter->applyGlobalAim(point);
  }
  return MS::kSuccess;
}

  