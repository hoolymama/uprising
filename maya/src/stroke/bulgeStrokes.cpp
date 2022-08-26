
#include <maya/MIOStream.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MRampAttribute.h>

#include "strokeData.h"
#include "bulgeStrokes.h"
#include "stroke.h"
// #include "cImgUtils.h"

#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

// const double rad_to_deg = (180 / 3.1415927);
// const double deg_to_rad = (3.1415927 / 180);

MObject bulgeStrokes::aMult;
MObject bulgeStrokes::aRamp;

MTypeId bulgeStrokes::id(k_bulgeStrokes);

bulgeStrokes::bulgeStrokes() {}

bulgeStrokes::~bulgeStrokes() {}

void *bulgeStrokes::creator()
{
  return new bulgeStrokes();
}

/// Post constructor
void bulgeStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus bulgeStrokes::initialize()
{
  MStatus st;
  MString method("bulgeStrokes::initialize");
  inheritAttributesFrom("strokeMutator");

	MRampAttribute rAttr;
	MFnNumericAttribute nAttr;

  
	aRamp = rAttr.createCurveRamp("ramp", "rmp");
	st = addAttribute(aRamp ); mser;

  aMult = nAttr.create("mult", "mlt", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aMult);

  st = attributeAffects(aRamp, aOutput);
  st = attributeAffects(aMult, aOutput);

  return (MS::kSuccess);
}

MStatus bulgeStrokes::mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{

  MStatus st;
  MObject thisObj = thisMObject();


  MRampAttribute rampAttr( thisObj, aRamp, &st );
  float mult = data.inputValue(aMult).asFloat();

  std::vector<Stroke>::iterator siter = strokes->begin();
  for (;siter != strokes->end(); siter++)
  {
    MFloatArray params;
    siter->calculateParams(params);

    Stroke::target_iterator titer = siter->targets_begin();
    for (unsigned i =0; titer != siter->targets_end(); titer++,i++)
    {

  		float weight;
		  rampAttr.getValueAtPosition( params[i], weight, &st ); mser;
      weight *= mult;
      
      titer->setRadius(titer->radius()*weight );
    }
  }
  return MS::kSuccess;
}
