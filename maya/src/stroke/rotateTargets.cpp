#include <math.h>       /* sin */

#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>


#include "strokeData.h"
#include "rotateTargets.h"
#include "stroke.h"
#include "cImgUtils.h"

#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

MObject rotateTargets::aCanvasMatrix;

MObject rotateTargets::aSineFrequency;
MObject rotateTargets::aSineAmplitude;
MObject rotateTargets::aRandomSineOffset;
MObject rotateTargets::aSeed;
MObject rotateTargets::aAxis;
  

MTypeId rotateTargets::id(k_rotateTargets);

rotateTargets::rotateTargets() {}

rotateTargets::~rotateTargets() {}

void *rotateTargets::creator()
{
  return new rotateTargets();
}

/// Post constructor
void rotateTargets::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus rotateTargets::initialize()
{
  MStatus st;
  MString method("rotateTargets::initialize");
  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;
  

  MFloatMatrix identity;
  identity.setToIdentity();
  aCanvasMatrix = mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aCanvasMatrix);


  aAxis = eAttr.create("axis", "ax");
  eAttr.addField("canvas", RotationAxis::kCanvas);
  eAttr.addField("brushTilt", RotationAxis::kTilt);
  eAttr.addField("brushTwist", RotationAxis::kTwist);
  eAttr.addField("brushBank", RotationAxis::kBank);
  eAttr.setHidden(false);
  eAttr.setStorable(true);
  eAttr.setReadable(true);
  eAttr.setKeyable(true);
  eAttr.setDefault(RotationAxis::kCanvas);
  addAttribute(aAxis);


  aSeed = nAttr.create("seed", "sd", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  nAttr.setDefault(0);
  st = addAttribute(aSeed);
  mser;

  aSineFrequency = nAttr.create("sineFrequency",
                                 "snf", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aSineFrequency);

 
  aSineAmplitude = nAttr.create("sineAmplitude",
                                 "sna", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aSineAmplitude);


 
  aRandomSineOffset = nAttr.create("randomSineOffset",
                                 "rsno", MFnNumericData::k2Float);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aRandomSineOffset);

  st = attributeAffects(aAxis, aOutput);
  st = attributeAffects(aSeed, aOutput);
  st = attributeAffects(aSineFrequency, aOutput);
  st = attributeAffects(aSineAmplitude, aOutput);
  st = attributeAffects(aRandomSineOffset, aOutput);

  st = attributeAffects(aCanvasMatrix, aOutput);

  return (MS::kSuccess);
}

MStatus rotateTargets::mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const
{

  MStatus st;
  MObject thisObj = thisMObject();

  float sineFrequency = data.inputValue(aSineFrequency).asFloat();
  float sineAmplitude = data.inputValue(aSineAmplitude).asFloat();
  const float2 &randomSineOffset = data.inputValue(aRandomSineOffset).asFloat2();
  int seedVal = data.inputValue( aSeed).asInt()  ;
  srand48(seedVal);

  if (sineFrequency <= 0.00001 || sineAmplitude <= 0.00001)
  {
    return MS::kUnknownParameter;
  }

  MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
  MFloatVector axis((MFloatVector::zAxis * canvasMatrix).normal());


  RotationAxis axisType = RotationAxis(data.inputValue(aAxis).asShort());
  



  std::vector<Stroke>::iterator siter = strokes->begin();
  for (;siter != strokes->end(); siter++)
  {
    MFloatArray params;
    siter->calculateParams(params);
		float offset = float(drand48()) ;
    offset = randomSineOffset[0]+(offset*randomSineOffset[1]-randomSineOffset[0]);
    
    Stroke::target_iterator titer = siter->targets_begin();
    for (unsigned i =0; titer != siter->targets_end(); titer++,i++)
    {
      if (axisType != RotationAxis::kCanvas) {
        if (axisType == RotationAxis::kTwist) {
          axis = titer->zAxis();
        } else if (axisType == RotationAxis::kBank) {
          axis = titer->yAxis();
        } else { // Tilt
          axis = titer->xAxis();
        }
      }

      float twistVal = sin((offset + params[i]) * sineFrequency) * sineAmplitude;
      if (fabs(twistVal) < 0.00001)
      {
        continue;
      }
      MFloatMatrix rotMat = MFloatMatrix(MQuaternion(twistVal, axis).asMatrix().matrix);
      titer->rotate(rotMat);
    }
  }

  return MS::kSuccess;
}
