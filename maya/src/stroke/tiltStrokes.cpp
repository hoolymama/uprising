
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include "strokeData.h"
#include "tiltStrokes.h"
#include "stroke.h"
#include "cImgUtils.h"

#include <jMayaIds.h>
// #include "mayaMath.h"
#include "errorMacros.h"
#include "texUtils.h"

const double rad_to_deg = (180 / 3.1415927);
const double deg_to_rad = (3.1415927 / 180);

MObject tiltStrokes::aCanvasMatrix;
MObject tiltStrokes::aGradientTexture;
MObject tiltStrokes::aSampleDistance;
MObject tiltStrokes::aStrength;

MTypeId tiltStrokes::id(k_tiltStrokes);

tiltStrokes::tiltStrokes() {}

tiltStrokes::~tiltStrokes() {}

void *tiltStrokes::creator()
{
  return new tiltStrokes();
}

/// Post constructor
void tiltStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

const double epsilon = 0.0001;

MStatus tiltStrokes::initialize()
{
  MStatus st;
  MString method("tiltStrokes::initialize");
	inheritAttributesFrom("strokeMutator");


  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnMatrixAttribute mAttr;

  MFloatMatrix identity;
  identity.setToIdentity();
  aCanvasMatrix = mAttr.create("canvasMatrix", "cmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  mAttr.setDefault(identity);
  addAttribute(aCanvasMatrix);

  aGradientTexture = nAttr.createColor("gradientTexture", "gtx");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aGradientTexture);

  aSampleDistance = nAttr.create("sampleDistance",
                                           "sds", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setMin(0.00);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(0.01);
  addAttribute(aSampleDistance);

  aStrength = nAttr.create("strength",  "stn", MFnNumericData::kFloat);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setSoftMax(1.0);
  nAttr.setDefault(1.0);
  addAttribute(aStrength);

  st = attributeAffects(aCanvasMatrix, aOutput);
  st = attributeAffects(aGradientTexture, aOutput);
  st = attributeAffects(aSampleDistance, aOutput);
  st = attributeAffects(aStrength, aOutput);

  return (MS::kSuccess);
}

MStatus tiltStrokes::mutate(
  const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const 
{
 
  MStatus st;
  MObject thisObj = thisMObject();
  // JPMDBG;
  if (!TexUtils::hasTexture(thisObj, tiltStrokes::aGradientTexture))
  {
    return MS::kUnknownParameter;
  }
  // JPMDBG;
  float sampleDist = data.inputValue(aSampleDistance).asFloat();
  float strength = data.inputValue(aStrength).asFloat();
  // JPMDBG;
  if (sampleDist == 0.0 || strength == 0.0)
  {
    return MS::kUnknownParameter;
  }
  // JPMDBG;
  MFloatMatrix canvasMatrix = data.inputValue(aCanvasMatrix).asFloatMatrix();
  MFloatVector canvasNormal((MFloatVector::zAxis * canvasMatrix).normal());
  // JPMDBG;

  MFloatPointArray points;
  std::vector<Stroke>::const_iterator csiter = strokes->begin();
  for (;csiter != strokes->end(); csiter++)
  {
    Stroke::const_target_iterator ctiter = csiter->targets_begin();
    for (; ctiter != csiter->targets_end(); ctiter++)
    {
      points.append(ctiter->position());
    }
  }
  // JPMDBG;
  MFloatVectorArray gradients;
  st = TexUtils::sample3dGradient(
      thisObj,
      tiltStrokes::aGradientTexture,
      sampleDist,
      strength,
      points,
      gradients);
  if (st.error())
  {
    return MS::kUnknownParameter;
  }
  // JPMDBG;


  std::vector<Stroke>::iterator siter = strokes->begin();
  for (unsigned i = 0; siter != strokes->end(); siter++)
  {
    Stroke::target_iterator titer = siter->targets_begin();
    for (; titer != siter->targets_end(); titer++)
    {
      const MFloatVector &gradient = gradients[i];
      i++;

      MFloatVector axis = (gradient ^ canvasNormal);
      float mag = axis.length();
      if (mag < 0.00001)
      {
        continue;
      }
      MFloatMatrix rotMat = MFloatMatrix(MQuaternion(mag, axis).asMatrix().matrix); 
      titer->rotate(rotMat);
      }
  }
  // JPMDBG;
  return MS::kSuccess;
}

