
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPluginData.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MTransformationMatrix.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>

#include <maya/MMeshIntersector.h>

#include "strokeData.h"
#include "scribbleStrokes.h"
#include "stroke.h"
#include "mayaMath.h"
#include <jMayaIds.h>

#include "errorMacros.h"
#include "texUtils.h"
#include "enums.h"


const float PI = 3.14159265359;

MObject scribbleStrokes::aTargetRotationMatrix;

MObject scribbleStrokes::aAngle;
MObject scribbleStrokes::aPointDensity;
MObject scribbleStrokes::aMinimumPoints;

MObject scribbleStrokes::aRadiusGain;
MObject scribbleStrokes::aRadiusOffset;

MObject scribbleStrokes::aTilt;
MObject scribbleStrokes::aBank;
MObject scribbleStrokes::aTwist;

MObject scribbleStrokes::aTiltMap;
MObject scribbleStrokes::aBankMap;
MObject scribbleStrokes::aTwistMap;

MObject scribbleStrokes::aProjectToPlane;
MObject scribbleStrokes::aProjectionPlaneMatrix;

MObject scribbleStrokes::aRotateOrder;

MObject scribbleStrokes::aColorPropagation;
MObject scribbleStrokes::aColorOverride;
MObject scribbleStrokes::aWhiteOverride;
MObject scribbleStrokes::aWeightOverride;

MTypeId scribbleStrokes::id(k_scribbleStrokes);

scribbleStrokes::scribbleStrokes() {}

scribbleStrokes::~scribbleStrokes() {}

void *scribbleStrokes::creator()
{
  return new scribbleStrokes();
}

/// Post constructor
void scribbleStrokes::postConstructor()
{
  MPxNode::postConstructor();
  setExistWithoutOutConnections(true);
}

MStatus scribbleStrokes::initialize()
{
  MStatus st;
  MString method("scribbleStrokes::initialize");

  inheritAttributesFrom("strokeMutator");

  MFnNumericAttribute nAttr;
  MFnUnitAttribute uAttr;
  MFnMatrixAttribute mAttr;
  MFnEnumAttribute eAttr;

  aTargetRotationMatrix = mAttr.create("targetRotationMatrix", "tmat", MFnMatrixAttribute::kFloat);
  mAttr.setStorable(false);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aTargetRotationMatrix);

  aPointDensity = nAttr.create("pointDensity", "den", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aPointDensity);

  aMinimumPoints = nAttr.create("minimumPoints", "mnpts", MFnNumericData::kInt);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(2);
  st = addAttribute(aMinimumPoints);
  mser;
  aAngle = uAttr.create("angleCm", "ang", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  addAttribute(aAngle);

  aRadiusGain = nAttr.create("radiusGain", "rgn", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aRadiusGain);

  aRadiusOffset = nAttr.create("radiusOffset", "rof", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(0.0f);
  addAttribute(aRadiusOffset);

  aTilt = uAttr.create("tilt", "tlt", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  addAttribute(aTilt);

  aBank = uAttr.create("bank", "bnk", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  addAttribute(aBank);

  aTwist = uAttr.create("twist", "tws", MFnUnitAttribute::kAngle);
  uAttr.setHidden(false);
  uAttr.setKeyable(true);
  uAttr.setStorable(true);
  addAttribute(aTwist);

  aTiltMap = nAttr.create("tiltMap", "tim", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aTiltMap);

  aBankMap = nAttr.create("bankMap", "bam", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aBankMap);

  aTwistMap = nAttr.create("twistMap", "twm", MFnNumericData::kFloat);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(1.0f);
  addAttribute(aTwistMap);

  aRotateOrder = eAttr.create("rotateOrder", "ro", PaintingEnums::kTwistTiltBank);
  eAttr.addField("twistTiltBank", PaintingEnums::kTwistTiltBank);
  eAttr.addField("tiltBankTwist", PaintingEnums::kTiltBankTwist);
  eAttr.addField("bankTwistTilt", PaintingEnums::kBankTwistTilt);
  eAttr.addField("tiltTwistBank", PaintingEnums::kTiltTwistBank);
  eAttr.addField("twistBankTilt", PaintingEnums::kTwistBankTilt);
  eAttr.addField("bankTiltTwist", PaintingEnums::kBankTiltTwist);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  st = addAttribute(aRotateOrder);

  aProjectToPlane = nAttr.create("projectToPlane", "ptp",  MFnNumericData::kBoolean);
  nAttr.setHidden(false);
  nAttr.setKeyable(true);
  nAttr.setStorable(true);
  nAttr.setWritable(true);
  nAttr.setDefault(true);
  addAttribute(aProjectToPlane);

  aProjectionPlaneMatrix = mAttr.create("projectionPlaneMatrix", "ppm", MFnMatrixAttribute::kDouble);
  mAttr.setStorable(true);
  mAttr.setHidden(false);
  mAttr.setKeyable(true);
  addAttribute(aProjectionPlaneMatrix);


  aColorPropagation = eAttr.create("ColorPropagation", "cpr", scribbleStrokes::kOverride);
  eAttr.addField("interpolate", scribbleStrokes::kInterpolate);
  eAttr.addField("kOverride", scribbleStrokes::kOverride);
  eAttr.setHidden(false);
  eAttr.setKeyable(true);
  st = addAttribute(aColorPropagation);

  aColorOverride = nAttr.createColor("colorOverride", "cov");
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aColorOverride);

  aWhiteOverride = nAttr.create("whiteOverride", "whov", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWhiteOverride);

  aWeightOverride = nAttr.create("weightOverride", "wgov", MFnNumericData::kFloat);
  nAttr.setStorable(true);
  nAttr.setReadable(true);
  nAttr.setKeyable(true);
  addAttribute(aWeightOverride);

  attributeAffects(aTargetRotationMatrix, aOutput);
  attributeAffects(aPointDensity, aOutput);
  attributeAffects(aMinimumPoints, aOutput);

  attributeAffects(aAngle, aOutput);
  attributeAffects(aRadiusOffset, aOutput);
  attributeAffects(aRadiusGain, aOutput);

  attributeAffects(aTilt, aOutput);
  attributeAffects(aBank, aOutput);
  attributeAffects(aTwist, aOutput);
  attributeAffects(aTiltMap, aOutput);
  attributeAffects(aBankMap, aOutput);
  attributeAffects(aTwistMap, aOutput);
  attributeAffects(aRotateOrder, aOutput);

  attributeAffects(aProjectToPlane, aOutput);
  attributeAffects(aProjectionPlaneMatrix, aOutput);

  attributeAffects(aColorPropagation, aOutput);
  attributeAffects(aColorOverride, aOutput);
  attributeAffects(aWhiteOverride, aOutput);
  attributeAffects(aWeightOverride, aOutput);

  return (MS::kSuccess);
}

MStatus scribbleStrokes::mutate(
    const MPlug &plug,
    MDataBlock &data,
    std::vector<Stroke> *strokes) const
{
  MStatus st;

  MFloatPointArray points;
  getTargetPoints(strokes, points);

  MFloatArray radiusGain;
  MFloatArray radiusOffset;

  MFloatArray tilts;
  MFloatArray banks;
  MFloatArray twists;

  getMappedValues(scribbleStrokes::aRadiusGain, data, points, radiusGain);
  getMappedValues(scribbleStrokes::aRadiusOffset, data, points, radiusOffset);

  bool projectToPlane = data.inputValue(aProjectToPlane).asBool();
  MMatrix projectionMatrix = data.inputValue(aProjectionPlaneMatrix).asMatrix();
  MMatrix projectionMatrixInverse = projectionMatrix.inverse();

  float dummy1 =  data.inputValue(aRadiusOffset).asFloat();
  float dummy2 =  data.inputValue(aRadiusGain).asFloat();
  float dummy3 =  data.inputValue(aTiltMap).asFloat();
  float dummy4 =  data.inputValue(aBankMap).asFloat();
  float dummy5 =  data.inputValue(aTwistMap).asFloat();



  float tilt = float(data.inputValue(aTilt).asAngle().asRadians());
  float bank = float(data.inputValue(aBank).asAngle().asRadians());
  float twist = float(data.inputValue(aTwist).asAngle().asRadians());

  getMappedValues(scribbleStrokes::aTiltMap, data, points, tilt, tilts);
  getMappedValues(scribbleStrokes::aBankMap, data, points, bank, banks);
  getMappedValues(scribbleStrokes::aTwistMap, data, points, twist, twists);

  float pointDensity = fmax(data.inputValue(aPointDensity).asFloat(), 0.001);
  int minimumPoints = data.inputValue(aMinimumPoints).asInt();

  MFloatMatrix targetRotationMatrix = data.inputValue(aTargetRotationMatrix).asFloatMatrix();
  targetRotationMatrix = mayaMath::rotationOnly(targetRotationMatrix);

  float angleCm = float(data.inputValue(aAngle).asAngle().asRadians());

  PaintingEnums::BrushRotateOrder order =  PaintingEnums::BrushRotateOrder(data.inputValue(aRotateOrder).asShort());
  scribbleStrokes::ColorPropagation propagation = scribbleStrokes::ColorPropagation(data.inputValue(aColorPropagation).asShort());

  MFloatVector rgbOverride = data.inputValue(aColorOverride).asFloatVector();
  float whiteOverride = data.inputValue(aWhiteOverride).asFloat();
  float weightOverride = data.inputValue(aWeightOverride).asFloat();
  MColor colorOverride(rgbOverride[0], rgbOverride[1], rgbOverride[2], whiteOverride);

  std::vector<Stroke> sourceStrokes(*strokes);
  strokes->clear();

  std::vector<Stroke>::iterator siter = sourceStrokes.begin();
  std::vector<Stroke>::iterator senditer = sourceStrokes.end();

  bool  doInterp = (propagation == scribbleStrokes::kInterpolate);
  // cerr << "interpolate: " << doInterp << endl; 
  int mapIndex = 0;
  for (; siter != senditer; siter++)
  {

    std::vector<Target>::const_iterator titer;
    std::vector<Target>::const_iterator tenditer = siter->targets().end();

    float angle = 0.0f;
    float distance = 0.0f;
    int len = siter->size();
    MPointArray editPoints(len);

    MFloatArray weights(len);
    MColorArray colors(len);

    
    if (doInterp)
    {
      titer = siter->targets().begin();
      unsigned t = 0;
      for (; titer != tenditer; titer++, t++)
      {
        weights.set(titer->weight(), t);
        colors.set(titer->color(), t);
      }
    }
    // cerr << "Set orig colors: " << colors << endl; 

    std::vector<MFloatMatrix> scribbleTransforms;
    calculateScribbleTransforms(*siter, mapIndex, tilts, banks, twists, order, scribbleTransforms);
    unsigned t = 0; 
    for (titer = siter->targets().begin(); titer != tenditer; titer++, t++, mapIndex++)
    {
      if (titer != siter->targets().begin())
      {
        distance = titer->distanceTo(*std::prev(titer));
      }
      angle += (angleCm * distance);
      float radius = (titer->weight() * radiusGain[mapIndex]) + radiusOffset[mapIndex];
      MVector z = MFloatVector::zAxis * scribbleTransforms[t];
      MVector tangent = MFloatVector::yAxis * scribbleTransforms[t];
      MQuaternion q(angle, z);
      editPoints.set((titer->position() + (tangent * radius).rotateBy(q)), t);
    }

    if (projectToPlane) {
      for (int k = 0; k < editPoints.length(); k++)
      {
        const MPoint & ep = editPoints[k];
        MPoint epLocal = ep * projectionMatrixInverse;
        epLocal.z = 0;
        editPoints[k] = epLocal * projectionMatrix;
      }
    }

    Stroke stroke;
    // cerr << "Making new stroke" << endl;
    if (doInterp)
    {
      // cerr << "doInterp true, so colors = " << colors << endl;
      stroke = Stroke(editPoints, weights, colors, pointDensity, minimumPoints, targetRotationMatrix);
    }
    else
    {
      stroke = Stroke(editPoints, pointDensity, minimumPoints, targetRotationMatrix);

      for ( Stroke::target_iterator ttiter = stroke.targets_begin(); ttiter !=  stroke.targets_end(); ttiter++)
      {
        ttiter->setWeight(weightOverride);
        ttiter->setColor(colorOverride);
      }
    }

    if (stroke.valid())
    {
      strokes->push_back(stroke);
    }
  }
  return MS::kSuccess;
}

/////////////////////////// getMappedValues ///////////////////////////
///////////////////////////////////////////////////////////////////////

void scribbleStrokes::getMappedValues(
    const MObject &attribute,
    MDataBlock &data,
    MFloatPointArray &points,
    float scale,
    MFloatArray &result) const
{
  MStatus st;
  MObject thisObj = thisMObject();
  unsigned len = points.length();
  bool isMapped = false;

  if (TexUtils::hasTexture(thisObj, attribute))
  {
    st = TexUtils::sampleSolidTexture(thisObj, attribute, scale, points, result);
    if (!st.error())
      isMapped = true;
  }

  if (!isMapped)
  {
    float value = data.inputValue(attribute).asFloat() * scale;
    result = MFloatArray(len, value);
  }
}

void scribbleStrokes::getMappedValues(
    const MObject &attribute,
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatArray &result) const
{
  getMappedValues(attribute, data, points, 1.0, result);
}

void scribbleStrokes::getMappedValues(
    const MObject &attribute,
    MDataBlock &data,
    MFloatPointArray &points,
    float scale,
    MFloatVectorArray &result) const
{
  MStatus st;
  MObject thisObj = thisMObject();
  unsigned len = points.length();
  bool isMapped = false;

  if (TexUtils::hasTexture(thisObj, attribute))
  {
    st = TexUtils::sampleSolidTexture(thisObj, attribute, 1.0, points, result);
    if (!st.error())
      isMapped = true;
  }

  if (!isMapped)
  {
    MFloatVector color = data.inputValue(attribute).asFloatVector() * scale;
    result = MFloatVectorArray(len, color);
  }
}
void scribbleStrokes::getMappedValues(
    const MObject &attribute,
    MDataBlock &data,
    MFloatPointArray &points,
    MFloatVectorArray &result) const
{
  getMappedValues(attribute, data, points, 1.0, result);
}

/////////////////////////// ROTATION ///////////////////////////
///////////////////////////////////////////////////////////////////////
void scribbleStrokes::calculateScribbleTransforms(
    const Stroke &stroke,
    int mapIndex,
    const MFloatArray &tilts,
    const MFloatArray &banks,
    const MFloatArray &twists,
    PaintingEnums::BrushRotateOrder order,
    std::vector<MFloatMatrix> &scribbleTransforms) const
{
  std::vector<Target>::const_iterator titer = stroke.targets().begin();
  std::vector<Target>::const_iterator tenditer = stroke.targets().end();

  switch (order)
  {
  case PaintingEnums::kTwistTiltBank:
    for (unsigned m = mapIndex, i = 0; titer != tenditer; titer++, m++, i++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);

      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyTwist(twist, mat);
      applyTilt(tilt, mat);
      applyBank(bank, mat);
      scribbleTransforms.push_back(mat);
    }

    break;
  case PaintingEnums::kTiltBankTwist:
    for (unsigned m = mapIndex; titer != tenditer; titer++, m++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);
      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyTilt(tilt, mat);
      applyBank(bank, mat);
      applyTwist(twist, mat);
      scribbleTransforms.push_back(mat);
    }

    break;
  case PaintingEnums::kBankTwistTilt:
    for (unsigned m = mapIndex; titer != tenditer; titer++, m++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);
      getFollowMatrix(*titer, mat);
      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyBank(bank, mat);
      applyTwist(twist, mat);
      applyTilt(tilt, mat);
      scribbleTransforms.push_back(mat);
    }

    break;
  case PaintingEnums::kTiltTwistBank:
    for (unsigned m = mapIndex; titer != tenditer; titer++, m++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);
      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyTilt(tilt, mat);
      applyTwist(twist, mat);
      applyBank(bank, mat);
      scribbleTransforms.push_back(mat);
    }

    break;
  case PaintingEnums::kTwistBankTilt:
    for (unsigned m = mapIndex; titer != tenditer; titer++, m++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);
      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyTwist(twist, mat);
      applyBank(bank, mat);
      applyTilt(tilt, mat);
      scribbleTransforms.push_back(mat);
    }

    break;
  default: // case scribbleStrokes::kBankTiltTwist:
    for (unsigned m = mapIndex; titer != tenditer; titer++, m++)
    {
      MFloatMatrix mat;
      getFollowMatrix(*titer, mat);
      const float &tilt = tilts[m];
      const float &bank = banks[m];
      const float &twist = twists[m];

      applyBank(bank, mat);
      applyTilt(tilt, mat);
      applyTwist(twist, mat);
      scribbleTransforms.push_back(mat);
    }
    break;
  }
}

void scribbleStrokes::getFollowMatrix(const Target &target, MFloatMatrix &mat) const
{
  mat = target.matrix();
  mat[3][0] = 0.0;
  mat[3][1] = 0.0;
  mat[3][2] = 0.0;
  mat *= MFloatMatrix(MQuaternion(MFloatVector::yAxis * mat, target.drawTangent()).asMatrix().matrix);
}

void scribbleStrokes::applyTilt(float angle, MFloatMatrix &mat) const
{
  MFloatVector axis = MFloatVector::xAxis * mat;
  MFloatMatrix rotMat = MFloatMatrix(MQuaternion(angle, axis).asMatrix().matrix);
  mat *= rotMat;
}

void scribbleStrokes::applyBank(float angle, MFloatMatrix &mat) const
{
  MFloatVector axis = MFloatVector::yAxis * mat;
  MFloatMatrix rotMat = MFloatMatrix(MQuaternion(angle, axis).asMatrix().matrix);
  mat *= rotMat;
}

void scribbleStrokes::applyTwist(float angle, MFloatMatrix &mat) const
{
  MFloatVector axis = MFloatVector::zAxis * mat;
  MFloatMatrix rotMat = MFloatMatrix(MQuaternion(angle, axis).asMatrix().matrix);
  mat *= rotMat;
}
